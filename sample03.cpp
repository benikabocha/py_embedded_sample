#include <pybind11/eval.h>
#include <pybind11/pybind11.h>

#include <iostream>
#include <memory>

class Job {
public:
	std::string GetName() const { return m_name; }
	void SetName(const std::string& name) { m_name = name; }

private:
	std::string m_name;
};

class Person {
public:
	std::string GetName() const { return m_name; }
	void SetName(const std::string& name) { m_name = name; }

	std::shared_ptr<Job> GetJob() const { return m_job; }
	void SetJob(const std::shared_ptr<Job>& job) { m_job = job; }

private:
	std::string m_name;
	std::shared_ptr<Job> m_job;
};

namespace py = pybind11;

PyMODINIT_FUNC PyInit_sample03() {
	py::module m("sample03", "pybind11 module sample.");

	py::class_<Job, std::shared_ptr<Job>> job(m, "Job");
	job.def(py::init<>()).def_property("name", &Job::GetName, &Job::SetName);

	py::class_<Person, std::shared_ptr<Person>> person(m, "Person");
	person.def(py::init<>())
		.def_property("name", &Person::GetName, &Person::SetName)
		.def_property("job", &Person::GetJob, &Person::SetJob);

	return m.ptr();
}

int main(int argc, char** argv) {
	wchar_t* program = Py_DecodeLocale(argv[0], nullptr);
	if (program == nullptr) {
		fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
		exit(1);
	}

	PyImport_AppendInittab("sample03", PyInit_sample03);

	Py_SetProgramName(program);
	Py_Initialize();

	py::module::import("sample03");

	try {
		auto global = py::dict(py::module::import("__main__").attr("__dict__"));

		// Python で関数を作成 (Name=Hoge, Job=Teacher)
		py::eval<py::eval_statements>(
			"import sample03\n"
			"def initialize_person(p):\n"
			"	job = sample03.Job()\n"
			"	job.name = 'Teacher'\n"
			"	p.name = 'Hoge'\n"
			"	p.job = job\n",
			global);
		{
			auto person = std::make_shared<Person>();
			global["initialize_person"](person);
			std::cout << "Name : " << person->GetName() << "\n";
			std::cout << "Job  : " << person->GetJob()->GetName() << "\n";
		}

		// 関数を変更 (Name=Foo, Job=Programmer)
		py::eval<py::eval_statements>(
			"import sample03\n"
			"def initialize_person(p):\n"
			"	job = sample03.Job()\n"
			"	job.name = 'Programmer'\n"
			"	p.name = 'Foo'\n"
			"	p.job = job\n",
			global);
		{
			auto person = std::make_shared<Person>();
			global["initialize_person"](person);
			std::cout << "Name : " << person->GetName() << "\n";
			std::cout << "Job  : " << person->GetJob()->GetName() << "\n";
		}
	} catch (py::error_already_set& e) {
		std::cout << "Python error.\n" << e.what() << "\n";
	}

	Py_Finalize();
	PyMem_RawFree(program);

	return 0;
}

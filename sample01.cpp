#include <pybind11/pybind11.h>
#include <pybind11/eval.h>

#include <iostream>

namespace py = pybind11;

int main(int argc, char** argv)
{
	wchar_t *program = Py_DecodeLocale(argv[0], NULL);
	if (program == NULL) {
		fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
		exit(1);
	}
	Py_SetProgramName(program);
	Py_Initialize();

	try
	{
		auto global = py::dict(py::module::import("__main__").attr("__dict__"));
		auto local = py::dict();

		// http://docs.python.jp/3.5/extending/embedding.html �̃T���v���� pybind11 �ŋL�q
		py::eval<py::eval_single_statement>("from time import time,ctime", global, local);
		py::eval("print('Today is', ctime(time()))", global, local);

		// �ϐ��̒�`
		local["x"] = 100;
		py::eval<py::eval_single_statement>("y = 200", global, local);
		py::eval("print('x + y =', x + y)", global, local);

		// �l�̎擾
		py::eval<py::eval_single_statement>("z = x + y", global, local);
		auto z = local["z"].cast<int>();
		std::cout << "cpp: z = " << z << "\n";

		// �֐���`�ƌĂяo��
		py::eval<py::eval_statements>(
			"def func_01():\n"
			"	print('func_01: call')\n",
			global,
			local
			);
		auto func_01 = local["func_01"];
		func_01();

		// �֐��ֈ�����n��
		py::eval<py::eval_statements>(
			"def func_02(a, b):\n"
			"	print('func_02: {} + {} = {}'.format(a, b, a + b))\n",
			global,
			local
			);
		auto func_02 = local["func_02"];
		func_02(123, 456);
		func_02("abc", "efg");

		// �֐��̖߂�l���擾����
		py::eval<py::eval_statements>(
			"def func_03(a, b):\n"
			"	return a + b\n",
			global,
			local
			);
		auto func_03 = local["func_03"];
		{
			auto result = func_03(123, 456);
			std::cout << "cpp: func_03 result " << py::str(result).cast<std::string>() << "\n";
		}
		{
			auto result = func_03("abc", "def");
			std::cout << "cpp: func_03 result " << py::str(result).cast<std::string>() << "\n";
		}

		std::cout << "\n\nAll local variable.\n";
		for (auto it = local.begin(); it != local.end(); ++it)
		{
			std::cout << py::str((*it).first).cast<std::string>() << " : ";
			std::cout << py::str((*it).second).cast<std::string>() << " : ";
			std::cout << py::str((*it).second.get_type()).cast<std::string>() << "\n";
		}

		py::eval_file("sample01.py", global, local);
	}
	catch (std::runtime_error e)
	{
		std::cout << "Python error.\n" << e.what() << "\n";
	}

	Py_Finalize();
	PyMem_RawFree(program);

	return 0;
}

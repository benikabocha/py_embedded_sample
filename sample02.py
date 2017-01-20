# -*- coding: utf-8 -*-

import sample02

print('sample02.add', sample02.add(1, 2))

dog = sample02.Dog()
print('sample02.call_go(dog):', sample02.call_go(dog))

class Cat(sample02.Animal):
    def go(self, n_times):
        return "ニャー! " * n_times

cat = Cat()
print('sample02.call_go(cat):', sample02.call_go(cat))

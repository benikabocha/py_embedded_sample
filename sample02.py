# -*- coding: utf-8 -*-

import sample02

# 関数の呼び出しをテスト
print('sample02.add(1, 2):', sample02.add(1, 2))

# sample02 で作成したクラスをテスト
dog = sample02.Dog()
print('sample02.call_go(dog):', sample02.call_go(dog))

# sample02.Animal を継承するテスト
class Cat(sample02.Animal):
    def go(self, n_times):
        return "ニャー! " * n_times

cat = Cat()
print('sample02.call_go(cat):', sample02.call_go(cat))

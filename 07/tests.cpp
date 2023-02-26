#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include <sstream>

#include "avl.hpp"

class TestFoo : public ::testing::Test
{
protected:
	void SetUp() {}
	void TearDown() {}
};

// Вносим в дерево числа от 0 до 9, и выводим содержимое дерева по порядку.
TEST(TestUtils, test_1)
{
	using Key = int;
	using T = int;
	using avl_t = avl<Key, T, std::less<Key>, CustomAllocator<std::pair<const Key, T>>>;
	auto a = avl_t();
	for (int i = 0; i < 10; i++) a.insert(i, i);
	std::stringstream ss;
	auto end_it = std::end(a);
	for (auto begin_it = std::begin(a); begin_it != end_it; ++begin_it)
	{
		ss << "{" << (*begin_it).key << ", " << (*begin_it).value << "}" << " ";
	}
	ASSERT_EQ(ss.str(), "{0, 0} {1, 1} {2, 2} {3, 3} {4, 4} {5, 5} {6, 6} {7, 7} {8, 8} {9, 9} ");
}

// Вносим числа от 0 до 9, затем удалаем по нечётным индексам и выводим дерево по порядку. Ищем значение 3 - находим по ключу 3. 
// Ищем значение 10 - не находим, возвращается nullptr. Число 7 содержится в дереве - contains возвращает true.
// Выводим числа в обратном порядке, пользуясь реверсивными итераторами
TEST(TestUtils, test_2)
{
	using Key = int;
	using T = int;
	using avl_t = avl<Key, T, std::less<Key>, CustomAllocator<std::pair<const Key, T>>>;
	auto a = avl_t();
	std::stringstream ss;
	for (int i = 0; i < 10; i++) a.insert(i, i);
	int x = 0;
	for (int i = 0; i < 10; i += 2) a.erase(i);
	x = 1;
	auto begin_it = std::begin(a);
	auto end_it = std::end(a);
	while (begin_it != end_it)
	{
		ss << "{" << (*begin_it).key << ", " << (*begin_it).value << "}" << std::endl;
		++begin_it;
	}
	auto it = a.find(3);
	ss << (it == nullptr ? "not found" : std::to_string((*it).key)) << std::endl;
	it = a.find(10);
	ss << (it == nullptr ? "not found" : std::to_string((*it).key)) << std::endl;
	std::cout << std::boolalpha << a.contains(7) << std::endl;
	auto rbegin_it = std::rbegin(a);
	auto rend_it = std::rend(a);
	while (rbegin_it != rend_it)
	{
		std::cout << "arr value = " << (*rbegin_it) << std::endl;
		++rbegin_it;
	}
	ASSERT_EQ(ss.str(), "{1, 1}\n{3, 3}\n{5, 5}\n{7, 7}\n{9, 9}\n3\nnot found\ntrue\n{9, 9}\n{7, 7}\n{5, 5}\n{3, 3}\n{1, 1}\n");
}


int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

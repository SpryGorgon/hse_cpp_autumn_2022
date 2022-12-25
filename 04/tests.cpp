#include <iostream>
#include <gtest/gtest.h>

#include "bigint.hpp"

class TestFoo : public ::testing::Test
{
protected:
	void SetUp() {}
	void TearDown() {}
};

// Конструкторы, умножение, сложение, вывод в поток
TEST(TestUtils, test_1)
{
	BigInt a = 1;
	BigInt b("123456789012345678901234567890");
	BigInt c = a * b + 2;
	std::stringstream ss;
	ss << c;
	ASSERT_EQ(ss.str(), "123456789012345678901234567892");
}

// Перемещающий конструктор
TEST(TestUtils, test_2)
{
	BigInt a = 1;
	BigInt b("123456789012345678901234567890");
	BigInt c = a * b + 2;
	BigInt d;
	d = std::move(c);
	std::stringstream ss;
	ss << d;
	ASSERT_EQ(ss.str(), "123456789012345678901234567892");
}

// Сложение после перемещающего конструктора
TEST(TestUtils, test_3)
{
	BigInt a = 1;
	BigInt b("123456789012345678901234567890");
	BigInt c = a * b + 2;
	BigInt d;
	d = std::move(c);
	a = d + b;
	std::stringstream ss;
	ss << a;
	ASSERT_EQ(ss.str(), "246913578024691357802469135782");
}

// Сложение
TEST(TestUtils, test_4)
{
	BigInt a = 99;
	BigInt b("123456789012345678901234567890");
	BigInt c = a + b;
	std::stringstream ss;
	ss << c;
	ASSERT_EQ(ss.str(), "123456789012345678901234567989");
}

// Сравнение чисел на равенство
TEST(TestUtils, test_5)
{
	BigInt a = 99;
	BigInt b("123456789012345678901234567890");
	BigInt c = a + b;
	BigInt d("123456789012345678901234567989");
	ASSERT_EQ(c == d, true);
}

// Вычитание с отрицательным рещультатом
TEST(TestUtils, test_6)
{
	BigInt a = 1;
	BigInt b("123456789012345678901234567890");
	BigInt c = a - b;
	std::stringstream ss;
	ss << c;
	ASSERT_EQ(ss.str(), "-123456789012345678901234567889");
}

// Вычитание с положительным результатом
TEST(TestUtils, test_7)
{
	BigInt a = 1;
	BigInt b("123456789012345678901234567890");
	BigInt c = b-a;
	std::stringstream ss;
	ss << c;
	ASSERT_EQ(ss.str(), "123456789012345678901234567889");
}

// Сравнение <
TEST(TestUtils, test_8)
{
	BigInt a = 1;
	BigInt b("123456789012345678901234567890");
	ASSERT_EQ(a < b, true);
}

// Сравнение <=
TEST(TestUtils, test_9)
{
	BigInt a = 1;
	BigInt b("123456789012345678901234567890");
	ASSERT_EQ(a <= b, true);
}

// Сравнение ==
TEST(TestUtils, test_10)
{
	BigInt a = 1;
	BigInt b("123456789012345678901234567890");
	ASSERT_EQ(a == b, false);
}

// Сравнение !=
TEST(TestUtils, test_11)
{
	BigInt a = 1;
	BigInt b("123456789012345678901234567890");
	ASSERT_EQ(a != b, true);
}

// Сравнение >=
TEST(TestUtils, test_12)
{
	BigInt a = 1;
	BigInt b("123456789012345678901234567890");
	ASSERT_EQ(a >= b, false);
}

// Сравнение >
TEST(TestUtils, test_13)
{
	BigInt a = 1;
	BigInt b("123456789012345678901234567890");
	ASSERT_EQ(a > b, false);
}

// Унарный минус
TEST(TestUtils, test_14)
{
	BigInt a("123456789012345678901234567890");
	std::stringstream ss;
	ss << -a;
	ASSERT_EQ(ss.str(), "-123456789012345678901234567890");
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

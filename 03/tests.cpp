#include <iostream>
#include <gtest/gtest.h>

#include "matrix.hpp"

class TestFoo : public ::testing::Test
{
protected:
	void SetUp() {}
	void TearDown() {}
};



// Нет токенов
TEST(TestUtils, test_1)
{
	Matrix m1(4, 1);
	Matrix m2(4, 6);

	ASSERT_EQ(m1 == m2, false);
}

// Число
TEST(TestUtils, test_2)
{
	Matrix m(2, 3);
	for (int i = 0; i < 6; i++) m[i / 3][i % 3] = (i + 1);
	ASSERT_EQ(m[0][2], 3);
}

// Не число, а строка, т.к. uint64_t не поддерживает отрицательные числа
TEST(TestUtils, test_3)
{
	Matrix m(2, 3);
	ASSERT_EQ(m.getRows(), 2);
}

// Строка
TEST(TestUtils, test_4)
{
	Matrix m(2, 3);
	ASSERT_EQ(m.getColumns(), 3);
}

// Число 0
TEST(TestUtils, test_5)
{
	Matrix m(2, 3, 1);
	m *= 3.14;
	ASSERT_EQ(m[0][0], 3);
}

// Число 1 с большим количеством незначащих нулей
TEST(TestUtils, test_6)
{
	Matrix m(2, 3, 1);
	std::stringstream ss;
	m *= 3.14;
	ss << m;
	ASSERT_EQ(ss.str(), "[[3, 3, 3],\n[3, 3, 3]]");
}

// Строка, т.к. не поместится в uint64_t
TEST(TestUtils, test_7)
{
	Matrix m1(5, 8, 1), m2(5, 8, 4), m3(5, 8, 5);
	ASSERT_EQ((m1+m2)==m3, true);
}


// Строка, т.к. в конце буква
TEST(TestUtils, test_8)
{
	Matrix m1(5, 8, 1), m2(5, 8, 4), m4(5, 8, 6);
	ASSERT_EQ((m1 + m2) != m4, true);
}

// Число - максимальное, помещающееся в uint64_t
TEST(TestUtils, test_9)
{
	Matrix m1(5, 8, 1), m2(5, 8, 4), m3(5, 8, -3);
	ASSERT_EQ((m1 - m2) == m3, true);
}

// Строка, т.к. число на 1 больше, чем максимальное для uint64_t
TEST(TestUtils, test_10)
{
	Matrix m1(5, 8, 1), m2(5, 8, 4);
	m1 *= 4.86;
	ASSERT_EQ(m1 == m2, true);
}

// Все токены из предыдущих тестов. Также вызывается callback в начале и в конце
TEST(TestUtils, test_11)
{
	Matrix m1(5, 8, 1), m2(5, 8, 4), m3(5, 8, -3);
	ASSERT_EQ((m1 - m2) == m3, true);
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#include <iostream>
#include <gtest/gtest.h>

#include "allocator.hpp"

class TestFoo : public ::testing::Test
{
protected:
    void SetUp() {}
    void TearDown() {}
};

// Есть 0 байт, выделяем 0 => nullptr
TEST(TestUtils, test_1)
{
	Allocator a;
	a.makeAllocator(0);
	char* ptr = a.alloc(0);

	ASSERT_EQ(ptr, nullptr);
}

// Есть 1 байт, выделяем 0 => nullptr
TEST(TestUtils, test_2)
{
	Allocator a;
	a.makeAllocator(1);
	char* ptr = a.alloc(0);

	ASSERT_EQ(ptr, nullptr);
}

// Есть 0 байт, выделяем 1 => nullptr
TEST(TestUtils, test_3)
{
	Allocator a;
	a.makeAllocator(0);
	char* ptr = a.alloc(1);

	ASSERT_EQ(ptr, nullptr);
}

// Есть 1 байт, выделяем 1 => не nullptr, записываем байт 't'
TEST(TestUtils, test_4)
{
	Allocator a;
	a.makeAllocator(1);
	char* ptr = a.alloc(1);
	*ptr = 't';

	ASSERT_EQ(*ptr, 't');
}

// Есть 100 байт, выделяем 50+30+30. Проверяем , что в 50 и 30 всё записывается, и ещё 30 не выделяются, т.к. нет места
TEST(TestUtils, test_5)
{
	Allocator a;
	a.makeAllocator(100);
	char* ptr = a.alloc(50);
	ASSERT_NE(ptr, nullptr);
	for (int i = 0; i < 50; i++) *(ptr + i) = ('a' + i);
	for (int i = 0; i < 50; i++) ASSERT_EQ(*(ptr + i), (char)('a' + i));
	ptr = a.alloc(30);
	ASSERT_NE(ptr, nullptr);
	for(int i=0;i<30;i++) *(ptr + i) = ('0' + i);
	for (int i = 0; i < 30; i++) ASSERT_EQ(*(ptr + i), (char)('0' + i));
	ptr = a.alloc(30);
	ASSERT_EQ(ptr, nullptr);
}

// Тест такой же, как предыдущий, но перед выделением лишних 30 байт происходит reset => не nullptr
TEST(TestUtils, test_6)
{
	Allocator a;
	a.makeAllocator(100);
	char* ptr = a.alloc(50);
	ASSERT_NE(ptr, nullptr);
	for (int i = 0; i < 50; i++) *(ptr + i) = ('a' + i);
	for (int i = 0; i < 50; i++) ASSERT_EQ(*(ptr + i), (char)('a' + i));
	ptr = a.alloc(30);
	ASSERT_NE(ptr, nullptr);
	for (int i = 0; i < 30; i++) *(ptr + i) = ('0' + i);
	for (int i = 0; i < 30; i++) ASSERT_EQ(*(ptr + i), (char)('0' + i));
	a.reset();
	ptr = a.alloc(30);
	ASSERT_NE(ptr, nullptr);
}

// Есть 10 байт, выделяем 9 + 0 + 1 => не nullptr, nullptr, не nullptr
TEST(TestUtils, test_7)
{
	Allocator a;
	a.makeAllocator(10);
	char* ptr = a.alloc(9);
	ASSERT_NE(ptr, nullptr);
	for (int i = 0; i < 9; i++) *(ptr + i) = 'l';
	for (int i = 0; i < 9; i++) ASSERT_EQ(*(ptr + i), 'l');
	ptr = a.alloc(0);
	ASSERT_EQ(ptr, nullptr);
	ptr = a.alloc(1);
	ASSERT_NE(ptr, nullptr);
}

// Есть 10 байт, выделяем 10 + 0 + 1 => не nullptr, nullptr, nullptr
TEST(TestUtils, test_8)
{
	Allocator a;
	a.makeAllocator(10);
	char* ptr = a.alloc(10);
	ASSERT_NE(ptr, nullptr);
	for (int i = 0; i < 10; i++) *(ptr + i) = 'k';
	for (int i = 0; i < 10; i++) ASSERT_EQ(*(ptr + i), 'k');
	ptr = a.alloc(0);
	ASSERT_EQ(ptr, nullptr);
	ptr = a.alloc(1);
	ASSERT_EQ(ptr, nullptr);
}

// Есть 10 байт, выделяем 11, после reset 10 + 0 + 1 => nullptr, не nullptr, nullptr, nullptr
TEST(TestUtils, test_9)
{
	Allocator a;
	a.makeAllocator(10);
	char* ptr = a.alloc(11);
	ASSERT_EQ(ptr, nullptr);
	ptr = a.alloc(10);
	ASSERT_NE(ptr, nullptr);
	for (int i = 0; i < 10; i++) *(ptr + i) = 'k';
	for (int i = 0; i < 10; i++) ASSERT_EQ(*(ptr + i), 'k');
	ptr = a.alloc(0);
	ASSERT_EQ(ptr, nullptr);
	ptr = a.alloc(1);
	ASSERT_EQ(ptr, nullptr);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

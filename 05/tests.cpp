#include <iostream>
#include <sstream>
#include <gtest/gtest.h>

#include "serealizer.hpp"

class TestFoo : public ::testing::Test
{
protected:
	void SetUp() {}
	void TearDown() {}
};

// Тест из условия задачи
TEST(TestUtils, test_1)
{
	Data x{ 1, true, 2 };

	std::stringstream stream;

	Serializer serializer(stream);
	serializer.save(x);

	Data y{ 0, false, 0 };
	Deserializer deserializer(stream);

	const Error err = deserializer.load(y);

	ASSERT_EQ(err, Error::NoError);
	ASSERT_EQ(x.a, y.a);
	ASSERT_EQ(x.b, y.b);
	ASSERT_EQ(x.c, y.c);
}

// Тест, похожий на предыдущий, но с другими данными
TEST(TestUtils, test_2)
{
	Data x{ 18231853722, false, 999999999999 };

	std::stringstream stream;

	Serializer serializer(stream);
	serializer.save(x);

	Data y{ 0, false, 0 };
	Deserializer deserializer(stream);

	const Error err = deserializer.load(y);

	ASSERT_EQ(err, Error::NoError);
	ASSERT_EQ(x.a, y.a);
	ASSERT_EQ(x.b, y.b);
	ASSERT_EQ(x.c, y.c);
}

//Тест serealizer-а
TEST(TestUtils, test_3)
{
	Data x{ 1, true, 2 };

	std::stringstream stream;

	Serializer serializer(stream);
	serializer.save(x);

	ASSERT_EQ(stream.str(), "1 true 2 ");
}

// Тест deserializer-а
TEST(TestUtils, test_4)
{
	std::stringstream stream("1 true 2 ");
	Data y{ 0, false, 0 };
	Deserializer deserializer(stream);

	const Error err = deserializer.load(y);

	ASSERT_EQ(err, Error::NoError);
	ASSERT_EQ(1, y.a);
	ASSERT_EQ(true, y.b);
	ASSERT_EQ(2, y.c);
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

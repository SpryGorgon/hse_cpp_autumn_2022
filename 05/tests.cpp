#include <iostream>
#include <assert.h>
#include <sstream>
#include <gtest/gtest.h>

#include "serealizer.hpp"

class TestFoo : public ::testing::Test
{
protected:
	void SetUp() {}
	void TearDown() {}
};

TEST(TestUtils, test_1)
{
	Data x{ 1, true, 2 };

	std::stringstream stream;

	Serializer serializer(stream);
	serializer.save(x);

	Data y{ 0, false, 0 };
	std::cout << stream.str() << std::endl;
	Deserializer deserializer(stream);

	const Error err = deserializer.load(y);

	assert(err == Error::NoError);

	assert(x.a == y.a);
	assert(x.b == y.b);
	assert(x.c == y.c);
}

TEST(TestUtils, test_2)
{
	Data x{ 18231853722, false, 999999999999 };

	std::stringstream stream;

	Serializer serializer(stream);
	serializer.save(x);

	Data y{ 0, false, 0 };
	std::cout << stream.str() << std::endl;
	Deserializer deserializer(stream);

	const Error err = deserializer.load(y);

	assert(err == Error::NoError);

	assert(x.a == y.a);
	assert(x.b == y.b);
	assert(x.c == y.c);
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

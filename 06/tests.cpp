#include <iostream>
#include <string>
#include <gtest/gtest.h>

#include "format.hpp"

class TestFoo : public ::testing::Test
{
protected:
	void SetUp() {}
	void TearDown() {}
};

// Тест из условия задачи
TEST(TestUtils, test_1)
{
	std::string text = format("{1}+{1} = {0}", 2, "one");
	ASSERT_EQ(text, "one+one = 2");
}

// 
TEST(TestUtils, test_2)
{
	std::string text = format("{3} is {2} than {0} and {1}, {1}=={1}, and {1}>{0}", 44, 99, "smaller", '7');
	ASSERT_EQ(text, "7 is smaller than 44 and 99, 99==99, and 99>44");
}

// 
TEST(TestUtils, test_3)
{
	try
	{
		std::string text = format("{}");
	}
	catch (const std::invalid_argument& e)
	{
		ASSERT_EQ(std::string(e.what()), "{} is not a number");
		return;
	}
	catch (...) {}
	ASSERT_EQ(true, false);
}

// 
TEST(TestUtils, test_4)
{
	try
	{
		std::string text = format("{dhcbwhbc}", 9);
	}
	catch (const std::invalid_argument& e)
	{
		ASSERT_EQ(std::string(e.what()), "{dhcbwhbc} is not a number");
		return;
	}
	catch (...) {}
	ASSERT_EQ(true, false);
}

// 
TEST(TestUtils, test_5)
{
	try
	{
		std::string text = format("{{");
	}
	catch (const std::invalid_argument& e)
	{
		ASSERT_EQ(std::string(e.what()), "Invalid string format");
		return;
	}
	catch (...) {}
	ASSERT_EQ(true, false);
}

// 
TEST(TestUtils, test_6)
{
	try
	{
		std::string text = format("{{}}");
	}
	catch (const std::invalid_argument& e)
	{
		ASSERT_EQ(std::string(e.what()), "Invalid string format");
		return;
	}
	catch (...) {}
	ASSERT_EQ(true, false);
}

// 
TEST(TestUtils, test_7)
{
	try
	{
		std::string text = format("{{0}}", 0);
	}
	catch (const std::invalid_argument& e)
	{
		ASSERT_EQ(std::string(e.what()), "Invalid string format");
		return;
	}
	catch (...) {}
	ASSERT_EQ(true, false);
}

// 
TEST(TestUtils, test_8)
{
	try
	{
		std::string text = format("{{0}", 0);
	}
	catch (const std::invalid_argument& e)
	{
		ASSERT_EQ(std::string(e.what()), "Invalid string format");
		return;
	}
	catch (...) {}
	ASSERT_EQ(true, false);
}

// 
TEST(TestUtils, test_9)
{
	try
	{
		std::string text = format("{0} args", 2, 2);
	}
	catch (const std::invalid_argument& e)
	{
		ASSERT_EQ(std::string(e.what()), "1 arguments expected, but more found");
		return;
	}
	catch (...) {}
	ASSERT_EQ(true, false);
}

// 
TEST(TestUtils, test_10)
{
	try
	{
		std::string text = format("{0} args: {0} and {1}", 2);
	}
	catch (const std::invalid_argument& e)
	{
		ASSERT_EQ(std::string(e.what()), "2 arguments expected, but less found");
		return;
	}
	catch (...) {}
	ASSERT_EQ(true, false);
}

// 
TEST(TestUtils, test_11)
{
	try
	{
		std::string text = format("{0} args: {0} and {2}", 2, 3);
	}
	catch (const std::invalid_argument& e)
	{
		ASSERT_EQ(std::string(e.what()), "3 arguments expected, but argument #1 not found");
		return;
	}
	catch (...) {}
	ASSERT_EQ(true, false);
}

// 
TEST(TestUtils, test_12)
{
	std::string text = format("{0} args: {0} and {1}", 2, 3);
	ASSERT_EQ(text, "2 args: 2 and 3");
}


int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

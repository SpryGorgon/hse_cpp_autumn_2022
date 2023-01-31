#include <iostream>
#include <gtest/gtest.h>

#include "parser.hpp"

class TestFoo : public ::testing::Test
{
protected:
	void SetUp() {}
	void TearDown() {}
};

using T = void;
template class TokenParser<T>;

std::string parsed_line = "";

T dc(const uint64_t&)
{
	parsed_line += "n";
	return;
}
T sc(const std::string&)
{
	parsed_line += "s";
	return;
}

T startCallback()
{
	parsed_line += "start\n";
	return;
}

T endCallback()
{
	parsed_line += "\nend";
	return;
}

// Нет токенов
TEST(TestUtils, test_1)
{
	TokenParser<T> parser;
	parsed_line = "";
	parser.SetDigitTokenCallback(dc);
	parser.SetStringTokenCallback(sc);
	std::string line = "";
	parser.Parse(line);
	ASSERT_EQ(parsed_line, "");
}

// Пробел
TEST(TestUtils, test_2)
{
	TokenParser<T> parser;
	parsed_line = "";
	parser.SetDigitTokenCallback(dc);
	parser.SetStringTokenCallback(sc);
	std::string line = " ";
	parser.Parse(line);
	ASSERT_EQ(parsed_line, "");
}

// Пробельные символы
TEST(TestUtils, test_3)
{
	TokenParser<T> parser;
	parsed_line = "";
	parser.SetDigitTokenCallback(dc);
	parser.SetStringTokenCallback(sc);
	std::string line = " \t\n\r\t\n \t \r";
	parser.Parse(line);
	ASSERT_EQ(parsed_line, "");
}

// Число
TEST(TestUtils, test_4)
{
	TokenParser<T> parser;
	parsed_line = "";
	parser.SetDigitTokenCallback(dc);
	parser.SetStringTokenCallback(sc);
	std::string line = "1";
	parser.Parse(line);
	ASSERT_EQ(parsed_line, "n");
}

// Число, но без соответствующего callback-а
TEST(TestUtils, test_5)
{
	TokenParser<T> parser;
	parsed_line = "";
	parser.SetDigitTokenCallback(nullptr);
	parser.SetStringTokenCallback(sc);
	std::string line = "1";
	parser.Parse(line);
	ASSERT_EQ(parsed_line, "");
}

// Не число, а строка, т.к. uint64_t не поддерживает отрицательные числа
TEST(TestUtils, test_6)
{
	TokenParser<T> parser;
	parsed_line = "";
	parser.SetDigitTokenCallback(dc);
	parser.SetStringTokenCallback(sc);
	std::string line = "-1";
	parser.Parse(line);
	ASSERT_EQ(parsed_line, "s");
}

// Строка
TEST(TestUtils, test_7)
{
	TokenParser<T> parser;
	parsed_line = "";
	parser.SetDigitTokenCallback(dc);
	parser.SetStringTokenCallback(sc);
	std::string line = "a";
	parser.Parse(line);
	ASSERT_EQ(parsed_line, "s");
}

// Строка, но без соответствующего callback-а
TEST(TestUtils, test_8)
{
	TokenParser<T> parser;
	parsed_line = "";
	parser.SetDigitTokenCallback(dc);
	parser.SetStringTokenCallback(nullptr);
	std::string line = "a";
	parser.Parse(line);
	ASSERT_EQ(parsed_line, "");
}

// Число 0
TEST(TestUtils, test_9)
{
	TokenParser<T> parser;
	parsed_line = "";
	parser.SetDigitTokenCallback(dc);
	parser.SetStringTokenCallback(sc);
	std::string line = "000000000000000000000000000000000000000000000000000000000000000000000000000000000";
	parser.Parse(line);
	ASSERT_EQ(parsed_line, "n");
}

// Число 1 с большим количеством незначащих нулей
TEST(TestUtils, test_10)
{
	TokenParser<T> parser;
	parsed_line = "";
	parser.SetDigitTokenCallback(dc);
	parser.SetStringTokenCallback(sc);
	std::string line = "00000000000000000000000000000000000000000000000000000000000000001";
	parser.Parse(line);
	ASSERT_EQ(parsed_line, "n");
}

// Строка, т.к. не поместится в uint64_t
TEST(TestUtils, test_11)
{
	TokenParser<T> parser;
	parsed_line = "";
	parser.SetDigitTokenCallback(dc);
	parser.SetStringTokenCallback(sc);
	std::string line = "000000010000000000000000000000000000000000000000000000000000000000000000000000000";
	parser.Parse(line);
	ASSERT_EQ(parsed_line, "s");
}


// Строка, т.к. в конце буква
TEST(TestUtils, test_12)
{
	TokenParser<T> parser;
	parsed_line = "";
	parser.SetDigitTokenCallback(dc);
	parser.SetStringTokenCallback(sc);
	std::string line = "32793274627292r";
	parser.Parse(line);
	ASSERT_EQ(parsed_line, "s");
}

// Число - максимальное, помещающееся в uint64_t
TEST(TestUtils, test_13)
{
	TokenParser<T> parser;
	parsed_line = "";
	parser.SetDigitTokenCallback(dc);
	parser.SetStringTokenCallback(sc);
	std::string line = "18446744073709551615";
	parser.Parse(line);
	ASSERT_EQ(parsed_line, "n");
}

// Строка, т.к. число на 1 больше, чем максимальное для uint64_t
TEST(TestUtils, test_14)
{
	TokenParser<T> parser;
	parsed_line = "";
	parser.SetDigitTokenCallback(dc);
	parser.SetStringTokenCallback(sc);
	std::string line = "18446744073709551616";
	parser.Parse(line);
	ASSERT_EQ(parsed_line, "s");
}

// Все токены из предыдущих тестов. Также вызывается callback в начале и в конце
TEST(TestUtils, test_15)
{
	TokenParser<T> parser;
	parsed_line = "";
	parser.SetDigitTokenCallback(dc);
	parser.SetStringTokenCallback(sc);
	parser.SetStartCallback(startCallback);
	parser.SetEndCallback(endCallback);
	std::string line = "1 -1 a 000000000000000000000000000000000000000000000000000000000000000000000000000000000\n";
	line += "00000000000000000000000000000000000000000000000000000000000000001\t";
	line += "000000010000000000000000000000000000000000000000000000000000000000000000000000000\r";
	line += "32793274627292r 18446744073709551615 18446744073709551616";
	parser.Parse(line);
	ASSERT_EQ(parsed_line, "start\nnssnnssns\nend");
}

// Все токены из предыдущих тестов, но все callback-и nullptr
TEST(TestUtils, test_16)
{
	TokenParser<T> parser;
	parsed_line = "";
	parser.SetDigitTokenCallback(nullptr);
	parser.SetStringTokenCallback(nullptr);
	parser.SetStartCallback(nullptr);
	parser.SetEndCallback(nullptr);
	std::string line = "1 -1 a 000000000000000000000000000000000000000000000000000000000000000000000000000000000\n";
	line += "00000000000000000000000000000000000000000000000000000000000000001\t";
	line += "000000010000000000000000000000000000000000000000000000000000000000000000000000000\r";
	line += "32793274627292r 18446744073709551615 18446744073709551616";
	parser.Parse(line);
	ASSERT_EQ(parsed_line, "");
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#include <iostream>
#include <gtest/gtest.h>

#include "parser.hpp"

class TestFoo : public ::testing::Test
{
protected:
	void SetUp() {}
	void TearDown() {}
};

using T = std::string;
template class TokenParser<std::string>;

T dc(const uint64_t& n)
{
	(void)n;
	return "n";
}
T sc(const std::string& s)
{
	(void)s;
	return "s";
}

T startCallback()
{
	return "start\n";
}

T endCallback()
{
	return "\nend";
}

// ��� �������
TEST(TestUtils, test_1)
{
	TokenParser<T> parser;
	parser.SetDigitTokenCallback(dc);
	parser.SetStringTokenCallback(sc);
	std::string line = "";
	T ans = parser.Parse(line);
	ASSERT_EQ(ans, "");
}

// �����
TEST(TestUtils, test_2)
{
	TokenParser<T> parser;
	parser.SetDigitTokenCallback(dc);
	parser.SetStringTokenCallback(sc);
	std::string line = "1";
	T ans = parser.Parse(line);
	ASSERT_EQ(ans, "n");
}

// �� �����, � ������, �.�. uint64_t �� ������������ ������������� �����
TEST(TestUtils, test_3)
{
	TokenParser<T> parser;
	parser.SetDigitTokenCallback(dc);
	parser.SetStringTokenCallback(sc);
	std::string line = "-1";
	T ans = parser.Parse(line);
	ASSERT_EQ(ans, "s");
}

// ������
TEST(TestUtils, test_4)
{
	TokenParser<T> parser;
	parser.SetDigitTokenCallback(dc);
	parser.SetStringTokenCallback(sc);
	std::string line = "a";
	T ans = parser.Parse(line);
	ASSERT_EQ(ans, "s");
}

// ����� 0
TEST(TestUtils, test_5)
{
	TokenParser<T> parser;
	parser.SetDigitTokenCallback(dc);
	parser.SetStringTokenCallback(sc);
	std::string line = "000000000000000000000000000000000000000000000000000000000000000000000000000000000";
	T ans = parser.Parse(line);
	ASSERT_EQ(ans, "n");
}

// ����� 1 � ������� ����������� ���������� �����
TEST(TestUtils, test_6)
{
	TokenParser<T> parser;
	parser.SetDigitTokenCallback(dc);
	parser.SetStringTokenCallback(sc);
	std::string line = "00000000000000000000000000000000000000000000000000000000000000001";
	T ans = parser.Parse(line);
	ASSERT_EQ(ans, "n");
}

// ������, �.�. �� ���������� � uint64_t
TEST(TestUtils, test_7)
{
	TokenParser<T> parser;
	parser.SetDigitTokenCallback(dc);
	parser.SetStringTokenCallback(sc);
	std::string line = "000000010000000000000000000000000000000000000000000000000000000000000000000000000";
	T ans = parser.Parse(line);
	ASSERT_EQ(ans, "s");
}


// ������, �.�. � ����� �����
TEST(TestUtils, test_8)
{
	TokenParser<T> parser;
	parser.SetDigitTokenCallback(dc);
	parser.SetStringTokenCallback(sc);
	std::string line = "32793274627292r";
	T ans = parser.Parse(line);
	ASSERT_EQ(ans, "s");
}

// ����� - ������������, ������������ � uint64_t
TEST(TestUtils, test_9)
{
	TokenParser<T> parser;
	parser.SetDigitTokenCallback(dc);
	parser.SetStringTokenCallback(sc);
	std::string line = "18446744073709551615";
	T ans = parser.Parse(line);
	ASSERT_EQ(ans, "n");
}

// ������, �.�. ����� �� 1 ������, ��� ������������ ��� uint64_t
TEST(TestUtils, test_10)
{
	TokenParser<T> parser;
	parser.SetDigitTokenCallback(dc);
	parser.SetStringTokenCallback(sc);
	std::string line = "18446744073709551616";
	T ans = parser.Parse(line);
	ASSERT_EQ(ans, "s");
}

// ��� ������ �� ���������� ������. ����� ���������� callback � ������ � � �����
TEST(TestUtils, test_11)
{
	TokenParser<T> parser;
	parser.SetDigitTokenCallback(dc);
	parser.SetStringTokenCallback(sc);
	parser.SetStartCallback(startCallback);
	parser.SetEndCallback(endCallback);
	std::string line = "1 -1 a 000000000000000000000000000000000000000000000000000000000000000000000000000000000\n";
	line += "00000000000000000000000000000000000000000000000000000000000000001\t";
	line += "000000010000000000000000000000000000000000000000000000000000000000000000000000000\r";
	line += "32793274627292r 18446744073709551615 18446744073709551616";
	T ans = parser.Parse(line);
	ASSERT_EQ(ans, "start\nnssnnssns\nend");
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

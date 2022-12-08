#include <iostream>
#include <sstream>
#include "parser.hpp"

const std::string mx = "18446744073709551615";

inline bool is_number(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

template<typename T>
TokenParser<T>::TokenParser()
{
	startCallback = nullptr;
	endCallback = nullptr;
	digitCallback = nullptr;
	stringCallback = nullptr;
}

// Устанавливаем callback-функцию перед стартом парсинга.
template<typename T>
void TokenParser<T>::SetStartCallback(NoArgPtr<T> f)
{
	startCallback = f;
}

// Устанавливаем callback-функцию после окончания парсинга.
template<typename T>
void TokenParser<T>::SetEndCallback(NoArgPtr<T> f)
{
	endCallback = f;
}

// Устанавливаем callback-функцию для обработки чисел.
template<typename T>
void TokenParser<T>::SetDigitTokenCallback(FooPtr<std::uint64_t,T> f)
{
	digitCallback = f;
}

// Устанавливаем callback-функцию для обработки строк
template<typename T>
void TokenParser<T>::SetStringTokenCallback(FooPtr<std::string,T> f)
{
	stringCallback = f;
}

// Тут другие методы для установки callback-функций.
//...

template<typename T>
T TokenParser<T>::Parse(const std::string& tokens)
{
	T ans = T();
	if (startCallback != nullptr) ans += startCallback();
	std::istringstream iss(tokens);
	std::string s_;
	while (iss >> s_)
	{
		try
		{
			if (s_.size() == 0) throw std::runtime_error("string is empty");
			auto ind = s_.find_first_not_of(" \t\n\r0");
			std::string s = s_.substr(ind == std::string::npos ? (s_.size() - 1) : ind);
			if (s.size() > mx.size() || (s.size() == mx.size() && s > mx) || !is_number(s)) throw std::runtime_error("overflow");
			ind = s.find_first_not_of(" \t\n\r");
			uint64_t n;
			std::istringstream iss(s);
			iss >> n;
			if (ind != std::string::npos && s[ind] == '-') throw std::runtime_error("negative value");
			if (digitCallback != nullptr) ans += digitCallback(n);
		}
		catch (...)
		{
			if (stringCallback != nullptr) ans += stringCallback(s_);
		}
	}
	if (endCallback != nullptr) ans += endCallback();
	return ans;
}

template class TokenParser<std::string>;
#pragma once
#include <cstdint>


template<typename T, typename CallbackType>
using FooPtr = CallbackType(*)(const T&);
template<typename CallbackType>
using NoArgPtr = CallbackType(*)();

template<typename T>
class TokenParser
{
private:
    NoArgPtr<T> startCallback, endCallback;
    FooPtr<std::uint64_t,T> digitCallback;
    FooPtr<std::string,T> stringCallback;
public:
    TokenParser();// = default;

    // Устанавливаем callback-функцию перед стартом парсинга.
    void SetStartCallback(NoArgPtr<T> f);

    // Устанавливаем callback-функцию после окончания парсинга.
    void SetEndCallback(NoArgPtr<T> f);

    // Устанавливаем callback-функцию для обработки чисел.
    void SetDigitTokenCallback(FooPtr<std::uint64_t,T> f);

    // Устанавливаем callback-функцию для обработки строк
    void SetStringTokenCallback(FooPtr<std::string,T> f);

    // Тут другие методы для установки callback-функций.
    //...
    void Parse(const std::string&);
};
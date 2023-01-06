// serializer.h
#pragma once
#include <iostream>
#include <string>
#include <type_traits>
#include <cstdint>

enum class Error
{
	NoError,
	CorruptedArchive
};

class Serializer
{
	static constexpr char Separator = '\n';
	std::ostream& out_;
public:
	explicit Serializer(std::ostream& out);
	template <class T>
	Error save(T& object);
	template <class... ArgsT>
	Error operator()(ArgsT... args);
	Error process(bool&& arg);
	Error process(uint64_t&& arg);
private:
	Error process();
	// process использует variadic templates
	template<class T, class... Args>
	Error process(T&& val, Args&&... args);
	Error process_(bool&& arg);
	Error process_(uint64_t&& arg);
};

class Deserializer
{
	std::istream& in_;
public:
	explicit Deserializer(std::istream& in);
	template <class T>
	Error load(T& object);
	template <class... ArgsT>
	Error operator()(ArgsT&... args);
	template<bool>
	Error process(bool& arg);
	template<uint64_t>
	Error process(uint64_t& arg);
private:
	const std::string mx = std::to_string(UINT64_MAX);
	Error process();
	// process использует variadic templates
	template<class T, class... Args>
	Error process(T&& val, Args&&... args);
	Error process_(bool& arg);
	inline bool is_number(const std::string& s);
	Error process_(uint64_t& arg);
};

struct Data
{
	uint64_t a;
	bool b;
	uint64_t c;

	template <class Serializer>
	Error serialize(Serializer& serializer);

	template <class Deserializer>
	Error deserialize(Deserializer& deserializer);
};
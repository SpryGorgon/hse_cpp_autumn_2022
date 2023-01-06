#include <assert.h>
#include <sstream>
#include "serealizer.hpp"


Serializer::Serializer(std::ostream& out)
	: out_(out) {}

template <class T>
Error Serializer::save(T& object)
{
	return object.serialize(*this);
}

template <class... ArgsT>
Error Serializer::operator()(ArgsT... args)
{
	return process(args...);
}
Error Serializer::process(bool&& arg)
{
	return process_(std::move(arg));
}
Error Serializer::process(uint64_t&& arg)
{
	return process_(std::move(arg));
}

Error Serializer::process()
{
	return Error::NoError;
}
// process использует variadic templates
template<class T, class... Args>
Error Serializer::process(T&& val, Args&&... args)
{
	auto ans = process_(std::move(val));
	if (ans != Error::NoError) return ans;
	return process(std::forward<Args>(args)...);
}

Error Serializer::process_(bool&& arg)
{
	out_ << std::boolalpha << arg << Separator;
	return Error::NoError;
}
Error Serializer::process_(uint64_t&& arg)
{
	out_ << arg << Separator;
	return Error::NoError;
}


Deserializer::Deserializer(std::istream& in)
	: in_(in) {}

template <class T>
Error Deserializer::load(T& object)
{
	return object.deserialize(*this);
}

template <class... ArgsT>
Error Deserializer::operator()(ArgsT&... args)
{
	return process(args...);
}
template<bool>
Error Deserializer::process(bool& arg)
{
	return process_(arg);
}
template<uint64_t>
Error Deserializer::process(uint64_t& arg)
{
	return process_(arg);
}

const std::string mx = std::to_string(UINT64_MAX);
Error Deserializer::process()
{
	return Error::NoError;
}
// process использует variadic templates
template<class T, class... Args>
Error Deserializer::process(T&& val, Args&&... args)
{
	process_(val);
	return process(std::forward<Args>(args)...);
}

Error Deserializer::process_(bool& arg)
{
	std::string s;
	in_ >> s;
	if (s == "true") arg = true;
	else if (s == "false") arg = false;
	else return Error::CorruptedArchive;
	return Error::NoError;
}
inline bool Deserializer::is_number(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}
Error Deserializer::process_(uint64_t& arg)
{
	std::string s;
	in_ >> s;
	if (s.size() > mx.size() || (s.size() == mx.size() && s > mx) || !is_number(s)) return Error::CorruptedArchive;
	std::istringstream iss(s);
	iss >> arg;
	return Error::NoError;
}

template 
Error Serializer::save(Data&);
template 
Error Deserializer::load(Data&);


template <class Serializer>
Error Data::serialize(Serializer& serializer)
{
	return serializer(a, b, c);
}

template <class Deserializer>
Error Data::deserialize(Deserializer& deserializer)
{
	return deserializer(a, b, c);
}
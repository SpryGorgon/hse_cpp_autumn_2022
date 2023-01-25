#include "format.hpp"
#include <sstream>

bool is_number(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

bool replace_all(std::string& s, std::string&& what, std::string&& with)
{
	bool found = false;
	for (size_t i = s.find(what); i != s.npos; i = s.find(what, i + with.size()))
	{
		found = true;
		s = s.replace(i, what.size(), with);
	}
	return found;
}

void check_format(const std::string& f, int& n_args)
{
	int balance = 0;
	std::string token = "";
	for (char c : f)
	{
		if (c == '}')
		{
			balance--;
			if (!is_number(token)) throw std::invalid_argument("{" + token + "} is not a number");
			int n_arg = std::stoi(token) + 1;
			token = "";
			n_args = std::max(n_args, n_arg);
		}
		if (balance == 1) token += c;
		if (c == '{') balance++;
		if (balance < 0 || balance>1) throw std::invalid_argument("Invalid string format");
	}
	if (balance != 0) throw std::invalid_argument("Invalid string format");
}

void format_(std::string& f, int depth, int n_args)
{
	check_format(f, n_args);
	if (depth != n_args) throw std::invalid_argument(std::to_string(n_args) + " arguments expected, but less found");
}

std::string format(std::string f)
{
	format_(f, 0, -1);
	return f;
}
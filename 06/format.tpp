#pragma once
#include <iostream>
#include <string>
#include <sstream>

template<class T, class... Args>
void format_(std::string& f, int depth, int n_args, T&& val, Args&&... args)
{
	if (depth == 0) check_format(f, n_args);
	else if (depth == n_args) throw std::invalid_argument(std::to_string(n_args) + " arguments expected, but more found");

	std::stringstream tmp = std::stringstream();
	tmp << val;
	bool found = replace_all(f, "{" + std::to_string(depth) + "}", tmp.str());
	if (!found) throw std::invalid_argument(std::to_string(n_args) + " arguments expected, but argument #" + std::to_string(depth) + " not found");
	format_(f, depth + 1, n_args, std::forward<Args>(args)...);
}

template<class T, class... Args>
std::string format(std::string f, T&& val, Args&&... args)
{
	format_(f, 0, -1, val, args...);
	return f;
}
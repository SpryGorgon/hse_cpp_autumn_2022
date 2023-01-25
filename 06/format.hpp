#pragma once
#include <iostream>
#include <string>
#include <sstream>

bool is_number(const std::string& s);

bool replace_all(std::string& s, std::string&& what, std::string&& with);

void check_format(const std::string& f, int& n_args);

void format_(std::string& f, int depth, int n_args);

std::string format(std::string f);

#include "format.tpp"
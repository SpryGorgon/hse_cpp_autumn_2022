#include <iostream>
#include <exception>
#include <assert.h>
#include "bigint.hpp"

void BigInt::destroy()
{
	delete[] data;
}
BigInt BigInt::add(const BigInt& a, const BigInt& b)
{
	std::string ans = "";
	bool hold = false;
	size_t i = 0, j = 0;
	while (i < a.size && j < b.size)
	{
		int32_t x = a.data[i] + b.data[j] + (hold ? 1 : 0);
		hold = x / 100;
		ans = std::to_string(x % 100) + ans;
		i++; j++;
	}
	for (; i < a.size; i++)
	{
		int32_t x = a.data[i] + (hold ? 1 : 0);
		hold = x / 100;
		ans = std::to_string(x % 100) + ans;
	}
	for (; j < b.size; j++)
	{
		int32_t x = b.data[j] + (hold ? 1 : 0);
		hold = x / 100;
		ans = std::to_string(x % 100) + ans;
	}
	if (hold) ans = "1" + ans;
	return BigInt(ans);
}
BigInt BigInt::subs(const BigInt& a, const BigInt& b)
{
	std::string ans = "";
	bool hold = false;
	size_t i = 0, j = 0;
	while (i < a.size && j < b.size)
	{
		int32_t x = a.data[i] - b.data[j] - (hold ? 1 : 0);
		hold = (x < 0 ? true : false);
		ans = std::to_string((x + 100) % 100) + ans;
		i++; j++;
	}
	for (; i < a.size; i++)
	{
		int32_t x = a.data[i] - (hold ? 1 : 0);
		hold = (x < 0 ? true : false);
		ans = std::to_string((x + 100) % 100) + ans;
	}
	for (; j < b.size; j++)
	{
		int32_t x = b.data[j] - (hold ? 1 : 0);
		hold = (x < 0 ? true : false);
		ans = std::to_string((x + 100) % 100) + ans;
	}
	if (hold) throw std::runtime_error("abs(a) < abs(b)");
	return BigInt(ans);
}
BigInt BigInt::mul(const BigInt& a, const int32_t b, size_t n_zeros)
{
	std::string ans = "";
	int32_t hold = 0;
	size_t i = 0;
	while (i < a.size)
	{
		int32_t x = a.data[i] + hold;
		x *= b;
		hold = x / 100;
		ans = std::to_string(x % 100) + ans;
		i++;
	}
	if (hold) ans = std::to_string(hold) + ans;
	for (size_t i = 0; i < n_zeros; i++) ans += "0";
	return BigInt(ans);
}
bool BigInt::dataEquals(const BigInt& a, const BigInt& b)
{
	if (a.size != b.size) return false;
	for (size_t i = a.size - 1;; i--)
	{
		if (a.data[i] != b.data[i]) return false;
		if (i == 0) break;
	}
	return true;
}
bool BigInt::dataLower(const BigInt& a, const BigInt& b)
{
	if (a.size < b.size) return true;
	if (a.size == b.size)
	{
		for (int32_t i = a.size - 1; i >= 0; i--) if (a.data[i] != b.data[i]) return a.data[i] < b.data[i];
	}
	return false;
}
BigInt::BigInt() : size(1), capacity(1), data(new int32_t[1]), neg(false) { data[0] = 0; }
BigInt::BigInt(int32_t n)
{
	size = (std::to_string(abs(n)).size() + 1) / 2;
	capacity = size;
	data = new int32_t[capacity];
	data[0] = 0;
	neg = ((n < 0) ? true : false);
	size_t i = 0;
	n = abs(n);
	while (n > 0)
	{
		data[i] = n % 100;
		n /= 100;
		i++;
	}
}
BigInt::BigInt(const std::string& s)
{
	if (s.size() > 0 && s[0] == '-') size = s.size() / 2;
	else size = (s.size() + 1) / 2;
	capacity = size;
	data = new int32_t[capacity];
	data[0] = 0;

	if (s.size() > 0 && s[0] == '-')
	{
		neg = true;
		for (size_t i = 1; i <= size; i++)
		{
			data[size - i] = std::stoi(s.substr(i * 2 - 1, 2));
		}
	}
	else
	{
		neg = false;
		for (size_t i = 0; i < size; i++)
		{
			data[size - 1 - i] = std::stoi(s.substr(i * 2, 2));
		}
	}
}
BigInt::BigInt(const BigInt& other) : size(other.size), capacity(other.capacity), data(new int32_t[other.capacity]), neg(other.neg)
{
	for (size_t i = 0; i < other.size; i++) data[i] = other.data[i];
}
BigInt::BigInt(BigInt&& other) : size(other.size), capacity(other.capacity), neg(other.neg)
{
	data = other.data;
	other.data = nullptr;
}
BigInt::~BigInt()
{
	destroy();
}
BigInt& BigInt::operator=(const BigInt& other)
{
	if (this == &other) return*this;
	destroy();
	size = other.size;
	capacity = other.capacity;
	neg = other.neg;
	data = new int32_t[capacity];
	for (size_t i = 0; i < size; i++) data[i] = other.data[i];
	return *this;
}
BigInt& BigInt::operator=(BigInt&& other)
{
	if (this == &other) return*this;
	destroy();
	size = other.size;
	capacity = other.capacity;
	neg = other.neg;
	data = other.data;
	other.data = nullptr;
	return *this;
}
std::ostream& operator << (std::ostream& os, const BigInt& o)
{
	os << (o.neg ? "-" : "");
	for (size_t i = o.size - 1;; i--)
	{
		os << o.data[i];
		if (i == 0) break;
	}
	return os;
}
BigInt BigInt::operator +(const BigInt& other) const
{
	BigInt res;
	if (neg && !other.neg)
	{
		bool isneg = dataLower(other, *this);
		res = (isneg ? subs(*this, other) : subs(other, *this));
		res.neg = isneg;
	}
	else if (other.neg && !neg)
	{
		bool isneg = dataLower(*this, other);
		res = (isneg ? subs(other, *this) : subs(*this, other));
		res.neg = isneg;
	}
	else
	{
		res = add(*this, other);
		res.neg = neg;
	}
	return res;
}
BigInt BigInt::operator -(const BigInt& other) const
{
	BigInt c = other;
	c.neg = !c.neg;
	return (*this + c);
}
BigInt BigInt::operator-() const
{
	BigInt res(*this);
	res.neg = !res.neg;
	return res;
}
BigInt BigInt::operator*(const BigInt& other) const
{
	BigInt ans(0);
	for (size_t i = 0; i < size; i++)
	{
		BigInt tmp = mul(other, data[i], i);
		ans = add(ans, tmp);
	}
	BigInt res(ans);
	if (neg ^ other.neg) res.neg = true;
	return res;
}
BigInt BigInt::operator +(int32_t other) const
{
	BigInt b(other);
	return (*this + b);
}
BigInt BigInt::operator -(int32_t other) const
{
	BigInt b(other);
	return (*this - b);
}
bool BigInt::operator==(const BigInt& other) const
{
	return (neg == other.neg && dataEquals(*this, other));
}
bool BigInt::operator !=(const BigInt& other) const
{
	return !(*this == other);
}
bool BigInt::operator<(const BigInt& other) const
{
	if (*this == other) return false;
	if (neg && !other.neg) return false;
	if (!neg && other.neg) return true;
	if (!neg) return dataLower(*this, other);
	return !dataLower(*this, other);
}
bool BigInt::operator>(const BigInt& other) const
{
	if (*this == other) return false;
	return !(*this < other);
}
bool BigInt::operator<=(const BigInt& other) const
{
	return ((*this == other) || (*this < other));
}
bool BigInt::operator>=(const BigInt& other) const
{
	return ((*this == other) || (*this > other));
}
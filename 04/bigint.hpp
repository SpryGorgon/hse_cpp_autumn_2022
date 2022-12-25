#pragma once
#include <iostream>

class BigInt
{
private:
	size_t size, capacity, base = 100;
	int32_t* data;
	bool neg;
private:
	void destroy();
	static BigInt add(const BigInt& a, const BigInt& b);
	static BigInt subs(const BigInt& a, const BigInt& b);
	static BigInt mul(const BigInt& a, const int32_t b, size_t n_zeros = 0);
	static bool dataEquals(const BigInt& a, const BigInt& b);
	static bool dataLower(const BigInt& a, const BigInt& b);
public:
	BigInt();
	BigInt(int32_t n);
	BigInt(const std::string& s);
	BigInt(const BigInt& other);
	BigInt(BigInt&& other);
	~BigInt();
	BigInt& operator=(const BigInt& other);
	BigInt& operator=(BigInt&& other);
	friend std::ostream& operator << (std::ostream& os, const BigInt& o);
	BigInt operator +(const BigInt& other) const;
	BigInt operator -(const BigInt& other) const;
	BigInt operator-() const;
	BigInt operator*(const BigInt& other) const;
	BigInt operator +(int32_t other) const;
	BigInt operator -(int32_t other) const;
	bool operator==(const BigInt& other) const;
	bool operator !=(const BigInt& other) const;
	bool operator<(const BigInt& other) const;
	bool operator>(const BigInt& other) const;
	bool operator<=(const BigInt& other) const;
	bool operator>=(const BigInt& other) const;
};
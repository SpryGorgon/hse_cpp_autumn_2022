#pragma once
#include <iostream>
#include <exception>

class Matrix
{
public:
	class Row
	{
	private:
		int* data;
		size_t size;
	public:
		Row();
		Row(size_t cols);
		~Row();
		void destroy();
		void init(size_t size, int data = 0);
		int& operator [] (const size_t j);
		const int& operator [] (const size_t j) const;
	};
private:
	Row* data;
	size_t shape[2];
private:
	void init(int data = 0);
	void destroy();
public:
	Matrix();
	Matrix(size_t s1, size_t s2, int data = 0);

	Matrix(const Matrix& other);
	Matrix(Matrix&& other) noexcept;
	~Matrix();

	size_t getRows() const;
	size_t getColumns() const;

	friend std::ostream& operator << (std::ostream& os, const Matrix& o);

	Row& operator [] (const size_t i);

	const Row& operator [] (const size_t i) const;

	Matrix operator + (const Matrix& other) const;

	Matrix operator - (const Matrix& other) const;

	void operator *= (const double other);

	bool operator == (const Matrix& other) const;

	bool operator != (const Matrix& other) const;
};
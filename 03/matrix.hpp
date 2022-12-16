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
		int size;
	public:
		Row();
		Row(int cols);
		~Row();
		void destroy();
		void init(int size, int data = 0);
		int& operator [] (const int j);
		const int& operator [] (const int j) const;
	};
private:
	Row* data;
	int shape[2];
private:
	void init(int data = 0);
	void destroy();
public:
	Matrix();
	Matrix(int s1, int s2, int data = 0);

	Matrix(const Matrix& other);
	Matrix(Matrix&& other) noexcept;
	~Matrix();

	int getRows() const;
	int getColumns() const;

	friend std::ostream& operator << (std::ostream& os, const Matrix& o);

	Row& operator [] (const int i);

	const Row& operator [] (const int i) const;

	Matrix operator + (const Matrix& other) const;

	Matrix operator - (const Matrix& other) const;

	void operator *= (const double other);

	bool operator == (const Matrix& other) const;

	bool operator != (const Matrix& other) const;
};
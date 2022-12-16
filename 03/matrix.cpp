#include <iostream>
#include <exception>
#include <assert.h>
#include "matrix.hpp"

Matrix::Row::Row() :data(nullptr), size(0) {}
Matrix::Row::Row(int cols)
{
	data = new int[cols];
	size = cols;
}
Matrix::Row::~Row()
{
	destroy();
}
void Matrix::Row::destroy()
{
	if (data == nullptr) return;
	delete[] data;
	data = nullptr;
	size = 0;
}
void Matrix::Row::init(int size, int data)
{
	if (this->data != nullptr) destroy();
	this->data = new int[size];
	this->size = size;
	for (int i = 0; i < size; i++) this->data[i] = data;
}
int& Matrix::Row::operator [] (const int j)
{
	if (j<0 || j>size) throw std::out_of_range("Out of range");
	return data[j];
}
const int& Matrix::Row::operator [] (const int j) const
{
	if (j<0 || j>size) throw std::out_of_range("Out of range");
	return data[j];
}

void Matrix::init(int data)
{
	for (int i = 0; i < shape[0]; i++) this->data[i].init(shape[1], data);
}
void Matrix::destroy()
{
	if (data == nullptr) return;
	delete[] data;
	data = nullptr;
	shape[0] = 0; shape[1] = 0;
}

Matrix::Matrix()
{
	data = nullptr;
	shape[0] = 0; shape[1] = 0;
}
Matrix::Matrix(int s1, int s2, int data)
{
	shape[0] = s1; shape[1] = s2;
	this->data = new Matrix::Row[s1];
	init(data);
}

Matrix::Matrix(const Matrix& other)
{
	shape[0] = other.getRows(); shape[1] = other.getColumns();

	data = new Matrix::Row[shape[0]];
	init();
	for (int i = 0; i < shape[0]; i++) for (int j = 0; j < shape[1]; j++) data[i][j] = other[i][j];
}
Matrix::Matrix(Matrix&& other) noexcept : data(other.data)
{
	other.data = nullptr;
	shape[0] = other.shape[0];
	shape[1] = other.shape[1];
}
Matrix::~Matrix()
{
	destroy();
}

int Matrix::getRows() const
{
	return shape[0];
}
int Matrix::getColumns() const
{
	return shape[1];
}

std::ostream& operator << (std::ostream& os, const Matrix& o)
{
	os << "[";
	for (int i = 0; i < o.getRows(); i++)
	{
		os << "[";
		for (int j = 0; j < o.getColumns(); j++)
		{
			os << o[i][j] << (j == (o.shape[1] - 1) ? "]" : ", ");
		}
		os << (i == (o.getRows() - 1) ? "]" : ",\n");
	}
	return os;
}

Matrix::Row& Matrix::operator [] (const int i)
{
	if (i < 0 || i > shape[0]) throw std::out_of_range("Out of range");
	return data[i];
}

const Matrix::Row& Matrix::operator [] (const int i) const
{
	if (i<0 || i>shape[0]) throw std::out_of_range("Out of range");
	return data[i];
}

Matrix Matrix::operator + (const Matrix& other) const
{
	if (shape[0] != other.getRows() || shape[1] != other.getColumns()) throw std::runtime_error("invalid shapes");
	Matrix res(shape[0], shape[1]);
	for (int i = 0; i < shape[0]; i++) for (int j = 0; j < shape[1]; j++) res[i][j] = data[i][j] + other[i][j];
	return res;
}

Matrix Matrix::operator - (const Matrix& other) const
{
	if (shape[0] != other.getRows() || shape[1] != other.getColumns()) throw std::runtime_error("invalid shapes");
		Matrix res(shape[0], shape[1]);
		for (int i = 0; i < shape[0]; i++) for (int j = 0; j < shape[1]; j++) res[i][j] = data[i][j] - other[i][j];
		return res;
}

void Matrix::operator *= (const double other)
{
	for (int i = 0; i < shape[0]; i++) for (int j = 0; j < shape[1]; j++) data[i][j] *= other;
}

bool Matrix::operator == (const Matrix& other) const
{
	if (shape[0] != other.getRows() || shape[1] != other.getColumns()) return false;
	for (int i = 0; i < shape[0]; i++) for (int j = 0; j < shape[1]; j++) if (data[i][j] != other[i][j]) return false;
	return true;
}

bool Matrix::operator != (const Matrix& other) const
{
	return !(*this == other);
}

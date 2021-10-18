#pragma once
#include <vector>
#include <stdexcept>
#include <string.h>
#include <iostream>
#include <math.h>
#include "vector.h"
namespace JSL
{
	
	class Matrix
	{
		public:
					
			Matrix(const int n,const int m): nRows(n), nCols(m)
			{
				Data = std::vector<std::vector<double>>(n,std::vector<double>(m,0.0));
			}
			
			Matrix(std::vector<std::vector<double>> input): nRows(input.size()), nCols( input[0].size())
			{
				if (nCols <= 0)
				{
					throw std::runtime_error("Cannot construct a JSL::Matrix with zero columns");
				}
				Data = std::vector<std::vector<double>>(nRows,std::vector<double>(nCols,0.0));
				for (int i = 0; i < nRows; ++i)
				{
					if (input[i].size() != nCols)
					{
						
						throw std::runtime_error("Cannot construct a JSL::Matrix from rows with differing number of columns. Please ensure input array is rectangular.");
					}
					for (int j = 0; j < nCols; ++j)
					{
						Data[i][j] = input[i][j];
					}
				}
				
			}
			
			Matrix(const Matrix & input): nRows(input.Rows()), nCols(input.Columns())
			{
				Data = std::vector<std::vector<double>>(nRows,std::vector<double>(nCols,0.0));
				for (int i = 0; i < nRows; ++i)
				{
					for (int j = 0; j < nCols; ++j)
					{
						Data[i][j] = input(i,j);

					}
				}
			}
			
			int Rows() const
			{
				return nRows;
			}
			int Columns() const
			{
				return nCols;
			}
			
			Matrix Transpose()
			{
				Matrix output(nCols,nRows);
				for (int i = 0; i < nRows; ++i)
				{
					for (int j = 0; j <nCols; ++j)
					{
						output(j,i) = Data[i][j];
					}
				}
				return output;
			}
			
			JSL::Vector GetRow(int rowID) const
			{
				if (rowID < nRows && rowID >= 0)
				{
					return JSL::Vector(Data[rowID]);
				}
				else
				{
					throw std::runtime_error("ERROR in JSL::Matrix: Row " + std::to_string(rowID) + " is out of bounds (0->" + std::to_string(nRows-1) + ")");
				}
			}
			JSL::Vector GetColumn(int colID) const
			{
				if (colID < nCols && colID >= 0)
				{
					std::vector<double> data(nRows,0.0);
					for (int i = 0; i < nRows; ++i)
					{
						data[i] = Data[i][colID];
						
					}
					return JSL::Vector(data);
				}
				else
				{
					throw std::runtime_error("ERROR in JSL::Matrix: Column " + std::to_string(colID) + " is out of bounds (0->" + std::to_string(nCols-1) + ")");
				}
			}
			
			
			//!specialised psuedo-constructor for the identity matrix
			static Matrix Identity(int n)
			{
				JSL::Matrix m(n,n);
				for (int i = 0; i < n; ++i)
				{
					m(i,i) = 1;
				}
				return m;
			}
		
		
			double & operator()(int rowID, int columnID)
			{
				if (rowID < nRows && rowID >= 0 && columnID < nCols && columnID >= 0)
				{
					return Data[rowID][columnID];
				}
				else
				{
					throw std::runtime_error(outOfBoundsError(rowID,columnID));
				}
			}
		
			const double & operator()(int rowID, int columnID) const
			{
				
				if (rowID < nRows && rowID >= 0 && columnID < nCols && columnID >= 0)
				{
					return Data[rowID][columnID];
				}
				else
				{
					throw std::runtime_error(outOfBoundsError(rowID,columnID));
				}
			}
		
			//! In-place addition of two matrices. \param rhs The matrix to be accumulated into the current object. Must be the same dimensions as the calling object. \returns A reference to the now-modified calling object
			Matrix & operator+=(const Matrix & rhs)
			{
				if (nRows != rhs.Rows() || nCols != rhs.Columns())
				{
					throw std::runtime_error("Cannot add vectors of different sizes");
				}
				for (int i = 0; i < nRows; ++i)
				{
					for (int j = 0; j < nCols; ++j)
					{
						Data[i][j] += rhs(i,j);
					}
				}
				return *this;
			}
			
			//! In-place subtraction of two matrices. \param rhs The matrix to be subtracted from the current object. Must be the same dimensions as the calling object. \returns A reference to the now-modified calling object
			Matrix & operator-=(const Matrix & rhs)
			{
				if (nRows != rhs.Rows() || nCols != rhs.Columns())
				{
					throw std::runtime_error("Cannot subtract vectors of different sizes");
				}
				for (int i = 0; i < nRows; ++i)
				{
					for (int j = 0; j < nCols; ++j)
					{
						Data[i][j] -= rhs(i,j);
					}
				}
				return *this;
			}
			
			//! In-place addition of a scalar onto the calling object. \param scalar The double to be accumulated into the current object. \returns A reference to the now-modified calling object
			Matrix & operator+=(const double & scalar)
			{
				for (int i = 0; i < nRows; ++i)
				{
					for (int j = 0; j < nCols; ++j)
					{
						Data[i][j] += scalar;
					}
				}
				return *this;
			}
			
			//! In-place subtraction of a scalar onto the calling object. \param scalar The double to be subtracted from the current object. \returns A reference to the now-modified calling object
			Matrix & operator-=(const double & scalar)
			{
				for (int i = 0; i < nRows; ++i)
				{
					for (int j = 0; j < nCols; ++j)
					{
						Data[i][j] -= scalar;
					}
				}
				return *this;
			}
			
			//! In-place multiplication of a scalar onto the calling object. \param scalar The double to be accumulated into the current object. \returns A reference to the now-modified calling object
			Matrix & operator*=(const double & scalar)
			{
				for (int i = 0; i < nRows; ++i)
				{
					for (int j = 0; j < nCols; ++j)
					{
						Data[i][j] *= scalar;
					}
				}
				return *this;
			}
			
			Matrix & operator*=(const Matrix & rhs)
			{
				if (Columns() != rhs.Rows())
				{
					std::string s1 = "(" + std::to_string(Rows()) + "x"+ std::to_string(Columns()) + ")";
					std::string s2 = "(" + std::to_string(rhs.Rows()) + "x"+ std::to_string(rhs.Columns()) + ")";
					throw std::runtime_error("JSL::Vector Error: Cannot peform *= operation on matrices of incompatible sizes: " + s1 + " vs " + s2);
				}
				
				std::vector<std::vector<double>>newData(nRows,std::vector<double>(rhs.Columns(),0.0));
				
				for (int i = 0; i < newData.size(); ++i)
				{
					for (int j = 0; j < newData[0].size(); ++j)
					{
						for (int k = 0; k < Columns(); ++k)
						{
							newData[i][j] += Data[i][k] * rhs(k,j);
						}
					}
				}
				
				//Assign in....changes the shape of the object potentially!
				Data = newData;
				nRows = Data.size();
				nCols = Data[0].size();
				return *this;
			}
			
			//! In-place division of a scalar onto the calling object. \param scalar The double to divide the current object by. \returns A reference to the now-modified calling object
			Matrix & operator/=(const double & scalar)
			{
				for (int i = 0; i < nRows; ++i)
				{
					for (int j = 0; j < nCols; ++j)
					{
						Data[i][j] /= scalar;
					}
				}
				return *this;
			}
							
			std::string to_string() const
			{
				std::string outString = "[";
				for (int i = 0; i < nRows; ++i)
				{
					outString += "[";
					for (int j = 0; j < nCols; ++j)
					{
						outString += std::to_string(Data[i][j]);
						if (j < nCols - 1)
						{
							outString += ", ";
						}
					}
					outString += "]";
					if (i < nRows - 1)
					{
						outString += "\n";
					}
				}
				outString += "]";
				return outString;
			}
		
		
		
		private:
			std::vector<std::vector<double>> Data;
			
			int nRows;
			int nCols;

			std::string inline outOfBoundsError(int idx1,int idx2) const
			{
				const std::string coord = "(" + std::to_string(idx1) + ", " + std::to_string(idx2) + ")";
				const std::string size =  "(" + std::to_string(Rows()) + ", " + std::to_string(Columns()) + ")";
				return "ERROR in JSL::Vector: Attempted to access " + coord + " but matrix is of size " + size;			
			}
	};
	
	inline bool operator==(const Matrix & lhs, const Matrix & rhs)
	{
		if (lhs.Columns() != rhs.Columns() || lhs.Rows() != rhs.Rows())
		{
			return false;
		}
		
		for (int i = 0; i < lhs.Rows();++i)
		{
			for (int j = 0; j < lhs.Columns(); ++j)
			{
				if (lhs(i,j) != rhs(i,j))
				{
					return false;
				}
			}
		}
		return true;
	}
	
	inline bool operator!=(const Matrix & lhs, const Matrix & rhs)
	{
		return !(lhs == rhs);
	}
	
		
	bool MatrixSizesEqual(const Matrix & m1, const Matrix & m2)
	{
		return (m1.Rows() == m2.Rows() && m1.Columns() == m2.Columns() );
	}	
	
	//! Performs obvious matrix addition (a+b)_ij = a_ij + b_ij. Throws an error if the matrices are not the same size. \param lhs The first vector to be summed \param rhs The second vector to be summed (order is irrelevant) \return The vector lhs + rhs
	inline Matrix operator+(const Matrix & lhs, const Matrix & rhs)
	{
		if (!MatrixSizesEqual(lhs,rhs))
		{
			throw std::runtime_error("JSL::Vector Error: Cannot peform + operation on matrices of different sizes");
		}
		
		JSL::Matrix output = lhs;
		
		for (int i = 0; i < output.Rows(); ++i)
		{
			for (int j = 0; j < output.Columns(); ++j)
			{
				output(i,j) += rhs(i,j);
			}
		}
		return output;
	};
	//! Performs obvious matrix subtraction (a-b)_ij = a_ij - b_ij. Throws an error if the matrices are not the same size. \param lhs The first vector  \param rhs The  vector to be subtracted from the first \return The matrix lhs - rhs
	inline Matrix operator-(const Matrix & lhs, const Matrix & rhs)
	{
		if (!MatrixSizesEqual(lhs,rhs))
		{
			throw std::runtime_error("JSL::Vector Error: Cannot peform + operation on matrices of different sizes");
		}
		
		Matrix output = lhs;
		
		for (int i = 0; i < output.Rows(); ++i)
		{
			for (int j = 0; j < output.Columns(); ++j)
			{
				output(i,j) -= rhs(i,j);
			}
		}
		return output;
	};

	//! Adds the value of scalar to every element in the matrix \param lhs The matrix to be summed \param scalar The scalar to be added element-wise \return The matrix lhs + scalar
	inline Matrix operator+(const Matrix & lhs, const double & scalar)
	{

		Matrix output = lhs;
		
		for (int i = 0; i < output.Rows(); ++i)
		{
			for (int j = 0; j < output.Columns(); ++j)
			{
				output(i,j) += scalar;
			}
		}
		return output;
	};
	
	
	inline Matrix operator*(const Matrix & lhs, const Matrix & rhs)
	{
		if (lhs.Columns() != rhs.Rows())
		{
			std::string s1 = "(" + std::to_string(lhs.Rows()) + "x"+ std::to_string(lhs.Columns()) + ")";
			std::string s2 = "(" + std::to_string(rhs.Rows()) + "x"+ std::to_string(rhs.Columns()) + ")";
			throw std::runtime_error("JSL::Vector Error: Cannot peform  operation on matrices of incompatible sizes: " + s1 + " vs " + s2);
		}
		
		Matrix output(lhs.Rows(),rhs.Columns());
		
		for (int i = 0; i < output.Rows(); ++i)
		{
			for (int j = 0; j < output.Columns(); ++j)
			{
				for (int k = 0; k < lhs.Columns(); ++k)
				{
					output(i,j) += lhs(i,k) * rhs(k,j);
				}
			}
		}
		return output;
	}
	
	inline Vector operator*(const Matrix & lhs, const Vector & rhs)
	{
		if (lhs.Columns() != rhs.Size())
		{
			std::string s1 = "(" + std::to_string(lhs.Rows()) + "x"+ std::to_string(lhs.Columns()) + ")";
			std::string s2 = "(" + std::to_string(rhs.Size()) + ")";
			throw std::runtime_error("JSL::Vector Error: Cannot multiply matrices and vectors of incompatible sizes: " + s1 + " vs " + s2);
		}
		
		Vector output(lhs.Rows());
		
		for (int i = 0; i < output.Size(); ++i)
		{
			for (int k = 0; k < lhs.Columns(); ++k)
			{
				output[i] += lhs(i,k) * rhs[k];
		
			}
		}
		return output;
	}
	
	//! Exactly equivalent to JSL::operator+(const Matrix &lhs, const double &scalar), just swapped around. \param scalar The scalar to be added element-wise \param rhs The matrix to be summed \return The scalar + rhs
	inline Matrix operator+(const double & scalar, const Matrix & rhs)
	{
		return rhs + scalar;
	}
	//! Subtracts the value of scalar to every element in the Matrix \param lhs The base matrix \param scalar The scalar to be subtracted from the base matrix element wise \return The vector lhs - scalar.
	inline Matrix operator-(const Matrix & lhs, const double & scalar)
	{
		return lhs + -1*scalar;
	}
	
	
	//! Naive element-wise scalar multiplication. \param scalar The value to multiply elements by \param rhs The matrix to multiply \returns The pointwise product of the elements of rhs and the scalar
	inline Matrix operator*(const double & scalar, const Matrix & rhs)
	{
		Matrix output = rhs;
		
		for (int i = 0; i < output.Rows(); ++i)
		{
			for (int j = 0; j < output.Columns(); ++j)
			{
				output(i,j) *= scalar;
			}
		}
		return output;
	}
	
	//! Alias of JSL::operator+(const double &scalar,const Matrix &rhs) with the operation order swapped around  \param lhs The matrix to multiply \param scalar The value to multiply elements by \returns The pointwise product of the elements of lhs and the scalar
	inline Matrix operator*(const Matrix & lhs, const double & scalar)
	{
		return scalar * lhs;
	}
	
	//! Essentially an alias for JSL::operator+(const double &scalar,const Matrix &rhs) with the scalar set to one-over itself, i.e. pointwise division of the provided matrix. \param lhs The matrix to divide \param scalar The value to divide elements by \returns The pointwise divisor of the elements of lhs and the scalar
	inline Matrix operator/(const Matrix & lhs, const double & scalar)
	{
		if (scalar == 0)
		{
			throw std::runtime_error("Division by zero results in undefined behaviour");
		}
		return 1.0/scalar * lhs;
	}
	//! A slightly odd operation (included for completeness) - adds the value of scalar to the negative of the elements of the vector \param scalar The value which acts as a base \param rhs The vector which will be subtracted elementwise from the base scalar \return The vector scalar - rhs
	inline Matrix operator-(const double & scalar, const Matrix & rhs)
	{
		return scalar + -1 * rhs;
	}
	
	
	//! Calls JSL::Matrix::to_string() and then passes it to the provided stream, enabling sweet, smooth output such as std::cout << M << std::endl. \param os An output stream capable of parsing strings \param obj A Matrix object to be inserted into the stream for output \returns A reference to the modified stream 
	inline std::ostream& operator<<(std::ostream& os, const Matrix & obj) 
	{
		os << obj.to_string();
		return os;
	}
}

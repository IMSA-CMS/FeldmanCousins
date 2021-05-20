
#include "stdafx.h"
#include "Matrix.h"


// Swap rows i and k of a matrix A
// Note that due to the reference, both getDimensions are preserved for
// built-in arrays
template<typename MatrixType>
void swap_rows(MatrixType& A,
	typename matrix_traits<MatrixType>::index_type i,
	typename matrix_traits<MatrixType>::index_type k)
{
	matrix_traits<MatrixType> mt;
	typedef typename matrix_traits<MatrixType>::index_type index_type;

	// check indices
	assert(mt.min_row(A) <= i);
	assert(i <= mt.max_row(A));

	assert(mt.min_row(A) <= k);
	assert(k <= mt.max_row(A));

	for (index_type col = mt.min_column(A); col <= mt.max_column(A); ++col)
		std::swap(mt.element(A, i, col), mt.element(A, k, col));
}

// divide row i of matrix A by v
template<typename MatrixType>
void divide_row(MatrixType& A,
	typename matrix_traits<MatrixType>::index_type i,
	typename matrix_traits<MatrixType>::value_type v)
{
	matrix_traits<MatrixType> mt;
	typedef typename matrix_traits<MatrixType>::index_type index_type;

	assert(mt.min_row(A) <= i);
	assert(i <= mt.max_row(A));

	assert(v != 0);

	for (index_type col = mt.min_column(A); col <= mt.max_column(A); ++col)
		mt.element(A, i, col) /= v;
}

// in matrix A, add v times row k to row i
template<typename MatrixType>
void add_multiple_row(MatrixType& A,
	typename matrix_traits<MatrixType>::index_type i,
	typename matrix_traits<MatrixType>::index_type k,
	typename matrix_traits<MatrixType>::value_type v)
{
	matrix_traits<MatrixType> mt;
	typedef typename matrix_traits<MatrixType>::index_type index_type;

	assert(mt.min_row(A) <= i);
	assert(i <= mt.max_row(A));

	assert(mt.min_row(A) <= k);
	assert(k <= mt.max_row(A));

	for (index_type col = mt.min_column(A); col <= mt.max_column(A); ++col)
		mt.element(A, i, col) += v * mt.element(A, k, col);
}

// convert A to reduced row echelon form
template<typename MatrixType>
void to_reduced_row_echelon_form(MatrixType& A)
{
	matrix_traits<MatrixType> mt;
	typedef typename matrix_traits<MatrixType>::index_type index_type;

	index_type lead = mt.min_row(A);

	for (index_type row = mt.min_row(A); row <= mt.max_row(A); ++row)
	{
		if (lead > mt.max_column(A))
			return;
		index_type i = row;
		while (mt.element(A, i, lead) == 0)
		{
			++i;
			if (i > mt.max_row(A))
			{
				i = row;
				++lead;
				if (lead > mt.max_column(A))
					return;
			}
		}
		swap_rows(A, i, row);
		divide_row(A, row, mt.element(A, row, lead));
		for (i = mt.min_row(A); i <= mt.max_row(A); ++i)
		{
			if (i != row)
				add_multiple_row(A, i, row, -mt.element(A, i, lead));
		}
	}
}
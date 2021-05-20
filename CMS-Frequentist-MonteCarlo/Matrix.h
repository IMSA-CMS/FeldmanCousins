#ifndef CMS_FREQUENTIST_MONTECARLO_MATRIXTRAITS_H
#define CMS_FREQUENTIST_MONTECARLO_MATRIXTRAITS_H
#include <algorithm> // for std::swap
#include <cstddef>
#include <cassert>

#include <vector>

// Matrix traits: This describes how a matrix is accessed. By
// externalizing this information into a traits class, the same code
// can be used both with native arrays and matrix classes. To use the
// default implementation of the traits class, a matrix type has to
// provide the following definitions as members:
//
// * typedef ... index_type;
//   - The type used for indexing (e.g. size_t)
// * typedef ... value_type;
//   - The element type of the matrix (e.g. double)
// * index_type min_row() const;
//   - returns the minimal allowed row index
// * index_type max_row() const;
//   - returns the maximal allowed row index
// * index_type min_column() const;
//   - returns the minimal allowed column index
// * index_type max_column() const;
//   - returns the maximal allowed column index
// * value_type& operator()(index_type i, index_type k)
//   - returns a reference to the element i,k, where
//     min_row() <= i <= max_row()
//     min_column() <= k <= max_column()
// * value_type operator()(index_type i, index_type k) const
//   - returns the value of element i,k
//
// Note that the functions are all inline and simple, so the compiler
// should completely optimize them away.
template<typename MatrixType> struct matrix_traits
{
    typedef unsigned index_type;
    typedef double value_type;
    static index_type min_row(MatrixType const& A)
    {
        return 0;
    }
    static index_type max_row(MatrixType const& A)
    {
        return A.size() - 1;
    }
    static index_type min_column(MatrixType const& A)
    {
        return 0;
    }
    static index_type max_column(MatrixType const& A)
    {
        return A[0].size() - 1;
    }
    static value_type& element(MatrixType& A, index_type i, index_type k)
    {
        return A[i][k];
    }
    static value_type element(MatrixType const& A, index_type i, index_type k)
    {
        return A[i][k];
    }
};

// specialization of the matrix traits for built-in two-dimensional
// arrays
template<typename T, std::size_t rows, std::size_t columns>
struct matrix_traits<T[rows][columns]>
{
    typedef std::size_t index_type;
    typedef T value_type;
    static index_type min_row(T const (&)[rows][columns])
    {
        return 0;
    }
    static index_type max_row(T const (&)[rows][columns])
    {
        return rows - 1;
    }
    static index_type min_column(T const (&)[rows][columns])
    {
        return 0;
    }
    static index_type max_column(T const (&)[rows][columns])
    {
        return columns - 1;
    }
    static value_type& element(T(&A)[rows][columns],
                               index_type i, index_type k)
    {
        return A[i][k];
    }
    static value_type element(T const (&A)[rows][columns],
                              index_type i, index_type k)
    {
        return A[i][k];
    }
};

// specialization of the matrix traits for vector of vectors
template<>
struct matrix_traits<std::vector<std::vector<double, double>, double>>
{
typedef std::size_t index_type;
typedef double value_type;
static index_type min_row(const std::vector<std::vector<double>>&)
{
    return 0;
}
static index_type max_row(const std::vector<std::vector<double>>& vec)
{
    return vec.size() - 1;
}
static index_type min_column(const std::vector<std::vector<double>>&)
{
    return 0;
}
static index_type max_column(const std::vector<std::vector<double>>& vec)
{
    return vec[0].size() - 1;
}
static value_type& element(std::vector<std::vector<double>>& A,
                           index_type i, index_type k)
{
    return A[i][k];
}
static value_type element(const std::vector<std::vector<double>> &A,
                          index_type i, index_type k)
{
    return A[i][k];
}
};

// Swap rows i and k of a matrix A
// Note that due to the reference, both getDimensions are preserved for
// built-in arrays
template<typename MatrixType>
void swap_rows(MatrixType& A,
               typename matrix_traits<MatrixType>::index_type i,
               typename matrix_traits<MatrixType>::index_type k);


// divide row i of matrix A by v
template<typename MatrixType>
void divide_row(MatrixType& A,
                typename matrix_traits<MatrixType>::index_type i,
                typename matrix_traits<MatrixType>::value_type v);


// in matrix A, add v times row k to row i
template<typename MatrixType>
void add_multiple_row(MatrixType& A,
                      typename matrix_traits<MatrixType>::index_type i,
                      typename matrix_traits<MatrixType>::index_type k,
                      typename matrix_traits<MatrixType>::value_type v);


// convert A to reduced row echelon form
template<typename MatrixType>
void to_reduced_row_echelon_form(MatrixType& A);
#endif
#pragma once

#include <Eigen/Core>
#include <array>

namespace predicates {
    using std::size_t;
    template <size_t N> using IndexSet = std::array<size_t, N>;


    /**
     * Return an array with the element at index `k` removed
     */
    template <size_t k, typename T, size_t N>
    constexpr std::array<T, N - 1> drop(std::array<T, N> arr) {
        std::array<T, N - 1> result{};
        for (size_t i = 0; i < k; ++i)
            result[i] = arr[i];
        for (size_t i = k; i < N - 1; ++i)
            result[i] = arr[i + 1];
        return result;
    }


    /**
     * Return the array `{0, 1, 2, ..., N - 1}`
     */
    template <size_t N>
    constexpr IndexSet<N> make_index_set() {
        IndexSet<N> result;
        for (size_t i = 0; i < N; ++i)
            result[i] = i;
        return result;
    }


    /**
     * Thanks to this brilliant bastard:
     * https://artificial-mind.net/blog/2020/10/31/constexpr-for
     */
    template <auto Start, auto End, auto Inc, class F>
    constexpr void constexpr_for(F&& f) {
        if constexpr (Start < End) {
            f(std::integral_constant<decltype(Start), Start>());
            constexpr_for<Start + Inc, End, Inc>(f);
        }
    }


    /**
     * Helper class for computing determinants of sub-matrices
     */
    template <size_t N, IndexSet<N> Is, IndexSet<N> Js>
    struct determinant_helper;


    /**
     * Base case: the determinant of a 2 x 2 matrix
     */
    template <IndexSet<2> Is, IndexSet<2> Js>
    struct determinant_helper<2, Is, Js> {
        template <typename Scalar, typename Matrix>
        Scalar run(const Matrix& matrix) {
            return
                Scalar(matrix(Is[0], Js[0])) * Scalar(matrix(Is[1], Js[1]))
                -
                Scalar(matrix(Is[0], Js[1])) * Scalar(matrix(Is[1], Js[0]));
        }
    };


    /**
     * Induction case: the determinant of an N x N matrix is the alternating
     * sum of the determinants of (N - 1) x (N - 1) sub-matrices
     */
    template <size_t N, IndexSet<N> Is, IndexSet<N> Js>
    struct determinant_helper {
        template <typename Scalar, typename Matrix>
        Scalar run(const Matrix& matrix) {
            Scalar result = 0;
            auto fn = [&](auto k) {
                auto m_ij = Scalar(matrix(Is[0], Js[k]));
                using helper =
                    determinant_helper<N - 1, drop<0>(Is), drop<k>(Js)>;
                auto det = m_ij * helper().template run<Scalar>(matrix);
                result = result + (((k % 2) == 0) ? +1 : -1) * det;
            };
            constexpr_for<0, N, 1>(fn);
            return result;
        }
    };


    /**
     * The outer-level interface to computing determinants of Eigen matrices.
     *
     * A critical feature is that we can internally convert all the matrix
     * entries to a type `Result` that may differ from the type `Scalar` used
     * to store the matrix entries. That way we can get determinants using
     * interval or rational arithmetic.
     */
    template <typename Result>
    struct Determinant {
        template <typename Scalar, int N>
        static Result run(const Eigen::Matrix<Scalar, N, N>& matrix) {
            constexpr auto Is = make_index_set<N>();
            using helper = determinant_helper<N, Is, Is>;
            return helper().template run<Result>(matrix);
        }
    };
}

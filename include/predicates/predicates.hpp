#pragma once

#include <Eigen/Core>
#include <Eigen/Dense>
#include <boost/numeric/interval.hpp>
#include <boost/numeric/interval/utility.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/eigen.hpp>
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
        IndexSet<N> result{};
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
    template <typename T, size_t N, size_t M, IndexSet<M> Is, IndexSet<M> Js>
    struct determinant_helper;


    /**
     * Base case: the determinant of a 2 x 2 matrix
     */
    template <typename T, size_t N, IndexSet<2> Is, IndexSet<2> Js>
    struct determinant_helper<T, N, 2, Is, Js> {
        T run(const Eigen::Matrix<T, N, N>& matrix) {
            return
                matrix(Is[0], Js[0]) * matrix(Is[1], Js[1])
                -
                matrix(Is[0], Js[1]) * matrix(Is[1], Js[0]);
        }
    };


    /**
     * Induction case: the determinant of an N x N matrix is the alternating
     * sum of the determinants of (N - 1) x (N - 1) sub-matrices
     */
    template <typename T, size_t N, size_t M, IndexSet<M> Is, IndexSet<M> Js>
    struct determinant_helper {
        T run(const Eigen::Matrix<T, N, N>& matrix) {
            T result = 0;
            auto fn = [&](auto k) {
                auto m_ij = matrix(Is[0], Js[k]);
                using helper =
                    determinant_helper<T, N, M - 1, drop<0>(Is), drop<k>(Js)>;
                auto det = m_ij * helper().run(matrix);
                if (k % 2 == 0)
                    result += det;
                else
                    result -= det;
            };
            constexpr_for<0, M, 1>(fn);
            return result;
        }
    };


    /**
     * The outer-level interface to computing determinants of Eigen matrices.
     */
    template <typename T, int N>
    T determinant(const Eigen::Matrix<T, N, N>& matrix) {
        constexpr auto Is = make_index_set<N>();
        return determinant_helper<T, N, N, Is, Is>().run(matrix);
    }


    /**
     * Sign-exact determinant computation on Eigen matrices using intervals as
     * a first resort and rationals as a fallback.
     */
    template <typename T, int N>
    T sign_exact_determinant(const Eigen::Matrix<T, N, N>& matrix) {
        using Interval = boost::numeric::interval<T>;
        const Eigen::Matrix<Interval, N, N> interval_matrix =
            matrix.template cast<Interval>();
        const Interval interval_result = determinant(interval_matrix);
        if (not boost::numeric::zero_in(interval_result))
            return boost::numeric::median(interval_result);

        using Rational = boost::multiprecision::cpp_rational;
        const Eigen::Matrix<Rational, N, N> rational_matrix =
            matrix.template cast<Rational>();
        return determinant(rational_matrix).template convert_to<T>();
    }
}

#pragma once

#include <array>

namespace predicates {
    using std::size_t;
    template <size_t N> using IndexSet = std::array<size_t, N>;


    template <size_t k, typename T, size_t N>
    constexpr std::array<T, N - 1> drop(std::array<T, N> arr) {
        std::array<T, N - 1> result{};
        for (size_t i = 0; i < k; ++i)
            result[i] = arr[i];
        for (size_t i = k; i < N - 1; ++i)
            result[i] = arr[i + 1];
        return result;
    }


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


    template <size_t N, IndexSet<N> Is, IndexSet<N> Js>
    struct determinant_helper;


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


    template <size_t N, typename Scalar, typename Matrix>
    auto determinant(const Matrix& matrix) {
        constexpr auto Is = make_index_set<N>();
        return determinant_helper<N, Is, Is>().template run<Scalar>(matrix);
    }
}

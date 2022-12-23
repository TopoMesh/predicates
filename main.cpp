#include <cmath>
#include <iostream>
#include <Eigen/Dense>
#include <boost/multiprecision/cpp_int.hpp>
#include "predicates.hpp"

int main() {

    {
        constexpr predicates::IndexSet<4> r = predicates::make_index_set<4>();
        constexpr predicates::IndexSet<3> s = predicates::drop<0>(r);
    }

    {
        Eigen::Matrix<double, 2, 2> M;
        M << 1, 2, 3, 4;
        double exact = M.determinant();
        double det = predicates::Determinant<double>::run(M);
        std::cout << exact << ", " << det << "\n";
    }

    {
        Eigen::Matrix<double, 3, 3> M;
        const double z = std::nextafter(0.5, 1.5);
        M << 1, 1, 1,
             0.5, 12.0, 24.0,
             z, 12.0, 24.0;
        double det = predicates::Determinant<double>::run(M);
        std::cout << det << "\n";

        using Rational = boost::multiprecision::cpp_rational;
        Rational qdet = predicates::Determinant<Rational>::run(M);
        std::cout << qdet << "\n";
    }

    return 0;
}

#include <cmath>
#include <iostream>
#include <Eigen/Dense>
#include <boost/multiprecision/cpp_int.hpp>
#include <predicates/predicates.hpp>


int main() {
    {
        Eigen::Matrix<double, 2, 2> M;
        M << 1, 2, 3, 4;
        double exact = M.determinant();
        double det = predicates::determinant(M);
        std::cout << exact << ", " << det << "\n";
    }

    {
        Eigen::Matrix<double, 3, 3> M;
        const double z = std::nextafter(0.5, 1.5);
        M << 1, 1, 1,
             0.5, 12.0, 24.0,
             z, 12.0, 24.0;
        double det = predicates::determinant(M);
        std::cout << det << "\n";

        using Rational = boost::multiprecision::cpp_rational;
        const Eigen::Matrix<Rational, 3, 3> N = M.cast<Rational>();
        Rational qdet = predicates::determinant(N);
        std::cout << qdet << "\n";
    }

    {
        Eigen::Matrix<double, 2, 3> M;
        const double z = std::nextafter(0.5, 1.5);
        M << 0.5, 12.0, 24.0,
             z, 12.0, 24.0;
        std::cout << predicates::orientation(M) << "\n";
    }

    return 0;
}

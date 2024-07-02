#include <cmath>
#include <iostream>
#include <Eigen/Dense>
#include <boost/multiprecision/cpp_int.hpp>
#include <predicates/predicates.hpp>


int main() {
    using Interval = boost::numeric::interval<double>;
    using Rational = boost::multiprecision::cpp_rational;

    // Determinant tests
    {
        Eigen::Matrix<double, 2, 2> M;
        M << 1, 2, 3, 4;
        double naive_det = predicates::internal::determinant(M);
        double exact_det = predicates::determinant(M);
        std::cout << naive_det << ", " << exact_det << "\n";
    }

    // Orientation tests
    {
        Eigen::Matrix<double, 2, 3> points;
        const double z = std::nextafter(0.5, 1.5);
        points << 0.5, 12.0, 24.0,
                  z, 12.0, 24.0;
        const double naive_det = predicates::volume(points);

        const Eigen::Matrix<Interval, 2, 3> interval_points = points.cast<Interval>();
        const Interval interval_det = predicates::internal::volume(interval_points);

        const Eigen::Matrix<Rational, 2, 3> rational_points = points.cast<Rational>();
        Rational rational_det = predicates::internal::volume(rational_points);

        const double sign_exact_det = predicates::volume(points);

        std::cout << "Naive volume:      " << naive_det << "\n";
        std::cout << "Interval volume:   "
                  << interval_det.lower() << ", " << interval_det.upper() << "\n";
        std::cout << "Rational volume:   " << rational_det << "\n";
        std::cout << "Sign-exact volume: " << sign_exact_det << "\n";
    }

    // Insphere tests
    {
        Eigen::Matrix<double, 2, 4> points;
        const double z = std::nextafter(-1, +1);
        points << -1, +1, +1, -1,
                  z, -1, +1, +1;

        const double naive_det = predicates::insphere(points);

        const Eigen::Matrix<Interval, 2, 4> interval_points = points.cast<Interval>();
        const Interval interval_det = predicates::internal::insphere(interval_points);

        const Eigen::Matrix<Rational, 2, 4> rational_points = points.cast<Rational>();
        Rational rational_det = predicates::internal::insphere(rational_points);

        const double sign_exact_det = predicates::insphere(points);
        std::cout << "Naive volume:      " << naive_det << "\n";
        std::cout << "Interval volume:   "
                  << interval_det.lower() << ", " << interval_det.upper() << "\n";
        std::cout << "Rational volume:   " << rational_det << "\n";
        std::cout << "Sign-exact volume: " << sign_exact_det << "\n";
    }

    return 0;
}

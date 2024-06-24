#include <predicates/predicates.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>

PYBIND11_MODULE(predicates, m) {
    m.def("sign_exact_determinant", &predicates::sign_exact_determinant<double, 2>);
    m.def("sign_exact_determinant", &predicates::sign_exact_determinant<double, 3>);
    m.def("sign_exact_determinant", &predicates::sign_exact_determinant<double, 4>);
    m.def("sign_exact_determinant", &predicates::sign_exact_determinant<double, 5>);
}

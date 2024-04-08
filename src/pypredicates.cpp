#include <predicates/predicates.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>

PYBIND11_MODULE(predicates, m) {
    m.def("determinant", &predicates::determinant<double, 2>);
    m.def("determinant", &predicates::determinant<double, 3>);
    m.def("determinant", &predicates::determinant<double, 4>);
    m.def("determinant", &predicates::determinant<double, 5>);
}

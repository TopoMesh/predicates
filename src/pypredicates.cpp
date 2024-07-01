#include <predicates/predicates.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>

PYBIND11_MODULE(predicates, m) {
    m.def("determinant", &predicates::determinant<double, 2>);
    m.def("determinant", &predicates::determinant<double, 3>);
    m.def("determinant", &predicates::determinant<double, 4>);
    m.def("determinant", &predicates::determinant<double, 5>);
    m.def("determinant", &predicates::determinant<double, 6>);

    m.def("orientation", &predicates::orientation<double, 2>);
    m.def("orientation", &predicates::orientation<double, 3>);
    m.def("orientation", &predicates::orientation<double, 4>);

    m.def("insphere", &predicates::insphere<double, 2>);
    m.def("insphere", &predicates::insphere<double, 3>);
    m.def("insphere", &predicates::insphere<double, 4>);
}

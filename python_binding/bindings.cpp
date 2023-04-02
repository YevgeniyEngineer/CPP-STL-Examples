// bindings.cpp
#include "vector_of_struct.hpp"
#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(vector_of_struct_py, m)
{
    m.doc() = "Python bindings for MyStruct, createStructVector and printStructVector";

    py::class_<MyStruct>(m, "MyStruct")
        .def(py::init<>())
        .def_readwrite("x", &MyStruct::x)
        .def_readwrite("y", &MyStruct::y)
        .def_readwrite("vec", &MyStruct::vec);

    m.def("createStructVector", &createStructVector, "Create a vector of MyStruct");
    m.def("printStructVector", &printStructVector, "Print a vector of MyStruct");
}

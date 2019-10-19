#ifndef PYIU_MERGE_H
#define PYIU_MERGE_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "helpercompat.h"

typedef struct {
    PyObject_HEAD
    PyObject *iteratortuple;
    PyObject *keyfunc;
    PyObject *current;
    Py_ssize_t numactive;
    int reverse;
} PyIUObject_Merge;

extern PyTypeObject PyIUType_Merge;

#endif

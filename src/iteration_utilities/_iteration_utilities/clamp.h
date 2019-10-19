#ifndef PYIU_CLAMP_H
#define PYIU_CLAMP_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "helpercompat.h"

typedef struct {
    PyObject_HEAD
    PyObject *iterator;
    PyObject *low;
    PyObject *high;
    int inclusive;
    int remove;
} PyIUObject_Clamp;

extern PyTypeObject PyIUType_Clamp;

#endif

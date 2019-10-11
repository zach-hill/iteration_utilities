/******************************************************************************
 * Licensed under Apache License Version 2.0 - see LICENSE
 *****************************************************************************/

#include "nth.h"
#include "docs_reduce.h"
#include "helper.h"
#include <structmember.h>

PyDoc_STRVAR(nth_prop_n_doc,
    "(:py:class:`int`) The index to get (readonly).\n"
    "\n"
    ".. versionadded:: 0.6");

PyDoc_STRVAR(nth_doc,
    "nth(x)\n"
    "--\n\n"
    "Class that returns the `n`-th found value.\n"
    "\n"
    "Parameters\n"
    "----------\n"
    "n : :py:class:`int`\n"
    "    The index of the wanted item. If negative the last item is searched.\n"
    "    \n"
    "    .. note::\n"
    "       This is the only parameter for ``__init__``. The following parameters\n"
    "       have to be specified when calling the instance.\n"
    "\n"
    "iterable : iterable\n"
    "    The `iterable` for which to determine the nth value.\n"
    "\n"
    "default : any type, optional\n"
    "    If no nth value is found and `default` is given the `default` is \n"
    "    returned.\n"
    "\n"
    "pred : callable, optional\n"
    "    If given return the nth item for which ``pred(item)`` is ``True``.\n"
    "    \n"
    "    .. note::\n"
    "       ``pred=None`` is equivalent to ``pred=bool``.\n"
    "\n"
    "truthy : :py:class:`bool`, optional\n"
    "    If ``False`` search for the nth item for which ``pred(item)`` is ``False``.\n"
    "    Default is ``True``.\n"
    "\n"
    "    .. note::\n"
    "       Parameter is ignored if `pred` is not given.\n"
    "\n"
    "retpred : :py:class:`bool`, optional\n"
    "    If given return ``pred(item)`` instead of ``item``.\n"
    "    Default is ``False``.\n"
    "\n"
    "    .. note::\n"
    "       Parameter is ignored if `pred` is not given.\n"
    "\n"
    "retidx : :py:class:`bool`, optional\n"
    "    If given return the index of the `n`-th element instead of the value.\n"
    "    Default is ``False``.\n"
    "\n"
    "Returns\n"
    "-------\n"
    "nth : any type\n"
    "    The last value or the nth value for which `pred` is ``True``.\n"
    "    If there is no such value then `default` is returned.\n"
    "\n"
    "Raises\n"
    "-------\n"
    "TypeError :\n"
    "    If there is no nth element and no `default` is given.\n"
    "\n"
    "Examples\n"
    "--------\n"
    "Some basic examples including the use of ``pred``::\n"
    "\n"
    "    >>> from iteration_utilities import nth\n"
    "    >>> # First item\n"
    "    >>> nth(0)([0, 1, 2])\n"
    "    0\n"
    "    >>> # Second item\n"
    "    >>> nth(1)([0, 1, 2])\n"
    "    1\n"
    "    >>> # Last item\n"
    "    >>> nth(-1)([0, 1, 2])\n"
    "    2\n"
    "    \n"
    "    >>> nth(1)([0, 10, '', tuple(), 20], pred=bool)\n"
    "    20\n"
    "    \n"
    "    >>> # second odd number\n"
    "    >>> nth(1)([0, 2, 3, 5, 8, 9, 10], pred=lambda x: x%2)\n"
    "    5\n"
    "    \n"
    "    >>> # default value if empty or no true value\n"
    "    >>> nth(0)([], default=100)\n"
    "    100\n"
    "    >>> nth(-1)([0, 10, 0, 0], pred=bool, default=100)\n"
    "    10\n"
    "\n"
    "Given a `pred` it is also possible to look for the nth ``False`` value and \n"
    "return the result of ``pred(item)``::\n"
    "\n"
    "    >>> nth(1)([1,2,0], pred=bool)\n"
    "    2\n"
    "    >>> nth(-1)([1,0,2,0], pred=bool, truthy=False)\n"
    "    0\n"
    "    >>> import operator\n"
    "    >>> nth(-1)([[0,3], [0,1], [0,2]], pred=operator.itemgetter(1))\n"
    "    [0, 2]\n"
    "    >>> nth(-1)([[0,3], [0,1], [0,2]], pred=operator.itemgetter(1), retpred=True)\n"
    "    2\n"
    "\n"
    "There are already three predefined instances:\n"
    "\n"
    "- :py:func:`~iteration_utilities.first`: equivalent to ``nth(0)``.\n"
    "- :py:func:`~iteration_utilities.second`: equivalent to ``nth(1)``.\n"
    "- :py:func:`~iteration_utilities.third`: equivalent to ``nth(2)``.\n"
    "- :py:func:`~iteration_utilities.last`: equivalent to ``nth(-1)``.\n"
);

PyObject *
PyIUNth_New(Py_ssize_t index)
{
    PyIUObject_Nth *self;

    self = PyObject_GC_New(PyIUObject_Nth, &PyIUType_Nth);
    if (self == NULL) {
        return NULL;
    }
    self->index = index;
    PyObject_GC_Track(self);
    return (PyObject *)self;
}

/******************************************************************************
 * New
 *****************************************************************************/

static PyObject *
nth_new(PyTypeObject *type,
        PyObject *args,
        PyObject *kwargs)
{
    PyIUObject_Nth *self;

    Py_ssize_t index;

    /* Parse arguments */
    if (!PyArg_ParseTuple(args, "n:nth", &index)) {
        goto Fail;
    }

    /* Create struct */
    self = (PyIUObject_Nth *)type->tp_alloc(type, 0);
    if (self == NULL) {
        goto Fail;
    }
    self->index = index;
    return (PyObject *)self;

Fail:
    return NULL;
}

/******************************************************************************
 * Destructor
 *****************************************************************************/

static void
nth_dealloc(PyIUObject_Nth *self)
{
    PyObject_GC_UnTrack(self);
    Py_TYPE(self)->tp_free(self);
}

/******************************************************************************
 * Traverse
 *****************************************************************************/

static int
nth_traverse(PyIUObject_Nth *self,
             visitproc visit,
             void *arg)
{
    return 0;
}

/******************************************************************************
 * Clear
 *****************************************************************************/

static int
nth_clear(PyIUObject_Nth *self)
{
    return 0;
}

/******************************************************************************
 * Call
 *****************************************************************************/

static PyObject *
nth_call(PyIUObject_Nth *self,
         PyObject *args,
         PyObject *kwargs)
{
    static char *kwlist[] = {"iterable", "default", "pred", "truthy",
                             "retpred", "retidx", NULL};

    PyObject *iterable, *iterator, *item;
    PyObject *defaultitem=NULL, *func=NULL, *last=NULL, *val=NULL;
    int ok=0, truthy=1, retpred=0, retidx=0;
    Py_ssize_t idx, nfound=-1;

    /* Parse arguments */
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|OOiii:nth.__call__", kwlist,
                                     &iterable, &defaultitem, &func,
                                     &truthy, &retpred, &retidx)) {
        return NULL;
    }

    if (func == (PyObject *)&PyBool_Type) {
        func = Py_None;
    }

    if (retpred && retidx) {
        PyErr_SetString(PyExc_ValueError,
                        "can only specify `retpred` or `retidx` argument "
                        "for `nth`.");
        return NULL;
    }

    iterator = PyObject_GetIter(iterable);
    if (iterator == NULL) {
        return NULL;
    }

    /* The loop variable "idx" is only incremented if a suitable item was
       found.
       */
    for (idx=0 ; idx<=self->index || self->index < 0; ) {
        item = Py_TYPE(iterator)->tp_iternext(iterator);

        /* If the iterator terminates also terminate the loop and remove the
           last found item (except one looks for the last one
           "self->index == -1").
           */
        if (item == NULL) {
            if (self->index >= 0) {
                Py_XDECREF(last);
                last = NULL;
            }
            break;
        }

        /* In case the index of the found element should be returned we need to
           increment the "nfound" counter.
           */
        if (retidx) {
            nfound++;
        }

        /* If no function is given we can skip the remainder of the loop and
           just use the new item.
           */
        if (func == NULL) {
            if (last != NULL) {
                Py_DECREF(last);
            }
            last = item;
            idx++;
            continue;

        /* If "None" (or "bool") is given as predicate we don't need to call
           the function explicitly.
           */
        } else if (func == Py_None) {
            ok = PyObject_IsTrue(item);

        /* Otherwise call the function.  */
        } else {
            val = PyIU_CallWithOneArgument(func, item);
            if (val == NULL) {
                Py_DECREF(iterator);
                Py_DECREF(item);
                Py_XDECREF(last);
                return NULL;
            }
            ok = PyObject_IsTrue(val);
        }

        /* Compare if the "ok" variable matches the required "truthyness" and
           replace the last found item with the new found one.
           */
        if (ok == truthy) {
            /* If the predicate should be returned we don't need the original
               item but only keep the result of the function call.
               */
            if (retpred) {
                Py_DECREF(item);
                if (val == NULL) {
                    /* Predicate was None or bool and no "val" was created. */
                    val = PyBool_FromLong(ok);
                }
                Py_XDECREF(last);
                last = val;
                /* Set val to NULL otherwise the next iteration might decref
                   it inadvertently. */
                val = NULL;

            /* Otherwise discard the value from the function call and keep the
               item from the iterator.
               */
            } else {
                Py_XDECREF(val);
                if (last != NULL) {
                    Py_DECREF(last);
                }
                last = item;
            }
            idx++;

        /* Error happened when calling the function or when comparing to True. */
        } else if (ok < 0) {
            Py_DECREF(iterator);
            Py_DECREF(item);
            Py_XDECREF(val);
            return NULL;

        /* The object is not considered suitable and it will be discarded. */
        } else {
            Py_DECREF(item);
            Py_XDECREF(val);
        }
    }

    Py_DECREF(iterator);

    if (PyErr_Occurred()) {
        if (PyErr_ExceptionMatches(PyExc_StopIteration)) {
            PyErr_Clear();
        } else {
            Py_XDECREF(last);
            return NULL;
        }
    }

    /* We still have a last element (so the loop did not terminate without
       finding the indexed element).
       */
    if (last != NULL) {
        if (retidx) {
            Py_DECREF(last);
#if PY_MAJOR_VERSION == 2
            return PyInt_FromSsize_t(nfound);
#else
            return PyLong_FromSsize_t(nfound);
#endif
        }
        return last;

    /* No last element but a default was given. */
    } else if (defaultitem != NULL) {
        Py_INCREF(defaultitem);
        return defaultitem;

    /* No item, no default raises an IndexError.  */
    } else {
        PyErr_SetString(PyExc_IndexError,
                        "`iterable` for `nth` does not contain enough values.");
        return NULL;
    }
}

/******************************************************************************
 * Repr
 *****************************************************************************/

static PyObject *
nth_repr(PyIUObject_Nth *self)
{
    return PyUnicode_FromFormat("%s(%zd)",
                                Py_TYPE(self)->tp_name,
                                self->index);
}

/******************************************************************************
 * Reduce
 *****************************************************************************/

static PyObject *
nth_reduce(PyIUObject_Nth *self, PyObject *Py_UNUSED(args))
{
    return Py_BuildValue("O(n)", Py_TYPE(self),
                         self->index);
}

/******************************************************************************
 * Type
 *****************************************************************************/

static PyMethodDef nth_methods[] = {

    {"__reduce__",                                      /* ml_name */
     (PyCFunction)nth_reduce,                           /* ml_meth */
     METH_NOARGS,                                       /* ml_flags */
     PYIU_reduce_doc                                    /* ml_doc */
     },

    {NULL, NULL}                                        /* sentinel */
};

#define OFF(x) offsetof(PyIUObject_Nth, x)
static PyMemberDef nth_memberlist[] = {

    {"n",                                               /* name */
     T_PYSSIZET,                                        /* type */
     OFF(index),                                        /* offset */
     READONLY,                                          /* flags */
     nth_prop_n_doc                                     /* doc */
     },

    {NULL}                                              /* sentinel */
};
#undef OFF

PyTypeObject PyIUType_Nth = {
    PyVarObject_HEAD_INIT(NULL, 0)
    (const char *)"iteration_utilities.nth",            /* tp_name */
    (Py_ssize_t)sizeof(PyIUObject_Nth),                 /* tp_basicsize */
    (Py_ssize_t)0,                                      /* tp_itemsize */
    /* methods */
    (destructor)nth_dealloc,                            /* tp_dealloc */
    (printfunc)0,                                       /* tp_print */
    (getattrfunc)0,                                     /* tp_getattr */
    (setattrfunc)0,                                     /* tp_setattr */
    0,                                                  /* tp_reserved */
    (reprfunc)nth_repr,                                 /* tp_repr */
    (PyNumberMethods *)0,                               /* tp_as_number */
    (PySequenceMethods *)0,                             /* tp_as_sequence */
    (PyMappingMethods *)0,                              /* tp_as_mapping */
    (hashfunc)0,                                        /* tp_hash */
    (ternaryfunc)nth_call,                              /* tp_call */
    (reprfunc)0,                                        /* tp_str */
    (getattrofunc)PyObject_GenericGetAttr,              /* tp_getattro */
    (setattrofunc)0,                                    /* tp_setattro */
    (PyBufferProcs *)0,                                 /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC |
        Py_TPFLAGS_BASETYPE,                            /* tp_flags */
    (const char *)nth_doc,                              /* tp_doc */
    (traverseproc)nth_traverse,                         /* tp_traverse */
    (inquiry)nth_clear,                                 /* tp_clear */
    (richcmpfunc)0,                                     /* tp_richcompare */
    (Py_ssize_t)0,                                      /* tp_weaklistoffset */
    (getiterfunc)0,                                     /* tp_iter */
    (iternextfunc)0,                                    /* tp_iternext */
    nth_methods,                                        /* tp_methods */
    nth_memberlist,                                     /* tp_members */
    0,                                                  /* tp_getset */
    0,                                                  /* tp_base */
    0,                                                  /* tp_dict */
    (descrgetfunc)0,                                    /* tp_descr_get */
    (descrsetfunc)0,                                    /* tp_descr_set */
    (Py_ssize_t)0,                                      /* tp_dictoffset */
    (initproc)0,                                        /* tp_init */
    (allocfunc)0,                                       /* tp_alloc */
    (newfunc)nth_new,                                   /* tp_new */
    (freefunc)PyObject_GC_Del,                          /* tp_free */
};

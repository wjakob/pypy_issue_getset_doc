#include <Python.h>

typedef struct {
    PyObject_HEAD
} Test;

static int test_init(PyObject *self, PyObject *args, PyObject *kwd) { return 0; }
static PyObject *test_getter(PyObject *self, void* payload) { return PyLong_FromLong(123); }

static PyGetSetDef test_getset[] = {
    { "prop", test_getter, NULL, "A docstring", NULL },
    {NULL, NULL, NULL, NULL, NULL }
};

static PyType_Slot test_slots[] = {
    { Py_tp_init, test_init },
    { Py_tp_getset, test_getset },
    { 0, NULL }
};

static PyType_Spec test_spec = {
    .name = "Test",
    .flags = Py_TPFLAGS_DEFAULT,
    .slots = test_slots,
    .basicsize = (int) sizeof(Test),
    .itemsize = 0
};


// ----------------------------------------------------

static PyModuleDef pypy_issues_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "pypy_issues",
    .m_doc = "Reproducer for miscellaneous PyPy issues",
    .m_size = -1
};

PyMODINIT_FUNC
PyInit_pypy_issues(void)
{
    PyObject *m = PyModule_Create(&pypy_issues_module);
    if (m == NULL)
        return NULL;

    PyObject *test_o = PyType_FromSpec(&test_spec);

    if (!test_o || PyModule_AddObject(m, "Test", test_o) < 0) {
        Py_XDECREF(test_o);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}

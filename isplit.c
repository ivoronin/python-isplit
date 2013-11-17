#include <Python.h>

static PyObject * isplit_new(PyTypeObject *type, PyObject *self, PyObject *args);
static PyObject * isplit_next(PyObject *self);
static void isplit_dealloc(PyObject *self);

typedef struct {
    PyObject_HEAD
    char *orig, *string, *sep;
} isplit;

#if PY_MAJOR_VERSION >= 3
struct module_state {
    PyObject *error;
};

static struct PyModuleDef isplit_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "isplit",
};
#endif /* PY_MAJOR_VERSION >= 3 */

static PyTypeObject isplit_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "isplit.isplit",
    .tp_basicsize = sizeof(isplit),
    .tp_new = isplit_new,
    .tp_dealloc = (destructor)isplit_dealloc,
#if PY_MAJOR_VERSION < 3
    .tp_flags = Py_TPFLAGS_DEFAULT|Py_TPFLAGS_HAVE_ITER,
#endif
    .tp_iter = PyObject_SelfIter,
    .tp_iternext = (iternextfunc)isplit_next,
};

static void isplit_dealloc(PyObject *self)
{
    isplit *i = (isplit*)self;
    free(i->orig);
    free(i->sep);
    self->ob_type->tp_free(self);
}

static PyObject * isplit_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    const char *string;
    const char *sep;
    isplit *i;

    if (!PyArg_ParseTuple(args, "ss", &string, &sep))
        return NULL;

    i = (isplit*)type->tp_alloc(type, 0);
    if (!i)
        return NULL;

    i->string = strdup(string);
    i->orig = i->string;
    i->sep = strdup(sep);

    return (PyObject*)i;
}

static PyObject * isplit_next(PyObject *self)
{
    PyObject * word;
    char * found;
    isplit * i = (isplit*)self;

    if (!i->string)
        return NULL;

    found = strsep(&i->string, i->sep);
    word = Py_BuildValue("s", found);
    return (PyObject*)word;
}

#if PY_MAJOR_VERSION >= 3
PyObject * PyInit_isplit(void)
#else
PyMODINIT_FUNC initisplit(void)
#endif
{
    PyObject * m;
    
    if (PyType_Ready(&isplit_type) < 0)
#if PY_MAJOR_VERSION >= 3
        return NULL;
#else
        return;
#endif

#if PY_MAJOR_VERSION >= 3
    m = PyModule_Create(&isplit_module);
#else
    m = Py_InitModule("isplit", NULL);
#endif

    PyModule_AddObject(m, "isplit", (PyObject*)&isplit_type);

#if PY_MAJOR_VERSION >= 3
    return m;
#endif
}
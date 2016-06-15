# Techno

This tiny C extension tends to be a simple "code injection" example. It substitutes CPython's internal function `PyRun_InteractiveOneObject` that incapsulates all the REPL machinery with
```c
int
my_function(FILE *fp, PyObject *filename, PyCompilerFlags *flags)
{
    printf("COUNTER: %d\n", counter);
    counter++;
    return PyRun_InteractiveOneObject(fp, filename, flags);
}
```
In order to install it, you will need "injection friendly" version of CPython interpreter, just build [this commit]( https://github.com/magniff/cpython/commit/08590300b43242a88cb8000b98f3e4c256da4d15)

## How to run, you might ask
Ones you have built CPython from commit above, you may install the extension as usual:
```bash
python setup.py install
```
then run your REPL:
```python
>>> import techno
>>> techno.init()  # pass REPL to my_function, press ENTER several times for example
COUNTER: 0
>>> 
COUNTER: 1
>>> 
COUNTER: 2
>>> print("hello world")
hello world
COUNTER: 3
>>> techno.reset()  # restore old behavior
>>> 
>>> 
```

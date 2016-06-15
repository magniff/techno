# Techno

This tiny C extension tends to be a simple "code injection" example. It introduces a frame counter, that become available in REPL.
In order to install it, you will need a "code injection friendly" version of CPython interpreter, just build [7768bbc6e2769db9e8861ffc269b72f3709a5b11](https://github.com/magniff/cpython/tree/7768bbc6e2769db9e8861ffc269b72f3709a5b11)

## How to run, you might ask
Ones you have built CPython from commit above, you may install the extension as usual:
```bash
python setup.py install
```
then run your REPL:
```python
>>> import techno
>>> techno.init()  # hacks interpreter internals
>>>
Frames evaluated: 1  # even though the input is empty there still one empty frame to evaluate
>>> 
Frames evaluated: 1
>>> import re
Frames evaluated: 1
>>> import code  # some module, that not been cached yet
Frames evaluated: 5301
>>> import code  # cached, so no need to invoke import machinery this time
Frames evaluated: 1
>>> def factorial(value):
...    return 1 if not value else factorial(value-1)*value
... 
Frames evaluated: 1
>>> factorial(10)
3628800
Frames evaluated: 12
>>> class A(): pass  # class contruction actually evaluates code inside
... 
Frames evaluated: 2
>>> a = A()  # A.__new__ defined in C, so there is nothing pythonish to eval
Frames evaluated: 1
>>> some_gen = (value**2 for value in range(10))
Frames evaluated: 1
>>> list(some_gen)  # generator interactions require python frames evaluation, see genobject.c
[0, 1, 4, 9, 16, 25, 36, 49, 64, 81]
Frames evaluated: 12
>>> with open("/dev/null", "w") as devnull:  # print defined in C
...    for value in range(10000):
...       print(value, file=devnull)
... 
Frames evaluated: 3
>>> ^C # Nothing to evaluate here, duuuuh
KeyboardInterrupt
Frames evaluated: 0
>>> hello world  # couldnt parse, nothing to run
  File "<stdin>", line 1
    hello world
              ^
SyntaxError: invalid syntax
Frames evaluated: 0
>>> import not_a_module  # import hook machinery, all finders from sys.meta_path been invoked
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ImportError: No module named 'not_a_module'
Frames evaluated: 119
>>> techno.reset()  # back to normal
Frames evaluated: 1
>>> 
>>> print("hello world")  # frame printer disabled, back to real life
hello world
>>> 
```

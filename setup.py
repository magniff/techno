from distutils.core import setup, Extension


techno_module = Extension('techno', sources=['techno.c'])


setup(
    name='techno',
    author='magniff',
    version='0.0.1',
    description='Low level tinkering utils.',
    ext_modules=[techno_module]
)

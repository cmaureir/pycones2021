from setuptools import setup, Extension

setup(name='pycones', version='1.0',
      ext_modules=[Extension('pycones', ['simple.c'])])

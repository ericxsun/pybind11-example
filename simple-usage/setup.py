#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright @2019 R&D, NTC Inc. (ntc.ai)
#
# Author: qinluo <eric.x.sun@gmail.com>
#

import os
import re
import sys

import setuptools
from setuptools import Extension
from setuptools.command.build_ext import build_ext


def has_flag(compiler, flag_name):
    """Return a boolean indicating whether a flag name is supported on the specified compiler.
    """
    import tempfile
    with tempfile.NamedTemporaryFile('w', suffix='.cpp') as fp:
        fp.write('int main (int argc, char **argv) { return 0; }')
        try:
            compiler.compile([fp.name], extra_postargs=[flag_name])
        except setuptools.distutils.errors.CompileError:  # pylint: disable=no-member
            return False

    return True


def cpp_flag(compiler):
    """Return the -std=c++[11/14] compiler flag.
    The c++14 is preferred over c++11 (when it is available).
    """
    if has_flag(compiler, '-std=c++14'):
        return '-std=c++14'

    if has_flag(compiler, '-std=c++11'):
        return '-std=c++11'

    raise RuntimeError('Unsupported compiler -- at least C++11 support is needed!')


class GetPybindInclude(object):  # pylint: disable=useless-object-inheritance, too-few-public-methods
    """Helper class to determine the pybind11 include path
    The purpose of this class is to postpone importing pybind11
    until it is actually installed, so that the ``get_include()``
    method can be invoked.
    """

    def __init__(self, user=False):
        self.user = user

    def __str__(self):
        import pybind11
        return pybind11.get_include(self.user)


class BuildExt(build_ext):
    """A custom build extension for adding compiler-specific options."""
    c_opts = {
        'msvc': ['/EHsc'],
        'unix': [],
    }

    if sys.platform == 'darwin':
        c_opts['unix'] += ['-stdlib=libc++', '-mmacosx-version-min=10.7']

    def build_extensions(self):
        compiler_type = self.compiler.compiler_type
        opts = self.c_opts.get(compiler_type, [])
        if compiler_type == 'unix':
            opts.append('-DVERSION_INFO="%s"' % self.distribution.get_version())
            opts.append(cpp_flag(self.compiler))
            if has_flag(self.compiler, '-fvisibility=hidden'):
                opts.append('-fvisibility=hidden')
        elif compiler_type == 'msvc':
            opts.append('/DVERSION_INFO="%s"' % self.distribution.get_version())
        for ext in self.extensions:
            ext.extra_compile_args = opts

        build_ext.build_extensions(self)


SRC = 'src'

r_cc = re.compile(r'\.(cpp|c|cc)$')
src_files = [os.path.join(root, _file) for root, _, files in os.walk(SRC) for _file in files if files]
src_files = map(str, src_files)
src_cc = [x for x in src_files if r_cc.findall(x)]

ext_modules = [
    Extension(
        'simpleusage_pybind',
        ['python/simpleusage/pybind/simpleusage_pybind.cc'] + src_cc,
        include_dirs=[
            # Path to pybind11 headers
            GetPybindInclude(),
            SRC
        ],
        language='c++',
        extra_compile_args=['-O3 -funroll-loops -pthread -march=native']
    ),
]

setuptools.setup(
    name='simpleusage',
    version='0.1',
    author='qinluo',
    author_email='eric.x.sun@gmail.com',
    description='simple usage of pybind11',
    ext_modules=ext_modules,
    install_requires=['pybind11>=2.2', 'setuptools >= 0.7.0'],
    cmdclass={'build_ext': BuildExt},
    packages=[str('simpleusage'), str('simpleusage.tests')],
    package_dir={str(''): str('python')},
    zip_safe=False
)

from distutils import sysconfig
from distutils.command.build_ext import build_ext as build_ext_orig
import os
import pathlib
from distutils.sysconfig import get_python_inc

from setuptools import setup, Extension

class CMakeExtension(Extension):

    def __init__(self, name):
        # don't invoke the original build_ext for this special extension
        super().__init__(name, sources=[])


class build_ext(build_ext_orig):

    def run(self):
        for ext in self.extensions:
            self.build_cmake(ext)
        super().run()

    def build_cmake(self, ext):
        cwd = pathlib.Path().absolute()

        # these dirs will be created in build_py, so if you don't have
        # any python sources to bundle, the dirs will be missing
        build_temp = pathlib.Path(self.build_temp)
        build_temp.mkdir(parents=True, exist_ok=True)
        extdir = pathlib.Path(self.get_ext_fullpath(ext.name))
        extdir.mkdir(parents=True, exist_ok=True)

        # example of cmake args
        config = 'Debug' if self.debug else 'Release'
        cmake_args = [
            # '-DCMAKE_CXX_COMPILER=/c/msys64/mingw64/bin/cc.exe',
            # '-DPYTHON_INCLUDE_DIR='+get_python_inc(),
            # '-DPYTHON_LIBRARY=' + sysconfig.get_config_var('LIBDIR'),
            '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + str(extdir.parent.absolute()),
            '-DCMAKE_BUILD_TYPE=' + config
        ]

        # example of build args
        build_args = [
            '--config', config,
            '--', '-j4'
        ]

        os.chdir(str(build_temp))
        self.spawn(['cmake', str(cwd)] + cmake_args)
        if not self.dry_run:
            self.spawn(['cmake.exe', '--build', '.'] + build_args)
        os.chdir(str(cwd))


setup(
    name='PyMusic',
    version='0.1',
    packages={'pymusic'},
    ext_modules=[CMakeExtension('pymidi')],
    cmdclass={
        'build_ext': build_ext,
    },
    entry_points={
        'console_scripts': [
            'pymusic = pymusic.test'
        ]
    }
)
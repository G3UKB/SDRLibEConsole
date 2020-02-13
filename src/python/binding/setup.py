from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

extensions = [
    Extension("libserver", ["server.pyx"],
        library_dirs=["E:\\Projects\\SDRLibE\\trunk\\server\\x64\\Release",
                      "E:\\Projects\\SDRLibE\\trunk\\wdsp_win\\x64\\Release",
                      "E:\\Projects\\SDRLibE\\libs\\fftw\\x64",
                      "E:\\Projects\\SDRLibE\\libs\\pthreads\\lib\\x64",
                      "E:\\Projects\\SDRLibE\\libs\\portaudio\\x64"],
        libraries=["Ws2_32", "libfftw3-3", "pthreadVC2", "portaudio_x64", "wdsp_win", "server"]), 
]

setup(name='SDRLibE Bindings',
      ext_modules=cythonize(extensions))
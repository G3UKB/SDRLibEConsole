# server.pyx

cimport server_api as api

cdef class Server:
    
    def __cinit__(self):
        api.c_server_init()
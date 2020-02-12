# server.pyx

"""
    This module wraps the SDRLibE server lib.
    it is a straight mapping, there is no logic other than marshalling.
"""

# Import the API defs
cimport server_api as api

# ===================================================
# Server functions
cdef class Server:

    # ----------------------------
    # Constructor
    def __cinit__(self):
        pass

    # ----------------------------
    # Server initialisation
    def cinit(self):
        api.c_server_init()
        
# ===================================================
# Audio functions
cdef class Audio:

    # ----------------------------
    # Constructor
    def __cinit__(self):
        pass
    
    # ----------------------------
    # Device enumerations
    def enum_inputs(self):
        """ Enum compatible input devices """
        
        devs = api.c_server_enum_audio_inputs()
        return __convert_device_list(devs)

    def enum_outputs(self):
        """ Enum compatible output devices """
        
        devs = api.c_server_enum_audio_outputs()
        return __convert_device_list(devs)

# ===================================================
# Radio functions
cdef class Radio:

    # ----------------------------
    # Constructor
    def __cinit__(self):
        pass
    

#=======================================================================================	
# These are functions with C types. They can be called internally in Cython but cannot be exposed.

# ----------------------------
# Convert an audio device list into a python array of entries
cdef __convert_device_list(api.DeviceEnumList* devs):
    device_list = []
    for n in range(devs.entries):
        if devs.devices[n].name.decode("ascii").lower().find('sound mapper') == -1:
            device_list.append({'index':devs.devices[n].index, 'name':devs.devices[n].name.decode("ascii"), 'ch':devs.devices[n].channels, 'hostapi':devs.devices[n].host_api.decode("ascii")})
    return device_list
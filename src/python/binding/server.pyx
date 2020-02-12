# server.pyx

cimport server_api as api

cdef class Server:
    
    def __cinit__(self):
        pass

    # ===================================================
    # Initialisation
    def cinit(self):
        api.c_server_init()
        
    # ===================================================
    # Audio device enumeration
    def enum_inputs(self):
        """ Enum compatible input devices """
        
        devs = api.c_server_enum_audio_inputs()
        return __convert_device_list(devs)

    def enum_outputs(self):
        """ Enum compatible output devices """
        
        devs = api.c_server_enum_audio_outputs()
        return __convert_device_list(devs)

#=======================================================================================	
# These are functions with C types. They can be called internally in Cython but cannot be exposed.
cdef __convert_device_list(api.DeviceEnumList* devs):
    """ Convert from DeviceEnumList to Python dict """
    
    device_list = []
    for n in range(devs.entries):
        if devs.devices[n].name.decode("ascii").lower().find('sound mapper') == -1:
            device_list.append({'index':devs.devices[n].index, 'name':devs.devices[n].name.decode("ascii"), 'ch':devs.devices[n].channels, 'hostapi':devs.devices[n].host_api.decode("ascii")})
    return device_list
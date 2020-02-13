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
    
    # ----------------------------
    # Start server
    def server_start(self):
        return api.c_server_start()
    
    # ----------------------------
    # Terminate server
    def server_terminate(self):
        api.c_server_terminate()
        
# ===================================================
# Audio functions
cdef class Audio:

    # ----------------------------
    # Constructor
    def __cinit__(self):
        pass
    
    # ----------------------------
    # Device enumerations
    # Inputs
    def enum_inputs(self):
        devs = api.c_server_enum_audio_inputs()
        return __convert_device_list(devs)
    
    # Outputs
    def enum_outputs(self):
        devs = api.c_server_enum_audio_outputs()
        return __convert_device_list(devs)

    # ----------------------------
    # Set a new audio route
    def server_set_audio_route(self, direction, location, receiver, host_api, dev, channel):
        api.c_server_set_audio_route(direction, location, receiver, host_api, dev, channel)
    
    # ----------------------------
    # Clear all routes    
    def server_clear_audio_routes():
        api.c_server_clear_audio_routes()
    
    # ----------------------------
    # Restart audio routes after cleas and set commands    
    def server_restart_audio_routes():
        api.c_server_restart_audio_routes()
    
    # ----------------------------
    # Change audio outputs for default routs (usually temporarily)    
    def server_change_audio_outputs(self, rx, audio_ch):
        api.c_server_change_audio_outputs(rx, audio_ch)
    
    # ----------------------------
    # Revert to configured routes  
    def server_revert_audio_outputs():
        api.c_server_revert_audio_outputs()
    
    # ----------------------------
    # Run local audio services   
    def server_local_audio_run(self runstate):
        api.c_server_local_audio_run(runstate)
    
# ===================================================
# Radio functions
cdef class Radio:

    # ----------------------------
    # Constructor
    def __cinit__(self):
        pass
    
    #=============================
    # Global functions
    # ----------------------------
    # Discover hardware    
    def radio_discover(self):
        return api.c_radio_discover()
    
    # ----------------------------
    # Start radio streaming   
    def radio_start(self, wbs):
        return api.c_radio_start(wbs)
    
    # ----------------------------
    # Stop radio streaming  
    def radio_stop(self):
        return api.c_radio_stop()
 
# ===================================================
# DSP functions
cdef class DSP:
   
    # ----------------------------
    # Constructor
    def __cinit__(self):
        pass
        
    #=============================
    # RX functions
    # ----------------------------
    # New RX mode
    def server_set_rx_mode(self, channel, mode):
        api.c_server_set_rx_mode(int channel, int mode)
        
    # ----------------------------
    # Filter run with settings   
    def server_set_rx_filter_run(self, channel, run):
        api.c_server_set_rx_filter_run(channel, run)
    
    # ----------------------------
    # New low, high frequency filter 
    def server_set_rx_filter_freq(self, channel, low, high):
        api.c_server_set_rx_filter_freq(channel, low, high)
    
    # ----------------------------
    # Set windowing for filter    
    def server_set_rx_filter_window(self, channel, window):
        api.c_server_set_rx_filter_window(channel, window)
    
    # ----------------------------
    # New AGC mode   
    def server_set_agc_mode(self, channel, mode):
        api.c_server_set_agc_mode(channel, mode)
    
    # ----------------------------
    # New audio gain    
    def server_set_rx_gain(self, rx, gain):
        api.c_server_set_rx_gain(rx, gain)
    
    # ----------------------------
    # Get last RX meter data for given type    
    def server_get_rx_meter_data(self, channel, which):
        api.c_server_get_rx_meter_data(channel, which)
    
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
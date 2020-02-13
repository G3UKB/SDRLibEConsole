# server.pyx

"""
    This module wraps the SDRLibE server lib.
    it is a straight mapping, there is no logic other than marshalling.
"""

# Import the API defs
cimport server_api as api

# Numpy imports
import numpy as np

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
        return api.c_server_init()
    
    # ----------------------------
    # Start server
    def server_start(self):
        return api.c_server_start()
    
    # ----------------------------
    # Terminate server
    def server_terminate(self):
        return api.c_server_terminate()
    
    #=============================
    # Param update functions
    def server_set_in_rate(self, rate):
        api.c_server_set_in_rate(rate)
        
    def server_set_out_rate(self, rate):
        api.c_server_set_out_rate(rate)
        
    def server_set_iq_blk_sz(self, blk_sz):
        api.c_server_set_iq_blk_sz(blk_sz)
        
    def server_set_mic_blk_sz(self, blk_sz):
        api.c_server_set_mic_blk_sz(blk_sz)
        
    def server_set_duplex(self, duplex):
        api.c_server_set_duplex(duplex)
        
    def server_set_fft_size(self, size):
        api.c_server_set_fft_size(size)
        
    def server_set_window_type(self, window_type):
        api.c_server_set_window_type(window_type)
        
    def server_set_av_mode(self, mode):
        api.c_server_set_av_mode(mode)
    
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
    def server_clear_audio_routes(self):
        return api.c_server_clear_audio_routes()
    
    # ----------------------------
    # Restart audio routes after cleas and set commands    
    def server_restart_audio_routes(self):
        return api.c_server_restart_audio_routes()
    
    # ----------------------------
    # Change audio outputs for default routs (usually temporarily)    
    def server_change_audio_outputs(self, rx, audio_ch):
        api.c_server_change_audio_outputs(rx, audio_ch)
    
    # ----------------------------
    # Revert to configured routes  
    def server_revert_audio_outputs(self):
        api.c_server_revert_audio_outputs()
    
    # ----------------------------
    # Run local audio services   
    def server_local_audio_run(self, runstate):
        return api.c_server_local_audio_run(runstate)
    
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
    
    #=============================
    # Command functions
    # ----------------------------
    # Set frequency
    def server_cc_out_set_rx_1_freq(self, freq_in_hz):
        api.c_server_cc_out_set_rx_1_freq(freq_in_hz)
    def server_cc_out_set_rx_2_freq(self, freq_in_hz):
        api.c_server_cc_out_set_rx_2_freq(freq_in_hz)
    def server_cc_out_set_rx_3_freq(self, freq_in_hz):
        api.c_server_cc_out_set_rx_3_freq(freq_in_hz)
    def server_cc_out_set_tx_freq(self, freq_in_hz):
        api.c_server_cc_out_set_tx_freq(freq_in_hz)
    
    # ----------------------------
    # Set duplex mode   
    def server_cc_out_duplex(self, duplex):
        api.c_server_cc_out_duplex(duplex)
    
    # ----------------------------
    # Set HF preamp   
    def server_cc_out_preamp(self, preamp):
        api.c_server_cc_out_preamp(preamp)
    
    #=============================
    # Alex filter functions
    def server_cc_out_alex_attn(self, attn):
        api.c_server_cc_out_alex_attn(attn)
        
    def server_cc_out_alex_rx_ant(self, ant):
        api.c_server_cc_out_alex_rx_ant(ant)
        
    def server_cc_out_alex_rx_out(self, out):
        api.c_server_cc_out_alex_rx_out(out)
        
    def server_cc_out_alex_tx_rly(self, rly):
        api.c_server_cc_out_alex_tx_rly(rly)
        
    def server_cc_out_alex_auto(self, state):
        api.c_server_cc_out_alex_auto(state)
        
    def server_cc_out_alex_hpf_bypass(self, bypass):
        api.c_server_cc_out_alex_hpf_bypass(bypass)
        
    def server_cc_out_lpf_30_20(self, setting):
        api.c_server_cc_out_lpf_30_20(setting)
        
    def server_cc_out_lpf_60_40(self, setting):
        api.c_server_cc_out_lpf_60_40(setting)
        
    def server_cc_out_lpf_80(self, setting):
        api.c_server_cc_out_lpf_80(setting)
        
    def server_cc_out_lpf_160(self, setting):
        api.c_server_cc_out_lpf_160(setting)
        
    def server_cc_out_lpf_6(self, setting):
        api.c_server_cc_out_lpf_6(setting)
        
    def server_cc_out_lpf_12_10(self, setting):
        api.c_server_cc_out_lpf_12_10(setting)
        
    def server_cc_out_lpf_17_15(self, setting):
        api.c_server_cc_out_lpf_17_15(setting)
        
    def server_cc_out_hpf_13(self, setting):
        api.c_server_cc_out_hpf_13(setting)
        
    def server_cc_out_hpf_20(self, setting):
        api.c_server_cc_out_hpf_20(setting)
        
    def server_cc_out_hpf_9_5(self, setting):
        api.c_server_cc_out_hpf_9_5(setting)
        
    def server_cc_out_hpf_6_5(self, setting):
        api.c_server_cc_out_hpf_6_5(setting)
        
    def server_cc_out_hpf_1_5(self, setting):
        api.c_server_cc_out_hpf_1_5(setting)
    
# ===================================================
# DSP functions
cdef class DSP:
   
    # ----------------------------
    # Constructor
    def __cinit__(self):
        pass
    
    # ----------------------------
    # Make wisdom file
    def server_make_wisdom(self, dir):
        api.c_server_make_wisdom(dir)
    
    #=============================
    # RX functions
    # ----------------------------
    # New RX mode
    def server_set_rx_mode(self, channel, mode):
        api.c_server_set_rx_mode(channel, mode)
        
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
        return api.c_server_get_rx_meter_data(channel, which)

    #=============================
    # TX functions
    # ----------------------------
    def server_set_tx_mode(self, channel, mode):
        api.c_server_set_tx_mode(channel, mode)
        
    def server_set_tx_filter_run(self, channel, run):
        api.c_server_set_tx_filter_run(channel, run)
        
    def server_set_tx_filter_freq(self, channel, low, high):
        api.c_server_set_tx_filter_freq(channel, low, high)
        
    def server_set_tx_filter_window(self, channel, window):
        api.c_server_set_tx_filter_window(channel, window)
        
    def server_get_tx_meter_data(self, channel, which):
        api.c_server_get_tx_meter_data(channel, which)
        
    def server_set_mic_gain(self, gain):
        api.c_server_set_mic_gain(gain)
        
    def server_set_rf_drive(self, drive):
        api.c_server_set_rf_drive(drive)
        
    def server_get_peak_input_level(self):
        api.c_server_get_peak_input_level()
    
# ===================================================
# Display functions
cdef class Display:
   
    # ----------------------------
    # Constructor
    def __cinit__(self):
        self.__display_data = np.zeros(1920, dtype=np.float)
        self.__wbs_data = np.zeros(1920, dtype=np.float)
    
    # ----------------------------
    # Change display width
    def server_set_display_width(self, width) :
        api.c_server_set_display_width(width) 

    # ----------------------------
    # Set display for channel
    def server_set_display(self, ch_id, display_width):
        api.c_server_set_display(ch_id, display_width)
    
    # ----------------------------
    # Get spectrum display data    
    def server_get_display_data(self, display_id, display_data):
        # a memview can be used to pass in a pointer
        cdef float[::1] arr_memview = self.__display_data
        if api.c_server_get_display_data(display_id, &arr_memview[0]):
            return [True, self.__display_data]
        else:
            return [False, None]
          
    # ----------------------------
    # Process WBS frame    
    def server_process_wbs_frame(self, ptr_in_bytes):
        api.c_server_process_wbs_frame(ptr_in_bytes)
    
    # ----------------------------
    # Get WBS data    
    def server_get_wbs_data(self, width, wbs_data):
        # a memview can be used to pass in a pointer
        cdef float[::1] arr_memview = self.__wbs_data
        if api.c_server_get_wbs_data(width, &arr_memview[0]):
            return [True, self.__wbs_data]
        else:
            return [False, None] 
    
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
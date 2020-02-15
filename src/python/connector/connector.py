#!/usr/bin/env python
#
# connector.py
#
# Client connector to server
# 
# Copyright (C) 2020 by G3UKB Bob Cowdery
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#    
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#    
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#    
#  The author can be reached by email at:   
#     bob@bobcowdery.plus.com
#

# Import all
from main.imports import *

#==============================================================================================
# The connector drives the server lib extension where there is any additional logic over
# and above straight calls.
#==============================================================================================

    
#=====================================================
# Interface class to SDRLibEConnector -> server -> HPSDR
# Sends messages to SDRLibEConnector and receives responses
#===================================================== 
class Connector: 
    #-------------------------------------------------
    # Initialisation
    def __init__(self) :
        # Get server instances
        self.__server = getInstance("server_inst")
        self.__audio = getInstance('audio_inst')
        self.__radio = getInstance('radio_inst')
        self.__dsp = getInstance('dsp_inst')
        self.__display = getInstance('display_inst')
        # Initial state
        self.__last_state = {
            1: {
                "mode": CH_LSB,
                "filter": CH_2K7
                },
            2: {
                "mode": CH_LSB,
                "filter": CH_2K7
                },
            3: {
                "mode": CH_LSB,
                "filter": CH_2K7
                },
        }

    #==============================================================================================
    # PUBLIC
    #==============================================================================================
    
    #-------------------------------------------------
    def enum_audio_inputs(self):
        return self.__audio.enum_inputs()

    #-------------------------------------------------
    def enum_audio_outputs(self):
        return self.__audio.enum_outputs()
    
    #-------------------------------------------------
    def set_audio_routes(self, reset):
        # Set all server audio route
        if reset:
            # First clear down existing routes
            if not self.__audio.server_clear_audio_routes():
                print("Failed to clear audio route(s)!")
                return False
        
        # Set the new route for all radios
        # Get radio model audio sections
        self.__radio_1_audio_model = Model.get_radio_model()[1]['AUDIO']
        self.__radio_2_audio_model = Model.get_radio_model()[2]['AUDIO']
        self.__radio_3_audio_model = Model.get_radio_model()[3]['AUDIO']
        # Set routes
        self.__set_route(RX_1, self.__radio_1_audio_model['SINK'], self.__radio_1_audio_model['DEV'], self.__radio_1_audio_model['CH'])
        self.__set_route(RX_2, self.__radio_2_audio_model['SINK'], self.__radio_2_audio_model['DEV'], self.__radio_2_audio_model['CH'])
        self.__set_route(RX_3, self.__radio_3_audio_model['SINK'], self.__radio_3_audio_model['DEV'], self.__radio_3_audio_model['CH'])
        print("All audio routes set")
        
        if reset:    
            # Restart audio
            if not self.__audio.server_restart_audio_routes():
                 print("Failed to restart audio!")
                 return False
        return True
    
    #-------------------------------------------------
    # Server cold start
    def coldstart(self):
        # Start the server
        success = False
        state = Model.get_state_model()
        
        # Initialise server
        if self.__server.cinit():
            state['HAVE-SERVER'] = True
            # Discover radio
            if self.__radio.radio_discover():
                state['DISCOVER'] = True
                # Set to configured number of receivers
                self.__server.server_set_num_rx(Model.get_num_rx())
                # Set all audio routes
                if self.set_audio_routes(False):
                    # Start server
                    if self.__server.server_start():
                        state['SERVER-RUN'] = True
                        success = True
                    else: 
                        print("Sorry, failed to start server, unable to continue!")
                        sys.exit()
                else:
                    print("Sorry, failed to set audio routes!")
            else:
                print("Sorry, radio discover failed!")
        else:
            print("Sorry, failed to initialise server, unable to continue!")
            sys.exit()
            
        return success
      
    #-------------------------------------------------
    # Server warm start
    def warmstart(self):
        success = True
        state = Model.get_state_model()
        if not state['DISCOVER']:
            if not self.__radio.radio_discover():
                print("No radio hardware detected! Please power-up the radio and try again.")
                success = False
            else:
                state['DISCOVER'] = True
                # Set to configured number of receivers
                self.__server.server_set_num_rx(Model.get_num_rx())
                # Set all audio routes
                if self.set_audio_routes(True):
                    # Start server
                    if not self.__server.server_start():
                        print("Sorry, failed to start server, unable to continue!")
                        sys.exit()
                    else:
                        state['SERVER-RUN'] = True
                else:
                    print("Sorry, failed to set audio routes!")
                    success = False
        if success:
            # Good to go
            # Start radio
            if self.__radio.radio_start(False):
                state['RADIO-RUN'] = True
            else:
                print("Sorry, failed to start radio!")
                state['RADIO-RUN'] = False
            
        return success
    
    #-------------------------------------------------
    # Stop radio
    def stop_radio(self):
        return self.__radio.radio_stop()

    #-------------------------------------------------
    # Bounce the server
    def restart(self):
        
        if self.__server.server_terminate():
            print("Terminating server...")
            sleep(2)
            print("Restarting server...")
            return self.coldstart()
    
    #-------------------------------------------------
    # Terminate the server
    def terminate(self):
        
        if not self.__server.server_terminate():
            print("Failed to terminate server!")
     
    #-------------------------------------------------
    # Set frequency
    def set_freq(self, id, freq):
        if id == 1:
            self.__radio.server_cc_out_set_rx_1_freq(freq)
        elif id == 2:
            self.__radio.server_cc_out_set_rx_2_freq(freq)
        else:
            self.__radio.server_cc_out_set_rx_3_freq(freq)
     
    #-------------------------------------------------
    # Set RX mode 
    def set_rx_mode(self, rx_id, new_mode_id):
        self.__set_mode_filter(rx_id, new_mode_id, self.__last_state[rx_id]["filter"])
    
    #-------------------------------------------------
    # Set RX filter 
    def set_rx_filter(self, rx_id, filter_id):
        self.__set_mode_filter(rx_id, self.__last_state[rx_id]["mode"], filter_id)
    
    #-------------------------------------------------
    # Set RX AGC 
    def set_agc(self, rx_id, agc_id):
        self.__dsp.server_set_agc_mode(rx_id-1, agc_id)
    
    #-------------------------------------------------
    # Set display 
    def set_display(self, rx_id, width):
        self.__display.server_set_display(rx_id-1, width)
    
    #-------------------------------------------------
    # Set display width
    def set_display_width(self, rx_id, width):
        self.__display.server_set_display_width(rx_id-1, width)

    #==============================================================================================
    # PRIVATE
    #==============================================================================================
    #-------------------------------------------------
    # Set one audio route
    def __set_route(self, radio, sink, dev, ch):
        
        if dev != NONE and ch != NONE:
            (api, dev) = dev.split('@')
            self.__audio.server_set_audio_route(DIR_OUTPUT, sink.encode('utf-8'), radio, api.encode('utf-8'), dev.encode('utf-8'), ch.encode('utf-8'))
    
    #-------------------------------------------------
    # Set mode and filter together as they inter-relate
    def __set_mode_filter(self, rx_id, mode_id, filter_id):
        # Get filter values
        low = filter_lookup[filter_id][1]
        high = filter_lookup[filter_id][2]
        if mode_id == CH_LSB or mode_id == CH_CWL or mode_id == CH_DIGL:
            # We are on the left side of the spectrum so reverse the and negate the filter
            new_low = -high
            new_high = - low
        elif mode_id == CH_DSB or mode_id == CH_FM or mode_id == CH_AM or mode_id == CH_SPEC or mode_id == CH_SAM or mode_id == CH_DRM:
            # Mode centered on 0
            new_low = -high
            new_high = high
        else:
            #we are on the right side and the values are correct
            new_low = low
            new_high = high
            
        # Set the radio mode and filter
        self.__dsp.server_set_rx_mode(rx_id-1, mode_id)
        self.__dsp.server_set_rx_filter_freq(rx_id-1, new_low, new_high)
            
        # Update the state
        self.__last_state[rx_id]["mode"] = mode_id
        self.__last_state[rx_id]["filter"] = filter_id
                     
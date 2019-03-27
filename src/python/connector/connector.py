#!/usr/bin/env python
#
# connector.py
#
# Client connector to SDRLibEConnector server
# 
# Copyright (C) 2019 by G3UKB Bob Cowdery
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
# The connector drives the SDRLibE/connector executable via a JSON UDP interface. This enables
# distribution if required and also makes it a managed server which will execute on its own
# once its setup is complete which could be a command line client or a GUI client.
#==============================================================================================

#=====================================================
# Global message name management
# i.e. use M_ID.POLL as a parameter to this module
#=====================================================

#-------------------------------------------------
# Message name enumerations
class M_ID(Enum):
    #---------------
    # Response types
    ACK_NAK = auto(),
    DATA = auto(),
    #---------------
    # Control commands
    POLL = auto(),
    DISCOVER = auto(),
    SVR_START = auto(),
    SVR_STOP = auto(),
    RADIO_START = auto(),
    RADIO_STOP = auto(),
    NUM_RX = auto(),
    #---------------
    # Radio commands
    HF_PRE = auto(),
    ATTN = auto(),
    R1_FREQ = auto(),
    R2_FREQ = auto(),
    R3_FREQ = auto(),
    R1_MODE = auto(),
    R2_MODE = auto(),
    R3_MODE = auto(),
    R1_FILT = auto(),
    R2_FILT = auto(),
    R3_FILT = auto(),
    TX_FILT = auto(),
    R1_AGC = auto(),
    R2_AGC = auto(),
    R3_AGC = auto(),
    R1_GAIN = auto(),
    R2_GAIN = auto(),
    R3_GAIN = auto(),
    #---------------
    # Alex filters
    ALEX_AUTO = auto(),
    HF_BYPASS = auto(),
    LPF_160 = auto(),
    LPF_80 = auto(),
    LPF_60_40 = auto(),
    LPF_30_20 = auto(),
    LPF_17_15 = auto(),
    LPF_12_10 = auto()
    LPF_6 = auto(),
    HPF_1_5 = auto(),
    HPF_6_5 = auto(),
    HPF_9_5 = auto(),
    HPF_13 = auto(),
    HPF_20 = auto(),
    #---------------
    # Audio
    ENUM_INPUTS = auto(),
    ENUM_OUTPUTS = auto(),
    REVERT_OUTPUTS = auto(),
    CHANGE_OUTPUTS = auto(),
    AUDIO_ROUTE = auto(),
    LOCAL_AUDIO_RUN = auto(),
    CLEAR_AUDIO_ROUTES = auto(),
    RESTART_AUDIO_ROUTES = auto(),
    #---------------
    # Display
    DISP_WIDTH = auto(),
    DISP_PERIOD = auto(),
    DISP_STATE = auto(),

#-------------------------------------------------
# Message name lookup    
class M_LU:   
    @staticmethod
    def lookup(msg_id):
        t = {
            # ID : (name, response-type)
            #---------------
            # Control commands
            M_ID.POLL : ("poll", M_ID.ACK_NAK),
            M_ID.DISCOVER : ("radio_discover", M_ID.ACK_NAK),
            M_ID.SVR_START : ("server_start", M_ID.ACK_NAK),
            M_ID.SVR_STOP : ("terminate", M_ID.ACK_NAK),
            M_ID.RADIO_START : ("radio_start", M_ID.ACK_NAK),
            M_ID.RADIO_STOP : ("radio_stop", M_ID.ACK_NAK),
            M_ID.NUM_RX : ("set_num_rx", M_ID.ACK_NAK),
            #---------------
            # Radio commands
            M_ID.HF_PRE : ("set_hf_pre", M_ID.ACK_NAK),
            M_ID.ATTN : ("set_attn", M_ID.ACK_NAK),
            M_ID.R1_FREQ : ("set_rx1_freq", M_ID.ACK_NAK),
            M_ID.R2_FREQ : ("set_rx2_freq", M_ID.ACK_NAK),
            M_ID.R3_FREQ : ("set_rx3_freq", M_ID.ACK_NAK),
            M_ID.R1_MODE : ("set_rx1_mode", M_ID.ACK_NAK),
            M_ID.R2_MODE : ("set_rx2_mode", M_ID.ACK_NAK),
            M_ID.R3_MODE : ("set_rx3_mode", M_ID.ACK_NAK),
            M_ID.R1_FILT : ("set_rx1_filter", M_ID.ACK_NAK),
            M_ID.R2_FILT : ("set_rx2_filter", M_ID.ACK_NAK),
            M_ID.R3_FILT  : ("set_rx3_filter", M_ID.ACK_NAK),
            M_ID.TX_FILT : ("set_tx_filter", M_ID.ACK_NAK),
            M_ID.R1_AGC : ("set_rx1_agc", M_ID.ACK_NAK),
            M_ID.R2_AGC : ("set_rx2_agc", M_ID.ACK_NAK),
            M_ID.R3_AGC : ("set_rx3_agc", M_ID.ACK_NAK),
            M_ID.R1_GAIN : ("set_rx1_gain", M_ID.ACK_NAK),
            M_ID.R2_GAIN : ("set_rx2_gain", M_ID.ACK_NAK),
            M_ID.R3_GAIN  : ("set_rx3_gain", M_ID.ACK_NAK),
            #---------------
            # Alex filters
            M_ID.ALEX_AUTO : ("set_alex_auto", M_ID.ACK_NAK),
            M_ID.HF_BYPASS : ("set_hf_bypass", M_ID.ACK_NAK),
            M_ID.LPF_160 : ("set_lpf_160", M_ID.ACK_NAK),
            M_ID.LPF_80 : ("set_lpf_80", M_ID.ACK_NAK),
            M_ID.LPF_60_40 : ("set_lpf_60_40", M_ID.ACK_NAK),
            M_ID.LPF_30_20 : ("set_lpf_30_20", M_ID.ACK_NAK),
            M_ID.LPF_17_15 : ("set_lpf_17_15", M_ID.ACK_NAK),
            M_ID.LPF_12_10 : ("set_lpf_12_10", M_ID.ACK_NAK),
            M_ID.LPF_6 : ("set_lpf_6", M_ID.ACK_NAK),
            M_ID.HPF_1_5 : ("set_hpf_1_5", M_ID.ACK_NAK),
            M_ID.HPF_6_5 : ("set_hpf_6_5", M_ID.ACK_NAK),
            M_ID.HPF_9_5 : ("set_hpf_9_5", M_ID.ACK_NAK),
            M_ID.HPF_13 : ("set_hpf_13", M_ID.ACK_NAK),
            M_ID.HPF_20 : ("set_hpf_20", M_ID.ACK_NAK),
            #---------------
            # Audio
            M_ID.ENUM_INPUTS : ("enum_inputs", M_ID.DATA),
            M_ID.ENUM_OUTPUTS : ("enum_outputs", M_ID.DATA),
            M_ID.REVERT_OUTPUTS : ("revert_outputs", M_ID.ACK_NAK),
            M_ID.CHANGE_OUTPUTS : ("change_outputs", M_ID.ACK_NAK),
            M_ID.AUDIO_ROUTE : ("set_audio_route", M_ID.ACK_NAK),
            M_ID.LOCAL_AUDIO_RUN : ("local_audio_run", M_ID.ACK_NAK),
            M_ID.CLEAR_AUDIO_ROUTES : ("clear_audio_routes", M_ID.ACK_NAK),
            M_ID.RESTART_AUDIO_ROUTES : ("restart_audio_routes", M_ID.ACK_NAK),  
            #---------------
            # Display
            M_ID.DISP_WIDTH : ("set_display_width", M_ID.ACK_NAK),
            M_ID.DISP_PERIOD : ("set_disp_period", M_ID.ACK_NAK),
            M_ID.DISP_STATE : ("set_disp_state", M_ID.ACK_NAK),
        }
        return t[msg_id]
    
#=====================================================
# Interface class to SDRLibEConnector -> server -> HPSDR
# Sends messages to SDRLibEConnector and receives responses
#===================================================== 
class Connector: 
    #-------------------------------------------------
    # Initialisation
    def __init__(self) :
        # Get app model
        server_model = Model.get_server_model()
        # Create the command socket
        self.__cmd_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        # Set a default timeout
        self.__cmd_sock.settimeout(5)
        # IP and Port of server
        self.__IP = server_model['SERVER-IP']
        self.__port = server_model['SERVER-CMD-PORT']
        # Buffers
        self.__resp_data = bytearray(4096)

    #==============================================================================================
    # PUBLIC
    #==============================================================================================
    
    #-------------------------------------------------
    # Send command and get synchronous response
    def cmd_exchange(self, cmd_id, params):
        entry = M_LU.lookup(cmd_id)
        cmd_str = entry[0] 
        resp_type = entry[1]
        msg = self.__get_message(cmd_str, params)
        self.__cmd_sock.sendto(msg, 0, (self.__IP, self.__port))
        return self.__do_receive(cmd_str, resp_type)
    
    #-------------------------------------------------
    def set_audio_routes(self, reset):
        # Set all server audio route
        if reset:
            # First clear down existing routes
            if not self.cmd_exchange(M_ID.CLEAR_AUDIO_ROUTES, []):
                print("Failed to clear audio route(s)!")
                return
        
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
            if not self.cmd_exchange(M_ID.RESTART_AUDIO_ROUTES, []):
                 print("Failed to restart audio!")
        
            
    #==============================================================================================
    # PRIVATE
    #==============================================================================================
    
    #-------------------------------------------------
    # Return the message structure for the given command
    def __get_message(self, cmd_str, params):
        msg = {"cmd" : cmd_str, "params" : params}
        return bytes(json.dumps(msg), 'UTF-8')
    
    #-------------------------------------------------
    # Wait for data from server
    def __do_receive(self, cmd_str, resp_type):
        
        try:
            nbytes, address = self.__cmd_sock.recvfrom_into(self.__resp_data)
        except socket.error as e:
            # print("Error receiving response %s!" % (str(e)))
            return None
        if nbytes > 0 :
            if resp_type == M_ID.ACK_NAK:
                # Boolean response for an ACK/NAK
                resp = json.loads(self.__resp_data.decode('UTF-8')[:nbytes])
                if resp['resp'] == 'ACK' : return True
                else : return False
            else :
                # We have response data to return
                return json.loads(self.__resp_data.decode('UTF-8')[:nbytes])
        else :
            return None

    #-------------------------------------------------
    # Set one audio route
    def __set_route(self, radio, sink, dev, ch):
        
        print("__set_route: ", radio, sink, dev, ch)
        if dev != NONE and ch != NONE:
            (api, dev) = dev.split('@')
            if not self.cmd_exchange(M_ID.AUDIO_ROUTE, [DIR_OUTPUT, sink, radio, api, dev, ch]):
                print("Error setting route for radio %d" % [radio])
                
#==============================================================================================
# TESTING
#==============================================================================================
# Main code

def set_audio(conn):
    # Get the audio outputs
    outputs =  conn.cmd_exchange(M_ID.ENUM_OUTPUTS, [])
    if outputs != None :
        # Get the speaker out parameters
        d = outputs["outputs"][1]
        return conn.cmd_exchange(M_ID.AUDIO_ROUTE, [1, 'LOCAL', 1, d['api'], d['name'], 'BOTH'])
    else :
        return None
           
def main():
        
    try:
        c = Connector()
        
        print ("DISCOVER", c.cmd_exchange(M_ID.DISCOVER, []))
        print("SET AUDIO", set_audio(c))
        print ("SRV_START", c.cmd_exchange(M_ID.SVR_START, []))
        print ("RADIO_START", c.cmd_exchange(M_ID.RADIO_START, [False]))
        print ("R1_FREQ", c.cmd_exchange(M_ID.R1_FREQ, [7.15]))
        
        """
        print ("POLL", c.cmd_exchange(M_ID.POLL, []))
        print ("DISCOVER", c.cmd_exchange(M_ID.DISCOVER, []))
        print ("SRV_START", c.cmd_exchange(M_ID.SVR_START, []))
        print ("RADIO_START", c.cmd_exchange(M_ID.RADIO_START, [False]))
        print ("RADIO_STOP", c.cmd_exchange(M_ID.RADIO_STOP, []))
        print ("NUM_RX", c.cmd_exchange(M_ID.NUM_RX, []))
        print ("HF_PRE", c.cmd_exchange(M_ID.HF_PRE, [True]))
        print ("ATTN", c.cmd_exchange(M_ID.ATTN, [0]))
        print ("R1_FREQ", c.cmd_exchange(M_ID.R1_FREQ, [7.1]))
        print ("R2_FREQ", c.cmd_exchange(M_ID.R2_FREQ, [7.1]))
        print ("R3_FREQ", c.cmd_exchange(M_ID.R3_FREQ, [7.1]))
        print ("R1_MODE", c.cmd_exchange(M_ID.R1_MODE, [1]))
        print ("R2_MODE", c.cmd_exchange(M_ID.R2_MODE, [2]))
        print ("R3_MODE", c.cmd_exchange(M_ID.R3_MODE, [3]))
        print ("R1_FILT", c.cmd_exchange(M_ID.R1_FILT, [1,2]))
        print ("R2_FILT", c.cmd_exchange(M_ID.R2_FILT, [1,2]))
        print ("R3_FILT", c.cmd_exchange(M_ID.R3_FILT, [1,2]))
        print ("R1_AGC", c.cmd_exchange(M_ID.R1_AGC, [0]))
        print ("R2_AGC", c.cmd_exchange(M_ID.R2_AGC, [1]))
        print ("R3_AGC", c.cmd_exchange(M_ID.R3_AGC, [2]))
        print ("R1_GAIN", c.cmd_exchange(M_ID.R1_GAIN, [0.1]))
        print ("R2_GAIN", c.cmd_exchange(M_ID.R2_GAIN, [0.2]))
        print ("R3_GAIN", c.cmd_exchange(M_ID.R3_GAIN, [0.3]))
        print ("ALEX_AUTO", c.cmd_exchange(M_ID.ALEX_AUTO, [False]))
        print ("HF_BYPASS", c.cmd_exchange(M_ID.HF_BYPASS, [False]))
        print ("LPF_160", c.cmd_exchange(M_ID.LPF_160, [True]))
        print ("LPF_80", c.cmd_exchange(M_ID.LPF_80, [True]))
        print ("LPF_60_40", c.cmd_exchange(M_ID.LPF_60_40, [True]))
        print ("LPF_30_20", c.cmd_exchange(M_ID.LPF_30_20, [True]))
        print ("LPF_17_15", c.cmd_exchange(M_ID.LPF_17_15, [True]))
        print ("LPF_12_10", c.cmd_exchange(M_ID.LPF_12_10, [True]))
        print ("LPF_6", c.cmd_exchange(M_ID.LPF_6, [True]))
        print ("LPF_6", c.cmd_exchange(M_ID.LPF_6, [True]))
        print ("HPF_1_5", c.cmd_exchange(M_ID.HPF_1_5, [True]))
        print ("HPF_6_5", c.cmd_exchange(M_ID.HPF_6_5, [True]))
        print ("HPF_9_5", c.cmd_exchange(M_ID.HPF_9_5, [True]))
        print ("HPF_13", c.cmd_exchange(M_ID.HPF_13, [True]))
        print ("HPF_20", c.cmd_exchange(M_ID.HPF_20, [True]))
        print ("ENUM_INPUTS", c.cmd_exchange(M_ID.ENUM_INPUTS, []))
        print ("ENUM_OUTPUTS", c.cmd_exchange(M_ID.ENUM_OUTPUTS, []))
        print ("REVERT_OUTPUTS", c.cmd_exchange(M_ID.REVERT_OUTPUTS, []))
        print ("CHANGE_OUTPUTS", c.cmd_exchange(M_ID.CHANGE_OUTPUTS, []))
        print ("AUDIO_ROUTE", c.cmd_exchange(M_ID.AUDIO_ROUTE, []))
        print ("DISP_WIDTH", c.cmd_exchange(M_ID.DISP_WIDTH, [600]))
        print ("DISP_PERIOD", c.cmd_exchange(M_ID.DISP_PERIOD, [100]))
        print ("DISP_STATE", c.cmd_exchange(M_ID.DISP_STATE, [True,True,True]))
        """
    except Exception as e:
            print ('Exception [%s][%s]' % (str(e), traceback.format_exc()))

# Entry point       
if __name__ == '__main__':
    main()
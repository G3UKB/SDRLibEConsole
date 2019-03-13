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

# System imports
import json
import socket
import traceback
import pprint
pp = pprint.PrettyPrinter(indent=4)
from _thread import start_new_thread
from time import sleep
from enum import Enum, auto

# Application imports

#==============================================================================================
# The connector drives the SDRLibE/connector executable via a JSON UDP interface. This enables
# distribution if required and also makes it a managed server which will execute on its own
# once its setup is complete which could be a command line client or a GUI client.
#==============================================================================================

#=====================================================
# Message name lookup
#===================================================== 
class M_ID(Enum):
    POLL = auto(),
    DISCOVER = auto()
class M_LU:
    def lookup(msg_id):
        t = {
            M_ID.POLL : "poll",
            M_ID.DISCOVER : "radio_discover"
        }
        return t[msg_id]
    

#=====================================================
# Sends messages to SDRLibEConnector and receives responses
#===================================================== 
class Connector:
    
    #-------------------------------------------------
    # Initialisation
    def __init__(self) :
        # Create the command socket
        self.__cmd_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        # Set a default timeout
        self.__cmd_sock.settimeout(5)
        # IP and Port of server
        self.__IP = '127.0.0.1'
        self.__port = 10010
        # Buffers
        self.__resp_data = bytearray(4096)

    #==============================================================================================
    # PUBLIC
    #==============================================================================================
    # Send command and get response
    def cmd_exchange(self, cmd_id, params):
        cmd_str = M_LU.lookup(cmd_id)
        msg = self.__get_message(cmd_str, params)
        self.__cmd_sock.sendto(msg, 0, (self.__IP, self.__port))
        return self.__do_receive(cmd_str)
        
    #==============================================================================================
    # PRIVATE
    #==============================================================================================
    # Return the message structure for the given command
    def __get_message(self, cmd_str, params):
        msg = {"cmd" : cmd_str, "params" : params}
        return bytes(json.dumps(msg), 'UTF-8')
    
    #-------------------------------------------------
    # Wait for data from server
    def __do_receive(self, cmd_str):
        nbytes, address = self.__cmd_sock.recvfrom_into(self.__resp_data)
        if nbytes > 0 :
            return json.loads(self.__resp_data.decode('UTF-8')[:nbytes])
        else :
            return none

#==============================================================================================
# TESTING
#==============================================================================================
# Main code
def main():
        
    try:
        c = Connector()
        print (c.cmd_exchange(M_ID.POLL, []))
        print (c.cmd_exchange(M_ID.DISCOVER, []))
    except Exception as e:
            print ('Exception [%s][%s]' % (str(e), traceback.format_exc()))

# Entry point       
if __name__ == '__main__':
    main()
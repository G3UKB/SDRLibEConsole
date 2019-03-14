#!/usr/bin/env python
#
# app_main.py
#
# Main application for SDRLibEConsole application
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
import os,sys
import traceback
from time import sleep
import logging

# Lib imports
from PyQt5.QtWidgets import QApplication, QWidget

# Application imports
sys.path.append('..')
from common.defs import *
from connector.connector import *
from ui.main_window.main_window import *

#==============================================================================================
# The main application for SDRLibEConsole
#==============================================================================================

#=====================================================
# Main application class
#=====================================================
class AppMain:
    #-------------------------------------------------
    # Start processing and wait for user to exit the application
    def main(self):
        # The one and only application
        self.__qtapp = QApplication([])
        # Create a top level window
        #self.__w = QWidget()
        #self.__w.show()
        
        # Create a connector interface
        con = Connector()
        # and run a test script
        print ("DISCOVER", con.cmd_exchange(M_ID.DISCOVER, []))
        print("SET AUDIO", set_audio(con))
        print ("SRV_START", con.cmd_exchange(M_ID.SVR_START, []))
        print ("RADIO_START", con.cmd_exchange(M_ID.RADIO_START, [False]))
        print ("R1_FREQ", con.cmd_exchange(M_ID.R1_FREQ, [7.15]))
        
        self.__w = MainWindow(con)
        self.__w.show()
        
        # Enter the event loop
        r = self.__qtapp.exec_()
        
#=====================================================
# Entry point
#=====================================================
def main():
    try:  
        app = AppMain()
        sys.exit(app.main())
        
    except Exception as e:
        print ('Exception from main code','Exception [%s][%s]' % (str(e), traceback.format_exc()))

# Enter here when run as script        
if __name__ == '__main__':
    main()
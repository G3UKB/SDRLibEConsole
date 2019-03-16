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
from model.model import *
from framework.instance_cache import *
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
        # The one and only QT application
        self.__qtapp = QApplication([])
        
        # Restore the model
        self.__m = Model()
        addToCache('model_inst', self.__m)
        self.__m.restore_model()
        
        # Create a connector interface
        con = Connector()
        addToCache('conn_inst', con)
        # and run a test script
        print ("DISCOVER", con.cmd_exchange(M_ID.DISCOVER, []))
        print("SET AUDIO", set_audio(con))
        print ("SRV_START", con.cmd_exchange(M_ID.SVR_START, []))
        print ("RADIO_START", con.cmd_exchange(M_ID.RADIO_START, [False]))
        
        # Create the modes window
        mode_win = Modes()
        addToCache('mode_win', mode_win)
        filter_win = Filters()
        addToCache('filter_win', filter_win)
        
        # Create the main window class
        self.__w = MainWindow()
        # Make visible
        self.__w.show()
        
        # Enter the GUI event loop
        r = self.__qtapp.exec_()
        
        # Save the model
        self.__m.save_model()
        
#=====================================================
# Entry point
#=====================================================

#-------------------------------------------------
# Start processing and wait for user to exit the application
def main():
    try:  
        app = AppMain()
        sys.exit(app.main())
        
    except Exception as e:
        print ('Exception from main code','Exception [%s][%s]' % (str(e), traceback.format_exc()))

#-------------------------------------------------
# Enter here when run as script        
if __name__ == '__main__':
    main()
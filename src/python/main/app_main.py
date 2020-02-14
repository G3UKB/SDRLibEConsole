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

# Import all
import sys
sys.path.append('..')
from main.imports import *

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
        
        # Create the lib interfaces
        self.__server = Server()
        addToCache('server_inst', self.__server)
        self.__audio = Audio()
        addToCache('audio_inst', self.__audio)
        self.__radio = Radio()
        addToCache('radio_inst', self.__radio)
        self.__dsp = DSP()
        addToCache('dsp_inst', self.__dsp)
        self.__display = Display()
        addToCache('display_inst', self.__display)
        
        # Create a connector inst
        self.__con = Connector()
        addToCache('conn_inst', self.__con)
        
        # See if we need to run wisdom
        self.__dsp.server_make_wisdom("..\\wisdom\\".encode('utf-8'))
        
        # Attempt to initialise the server
        if not self.__con.coldstart():
            print("Failed to coldstart server!")
        
        # Create the main window class
        self.__w = MainWindow()
        # Make visible
        self.__w.show()
        
        # Create an aux window for testing
        self.__aw2 = AuxWindow("Aux [RX-2]", 0, 2)
        addToCache('aw2_inst', self.__aw2)
        self.__aw3 = AuxWindow("Aux [RX-3]", 0, 3)
        addToCache('aw3_inst', self.__aw3)
        
        # Enter the GUI event loop
        r = self.__qtapp.exec_()
        
        # Close the server
        self.__con.terminate()
        
        # Save window metrics
        self.__w.saveMetrics()
        self.__aw2.saveMetrics()
        self.__aw3.saveMetrics()
        
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
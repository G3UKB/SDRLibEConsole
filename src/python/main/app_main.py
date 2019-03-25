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
        
        # Create a connector interface
        self.__con = Connector()
        addToCache('conn_inst', self.__con)
        # See if a server or simulator is running?
        state = Model.get_state_model()
        if self.__con.cmd_exchange(M_ID.POLL, []) == None:
            print("Failed to connect to server! Press 'Start' to try again.")
        else:
            state['HAVE-SERVER'] = True
            if self.__con.cmd_exchange(M_ID.DISCOVER, []) == None:
                print("No radio hardware detected! Press 'Start' to try again.")
            else:
                state['DISCOVER'] = True
                # Set all audio routes
                self.set_audio_routes()        
                if self.__con.cmd_exchange(M_ID.SVR_START, []) == None:
                    print("Sorry, failed to start server, unable to continue!")
                    sys.exit()
                state['SERVER-RUN'] = True
        
        # Create the main window class
        self.__w = MainWindow()
        # Make visible
        self.__w.show()
        
        # Create an aux window for testing
        self.__aw = AuxWindow("Aux [RX-2]", 0, 2)
        # Make visible
        self.__aw.show()
        
        # Enter the GUI event loop
        r = self.__qtapp.exec_()
        
        # Save window metrics
        self.__w.saveMetrics()
        
        # Save the model
        self.__m.save_model()
    
    #-------------------------------------------------
    # Set all routes
    def set_audio_routes(self):
        radio_model = self.__m.get_radio_model()
        if radio_model[1]['AUDIO']['DEV'] != NONE:
            (api, dev) = radio_model[1]['AUDIO']['DEV'].split('@')
            if not self.__con.cmd_exchange(M_ID.AUDIO_ROUTE, [DIR_OUTPUT, radio_model[1]['AUDIO']['SINK'], 1, api, dev, radio_model[1]['AUDIO']['CH']]):
                print("Failed to set audio route! Unable to continue")
                sys.exit()
        
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
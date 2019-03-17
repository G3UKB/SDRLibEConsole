#!/usr/bin/env python
#
# main_wimndow.py
#
# Main window for the SDRLibEConsole application
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
# The main window for SDRLibEConsole
#==============================================================================================

#=====================================================
# Main window class
# The main window runs RX1 and has all common controls
# Auxiliary windows run the other radios
#=====================================================
class MainWindow(QMainWindow):
    
    #-------------------------------------------------
    # Constructor
    def __init__(self):
        
        super(MainWindow, self).__init__()
        
        # Get instances
        self.__con = getInstance('conn_inst')
        self.__mode_win = getInstance('mode_win')
        self.__filter_win = getInstance('filter_win')
        self.__agc_win = getInstance('agc_win')
        
        # Set title
        self.setWindowTitle('SDRLIbEConsole')
        # Set the back colour
        palette = QPalette()
        palette.setColor(QPalette.Background, QColor(43,63,68,255))
        self.setPalette(palette)
        
        # Get app model
        self.__app_model = Model.get_app_model()
        # Set window metrics
        self.setGeometry(self.__app_model['X'], self.__app_model['Y'], self.__app_model['W'], self.__app_model['H'])
        
        # Get radio model
        self.__radio_model = Model.get_radio_model()
        
        # Main panel and grid
        panel = QWidget()
        self.setCentralWidget(panel)
        main_grid = QGridLayout()
        panel.setLayout(main_grid)
        # Contents
        self.__setup_ui(main_grid)
    
    #-------------------------------------------------
    # Setup UI contents
    def __setup_ui(self, main_grid) :
        
        # Top side button grid
        # Note add the grid directly as a layout not in a panel else space cannot be removed
        self.__top_grid = QGridLayout()
        main_grid.addLayout(self.__top_grid, 0, 0)
        
        # Left side button grid
        # Note add the grid directly as a layout not in a panel else space cannot be removed
        self.__side_grid = QGridLayout()
        main_grid.addLayout(self.__side_grid, 1, 1)
        self.__side_grid.setSpacing(0)
        margins = QMargins()
        margins.setLeft = 0
        margins.setRight = 0
        margins.setTop = 0
        margins.setBottom = 0
        self.__side_grid.setContentsMargins(margins)
        
        #-------------------------------------------------
        # Top buttons
        # Start button
        self.start_btn = QPushButton('Start', self)
        self.__set_button(self.__top_grid, self.start_btn, 0, 0, self.__start_evnt, 'Start', 2)
        self.start_btn.setCheckable(True)
        # Exit button
        self.exit_btn = QPushButton('Exit', self)
        self.__set_button(main_grid, self.exit_btn, 0, 1, self.__exit_evnt, 'Exit', 2)
        
        #-------------------------------------------------
        # Left side buttons
        # Mode button
        self.mode_btn = QPushButton('Mode', self)
        self.__set_button(self.__side_grid, self.mode_btn, 0, 0, self.__mode_evnt, mode_lookup[self.__radio_model[1]['MODE']][1], 1)
        # Filter button
        self.filter_btn = QPushButton('Filter', self)
        self.__set_button(self.__side_grid, self.filter_btn, 1, 0, self.__filter_evnt, filter_lookup[self.__radio_model[1]['FILTER']][3], 1)
        # AGC button
        self.agc_btn = QPushButton('AGC', self)
        self.__set_button(self.__side_grid, self.agc_btn, 2, 0, self.__agc_evnt, agc_lookup[self.__radio_model[1]['AGC']][1], 1)
        
        #-------------------------------------------------
        # VFO control
        vfo_grid = QGridLayout()
        main_grid.addLayout(vfo_grid, 1, 0)
        self.__vfo = Vfo(self.__con, CH_RX, 1)
        self.__vfo.addVfo(self, vfo_grid)
    
    #==============================================================================================
    # PUBLIC
    #==============================================================================================
    
    #-------------------------------------------------
    # Callback for current mode
    def setMode(self, mode):    
        self.mode_btn.setText(mode)
    
    #-------------------------------------------------
    # Callback for current filter
    def setFilter(self, filter):    
        self.filter_btn.setText(filter)
    
    #-------------------------------------------------
    # Callback for current AGC setting
    def setAGC(self, agc):    
        self.agc_btn.setText(agc)
    
    #-------------------------------------------------
    # Save window metrics
    def setMetrics(self):
        app_model = Model.get_app_model()
        app_model['X'] = self.x()
        app_model['Y'] = self.y()
        app_model['W'] = self.width()
        app_model['H'] = self.height()
        
    #==============================================================================================
    # OVERRIDES
    #==============================================================================================
        
    #-------------------------------------------------
    # Mouse wheel event for VFO
    def wheelEvent(self, event):
        # Forward event to VFO
        if event.angleDelta().y() >= 0:
            self.__vfo.doWheelEvent(VFO_UP)
        else:
            self.__vfo.doWheelEvent(VFO_DOWN)
    
    #==============================================================================================
    # EVENTS
    #==============================================================================================
    
    #-------------------------------------------------
    # Start button event
    def __start_evnt(self, btn_state) :
        
        # See what we need to start first
        state = Model.get_state_model()
        if not state['HAVE-SERVER']:
            if self.__con.cmd_exchange(M_ID.POLL, []) == None:
                print("Failed to connect to server! Please start the server and try again.")
                return
            else:
                state['HAVE-SERVER'] = True
        if not state['DISCOVER']:
            if self.__con.cmd_exchange(M_ID.DISCOVER, []) == None:
                print("No radio hardware detected! Please start the radio and try again.")
                return
            else:
                state['DISCOVER'] = True
        if not state['SERVER-RUN']:
            if self.__con.cmd_exchange(M_ID.SVR_START, []) == None:
                print("Sorry, failed to start server! Please try a server restart.")
                return
            else:
                state['SERVER-RUN'] = True
        
        # Good to go
        if btn_state:
            # Start radio
            self.__con.cmd_exchange(M_ID.RADIO_START, [False])
            self.start_btn.setStyleSheet("QPushButton {background-color: rgb(58,86,92); color: green; font: bold 12px}")
            self.start_btn.setText('Stop')
            state['RADIO-RUN'] = True
        else:
            # Stop radio
            self.__con.cmd_exchange(M_ID.RADIO_STOP, [])
            self.start_btn.setStyleSheet("QPushButton {background-color: rgb(58,86,92); color: red; font: bold 12px}")
            self.start_btn.setText('Start')
            state['RADIO-RUN'] = False
            
    #-------------------------------------------------
    # Exit button event
    def __exit_evnt(self) :
        QApplication.quit()
    
    #-------------------------------------------------
    # Mode button event
    def __mode_evnt(self) :
        self.__mode_win.set_context(self.setMode, self.x() + self.width(), self.y(), CH_RX, 1)
        self.__mode_win.show()
    
    #-------------------------------------------------
    # Filter button event
    def __filter_evnt(self) :
        self.__filter_win.set_context(self.setFilter, self.x() + self.width(), self.y() + 20, CH_RX, 1)
        self.__filter_win.show()
    
    #-------------------------------------------------
    # AGC button event
    def __agc_evnt(self) :
        self.__agc_win.set_context(self.setAGC, self.x() + self.width(), self.y() + 40, CH_RX, 1)
        self.__agc_win.show()
    
    #==============================================================================================
    # PRIVATE
    #==============================================================================================
    
    #-------------------------------------------------
    # Set up button
    def __set_button(self, grid, button, row, col, callback, text, style ):
        if style == 1 :
            button.setStyleSheet("QPushButton {background-color: rgb(58,86,92); color: rgb(14,20,22); font: bold 10px}")
        elif style == 2 :
            button.setStyleSheet("QPushButton {background-color: rgb(58,86,92); color: rgb(242,79,0); font: bold 12px}")
        grid.addWidget(button, row, col)
        button.clicked.connect(callback)
        button.setText(text)
        button.setMaximumSize(50,20)
    
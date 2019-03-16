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

# System imports
import os,sys
import traceback
from time import sleep
import logging

# Lib imports
from PyQt5.QtWidgets import QWidget, QMainWindow, QGridLayout
from PyQt5.QtGui import QPalette, QColor

# Application imports
from framework.instance_cache import *
from ui.components.vfo import *
from ui.components.button_base import *
from ui.components.modes import *
from common.defs import *
from model.model import *

#==============================================================================================
# The main window for SDRLibEConsole
#==============================================================================================

#=====================================================
# Main window class
#=====================================================
class MainWindow(QMainWindow):
    
    #-------------------------------------------------
    # Constructor
    def __init__(self):
        super(MainWindow, self).__init__()
        self.__con = getInstance('conn_inst')
        self.__mode_win = getInstance('mode_win')
       
        # Set the back colour
        palette = QPalette()
        palette.setColor(QPalette.Background, QColor(43,63,68,255))
        self.setPalette(palette)
        
        # Get radio model
        self.__radio_model = Model.get_radio_model()
        
        # Main panel and grid
        panel = QWidget()
        self.setCentralWidget(panel)
        main_grid = QGridLayout()
        panel.setLayout(main_grid)
    
        # Top grid
        # Note add the grid directly as a layout not in a panel else space cannot be removed
        top_grid = QGridLayout()
        main_grid.addLayout(top_grid, 0, 0)
        # Add mode button
        self.mode_btn = QPushButton('Mode', self)
        self.mode_btn.setStyleSheet("QPushButton {background-color: rgb(140,21,38); font: bold 12px}")
        top_grid.addWidget(self.mode_btn, 0, 0)
        self.mode_btn.clicked.connect(self.__mode_evnt)
        self.mode_btn.setText(mode_lookup[self.__radio_model[1]['MODE']][1])
        # Add filter button
        self.filter_btn = QPushButton('Filter', self)
        self.filter_btn.setStyleSheet("QPushButton {background-color: rgb(140,21,38); font: bold 12px}")
        top_grid.addWidget(self.filter_btn, 0, 1)
        self.filter_btn.clicked.connect(self.__filter_evnt)
        # Add AGC button
        self.agc_btn = QPushButton('AGC', self)
        self.agc_btn.setStyleSheet("QPushButton {background-color: rgb(140,21,38); font: bold 12px}")
        top_grid.addWidget(self.agc_btn, 0, 2)
        self.agc_btn.clicked.connect(self.__agc_evnt)
    
        # Add VFO control
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
    # Mode button event
    def __mode_evnt(self) :
        self.__mode_win.set_context(self.setMode, self.x(), self.y(), CH_RX, 1)
        self.__mode_win.show()
    
    def __filter_evnt(self) :
        pass
    
    def __agc_evnt(self) :
        pass
    
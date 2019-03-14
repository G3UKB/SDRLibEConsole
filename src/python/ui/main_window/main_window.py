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
from ui.components.vfo import *
from common.defs import *

#==============================================================================================
# The main window for SDRLibEConsole
#==============================================================================================

#=====================================================
# Main window class
#=====================================================
class MainWindow(QMainWindow):
    
    def __init__(self, connector):
        super(MainWindow, self).__init__()
        self.__con = connector
        
        # Set the back colour
        palette = QPalette()
        palette.setColor(QPalette.Background, QColor(43,63,68,255))
        self.setPalette(palette)
        
        # Main panel and grid
        panel = QWidget()
        self.setCentralWidget(panel)
        main_grid = QGridLayout()
        panel.setLayout(main_grid)
    
        # Top grid
        # Note add the grid directly as a layout not in a panel else space cannot be removed
        top_grid = QGridLayout()
        main_grid.addLayout(top_grid, 0, 0)
    
        # Add VFO control
        vfo_grid = QGridLayout()
        main_grid.addLayout(vfo_grid, 1, 0)
        self.__vfo = Vfo(self.__con, CH_RX, 0)
        self.__vfo.addVfo(self, vfo_grid, 7.1)
        
        
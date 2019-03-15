#!/usr/bin/env python
#
# modes.py
#
# Modes class for SDRLibEConnector server
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
from PyQt5.QtCore import Qt, QObject, QEvent, QMargins
from PyQt5.QtWidgets import QWidget, QPushButton, QGridLayout
from PyQt5.QtGui import QPalette, QColor

# Application imports
from common.defs import *
from connector.connector import *

#==============================================================================================
# Modes provides a button panel for the available modes
#==============================================================================================

#=====================================================
# Modes main class
#===================================================== 
class Modes(QWidget):
    
    #-------------------------------------------------
    # Constructor
    def __init__(self, connector, direction, modes_id):
        """
        Constructor
        
        Arguments:  
            connector   --  ref to the connector instance
            direction   --  CH_RX | CH_TX
            modes_id    --  the rx or tx id for this mode panel
            
        """
        
        super(Modes, self).__init__()
        
        self.__con = connector
        self.__direction = direction
        self.__modes_id = modes_id
        
        # Set the back colour
        palette = QPalette()
        palette.setColor(QPalette.Background, QColor(43,63,68,255))
        self.setPalette(palette)
        self.setWindowFlags(
            Qt.CustomizeWindowHint |
            Qt.FramelessWindowHint
        )
        
        # Set the grid
        self.__grid = QGridLayout()
        self.__grid.setSpacing(0)
        margins = QMargins()
        margins.setLeft = 0
        margins.setRight = 0
        margins.setTop = 0
        margins.setBottom = 0
        self.__grid.setContentsMargins(margins)
        self.setLayout(self.__grid)
        
        self.lsb_btn = QPushButton('LSB', self)
        self.__add_button(self.lsb_btn, 0, 0)
        self.usb_btn = QPushButton('USB', self)
        self.__add_button(self.usb_btn, 0, 1)
        self.dsb_btn = QPushButton('DSB', self)
        self.__add_button(self.dsb_btn, 0, 2)
        self.cwl_btn = QPushButton('CW-L', self)
        self.__add_button(self.cwl_btn, 1, 0)
        self.cwu_btn = QPushButton('CW-U', self)
        self.__add_button(self.cwu_btn, 1, 1)
        self.fm_btn = QPushButton('FM', self)
        self.__add_button(self.fm_btn, 1, 2)
        self.am_btn = QPushButton('AM', self)
        self.__add_button(self.am_btn, 2, 0)
        self.digu_btn = QPushButton('DIG-U', self)
        self.__add_button(self.digu_btn, 2, 1)
        self.spec_btn = QPushButton('DIG-L', self)
        self.__add_button(self.spec_btn, 2, 2)
        self.digl_btn = QPushButton('SPEC', self)
        self.__add_button(self.digl_btn, 3, 0)
        self.sam_btn = QPushButton('SAM', self)
        self.__add_button(self.sam_btn, 3, 1)
        self.drm_btn = QPushButton('DRM', self)
        self.__add_button(self.drm_btn, 3, 2)
    
    def __add_button(self, button, row, col):
        button.setCheckable(True)
        button.setStyleSheet("QPushButton {background-color: rgb(140,21,38); font: bold 12px}")
        self.__grid.addWidget(button, row, col)
        
        
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
from PyQt5.QtWidgets import QWidget, QPushButton, QButtonGroup, QGridLayout
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
    def __init__(self):
        """
        Constructor
        
        Arguments:  
            
        """
        
        super(Modes, self).__init__()
        
        # Get the connector instance
        self.__con = getInstance('conn_inst')
        
        # Set the back colour
        palette = QPalette()
        palette.setColor(QPalette.Background, QColor(43,63,68,255))
        self.setPalette(palette)
        self.setWindowFlags(
            Qt.CustomizeWindowHint |
            Qt.FramelessWindowHint |
            Qt.WindowStaysOnTopHint
        )
        self.setWindowOpacity(0.7)
        
        # Set the layout
        self.__grid = QGridLayout()
        self.__grid.setSpacing(0)
        margins = QMargins()
        margins.setLeft = 0
        margins.setRight = 0
        margins.setTop = 0
        margins.setBottom = 0
        self.__grid.setContentsMargins(margins)
        self.setLayout(self.__grid)
        
        # Create an exclusive button group
        self.__btn_grp = QButtonGroup()
        self.__btn_grp.setExclusive(True)
        
        # Add mode buttons
        self.__btns = (
            # btn(label), row, col, id 
            (QPushButton('LSB', self), 0, 0, 0),
            (QPushButton('USB', self), 0, 1, 1),
            (QPushButton('DSB', self), 0, 2, 2),
            (QPushButton('CW-L', self), 1, 0, 3),
            (QPushButton('CW-U', self), 1, 1, 4),
            (QPushButton('FM', self), 1, 2, 5),
            (QPushButton('AM', self), 2, 0, 6),
            (QPushButton('DIG-U', self), 2, 1, 7),
            (QPushButton('DIG-L', self), 2, 2, 8),
            (QPushButton('SPEC', self), 3, 0, 9),
            (QPushButton('SAM', self), 3,  1, 10),
            (QPushButton('DRM', self), 3, 2, 11),
        )
        for btn in self.__btns:
           self.  __add_button(btn[0], btn[1], btn[2], btn[3])
        
        # Connect click event
        self.__btn_grp.buttonClicked.connect(self.__mode_evnt)
    
    #==============================================================================================
    # PUBLIC
    #==============================================================================================
    
    #-------------------------------------------------
    # Set context   
    def set_context(self, callback, x, y, direction, modes_id):
        """
        Set Context
        
        Arguments:  
            callback    --  callback here with mode string
            x           --  x coord of main window
            y           --  y coord of main window
            direction   --  RX or TX
            modes_id    -- the id of this mode instantiation (radio id)
            
        """
        self.__callback = callback
        self.__direction = direction
        # Position at top right corner of invoking button
        self.__modes_id = modes_id
        self.move( x + 30, y + 50)
    
    #==============================================================================================
    # PRIVATE
    #==============================================================================================
        
    #-------------------------------------------------
    # Add a button to the grid   
    def __add_button(self, btn, row, col, id):
        """
        Add button to grid
        
        Arguments:  
            btn --  the QButton
            row --  grid row
            col --  grid col
            id  --  id to give button
            
        """
        btn.setCheckable(True)
        btn.setStyleSheet("QPushButton {background-color: rgb(167,167,167); font: bold 12px}")
        self.__grid.addWidget(btn, row, col)
        self.__btn_grp.addButton(btn)
        self.__btn_grp.setId(btn, id)
    
    #-------------------------------------------------
    # Button group click event
    def __mode_evnt(self, btn) :
        """
        Any button click event
        
        Arguments:  
            btn --  the QButton
            
        """
        if self.__direction == CH_RX:
            if self.__modes_id == 1:
                r = M_ID.R1_MODE
            elif self.__modes_id == 2:
                r = M_ID.R2_MODE
            else:
                r = M_ID.R3_MODE
            # Execute mode change
            self.__con.cmd_exchange(r, [self.__btn_grp.id(btn)])
            # Its a popup
            self.hide()
            # Tell parent what was selected
            self.__callback(btn.text())
        
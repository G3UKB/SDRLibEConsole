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
from ui.components.button_base import *

#==============================================================================================
# Modes provides a button panel for the available modes
#==============================================================================================

#=====================================================
# Modes main class
#===================================================== 
class Modes(ButtonBase):
    
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
           self.  add_button(btn[0], btn[1], btn[2], btn[3])
        
        # Connect click event
        self.btn_grp.buttonClicked.connect(self.__mode_evnt)
    
    #==============================================================================================
    # PUBLIC
    #==============================================================================================
    
    #==============================================================================================
    # PRIVATE
    #==============================================================================================
        
    #-------------------------------------------------
    # Button group click event
    def __mode_evnt(self, btn) :
        """
        Any button click event
        
        Arguments:  
            btn --  the QButton
            
        """
        if self.direction == CH_RX:
            if self.id == 1:
                r = M_ID.R1_MODE
            elif self.id == 2:
                r = M_ID.R2_MODE
            else:
                r = M_ID.R3_MODE
            # Execute mode change
            self.__con.cmd_exchange(r, [self.btn_grp.id(btn)])
            # Its a popup
            self.hide()
            # Tell parent what was selected
            self.callback(btn.text())
        
#!/usr/bin/env python
#
# vfo.py
#
# VFO class for SDRLibEConnector server
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
from PyQt5.QtCore import QObject, QEvent
from PyQt5.QtWidgets import QWidget, QLabel

# Application imports
from common.defs import *
from connector.connector import *

# Resources
MHZ_FONT = 30
KHZ_FONT = 30
HZ_FONT = 20
MHZ_COLOR = 'white'
KHZ_COLOR = 'white'
HZ_COLOR = 'orange'

#==============================================================================================
# The VFO provides 9 scrollable digits and is multi-instanced for each RX and TX.
#==============================================================================================

#=====================================================
# VFO main class
#===================================================== 
class Vfo:
    
    def __init__(self, connector, direction, vfo_id):
        """
        Constructor
        
        Arguments:  
            connector   --  ref to the connector instance
            direction   --  CH_RX | CH_TX
            vfo_id      --  the rx or tx id for this vfo
            
        """
        
        self.__con = connector
        self.__dir = direction
        self.__vfo_id = vfo_id
        
        # Initialise variables
        # Set when the mouse is over a VFO digit to the respective increment
        self.__freq_inc = None
        # Tracks the current frequency
        self.__current_freq = MIN_FREQ
        
        # Lookup table for increments in MHz
        self.__freq_inc_lookup = {
            '100MHz': 100.0,
            '10MHz': 10.0,
            '1MHz': 1.0,
            '100KHz': 0.1,
            '10KHz': 0.01,
            '1KHz': 0.001,
            '100Hz': 0.0001,
            '10Hz': 0.00001,
            '1Hz': 0.000001,
        }
        
    # Interactors --------------------------------------------------------------
    def addVfo(self, parent, grid, current_freq):
        """
        Add a single VFO control
        
        Arguments:
            parent          --  parent widget
            grid            --  grid to add interactors to
            current_freq    --  the current freq from the saved state
            
        """
        
        grid.setSpacing(0)
        grid.setRowMinimumHeight(0, 20)
        grid.setColumnMinimumWidth(0, 20)
        grid.setColumnMinimumWidth(1, 20)
        grid.setColumnMinimumWidth(2, 20)
        grid.setColumnMinimumWidth(3, 20)
        grid.setColumnMinimumWidth(4, 20)
        grid.setColumnMinimumWidth(5, 20)
        grid.setColumnMinimumWidth(6, 20)
        grid.setColumnMinimumWidth(7, 20)
        grid.setColumnMinimumWidth(8, 20)
        grid.setColumnMinimumWidth(9, 20)
        # Add the VFO digits
        self.__d_100MHz = VfoDigit('0', MHZ_COLOR, MHZ_FONT)
        self.__d_100MHz.setAccessibleName('100MHz')
        self.__d_10MHz = VfoDigit('0', MHZ_COLOR,MHZ_FONT)
        self.__d_10MHz.setAccessibleName('10MHz')
        self.__d_1MHz = VfoDigit('0', MHZ_COLOR,KHZ_FONT)
        self.__d_1MHz.setAccessibleName('1MHz')
        self.__d_100KHz = VfoDigit('0', KHZ_COLOR,KHZ_FONT)
        self.__d_100KHz.setAccessibleName('100KHz')
        self.__d_10KHz = VfoDigit('0', KHZ_COLOR,KHZ_FONT)
        self.__d_10KHz.setAccessibleName('10KHz')
        self.__d_1KHz = VfoDigit('0', KHZ_COLOR,KHZ_FONT)
        self.__d_1KHz.setAccessibleName('1KHz')
        self.__d_100Hz = VfoDigit('0', HZ_COLOR,HZ_FONT)
        self.__d_100Hz.setAccessibleName('100Hz')
        self.__d_10Hz = VfoDigit('0', HZ_COLOR,HZ_FONT)
        self.__d_10Hz.setAccessibleName('10Hz')
        self.__d_1Hz = VfoDigit('0', HZ_COLOR,HZ_FONT)
        self.__d_1Hz.setAccessibleName('1Hz')
        sep1 = VfoSep('_', 'orange',20)
        sep2 = VfoSep('_', 'orange',20)
        grid.addWidget(self.__d_100MHz,0,0)
        grid.addWidget(self.__d_10MHz,0,1)
        grid.addWidget(self.__d_1MHz,0,2)
        grid.addWidget(sep1,0,3)
        grid.addWidget(self.__d_100KHz,0,4)
        grid.addWidget(self.__d_10KHz,0,5)
        grid.addWidget(self.__d_1KHz,0,6)
        grid.addWidget(sep2,0,7)
        grid.addWidget(self.__d_100Hz,0,8)
        grid.addWidget(self.__d_10Hz,0,9)
        grid.addWidget(self.__d_1Hz,0,10)
        
        filter = mouseoverEvent(parent, self.__mouse_over)
        self.__d_100MHz.installEventFilter(filter)
        self.__d_10MHz.installEventFilter(filter)
        self.__d_1MHz.installEventFilter(filter)
        self.__d_100KHz.installEventFilter(filter)
        self.__d_10KHz.installEventFilter(filter)
        self.__d_1KHz.installEventFilter(filter)
        self.__d_100Hz.installEventFilter(filter)
        self.__d_10Hz.installEventFilter(filter)
        self.__d_1Hz.installEventFilter(filter)
        
        # Set the frequency from the saved state
        self.__adjust_vfo(current_freq)
        self.__current_freq = current_freq
    
    def wheelEvent(self, delta):
        """
        Mouse wheel event
        
        Arguments:
            delta   --  change in position
            
        """
        
        if self.__freq_inc != None:
            if delta > 0:
                if (self.__current_freq + self.__freq_inc) < MAX_FREQ:
                    self.__current_freq = self.__current_freq + self.__freq_inc
            else:
                if (self.__current_freq - self.__freq_inc) > MIN_FREQ:
                    self.__current_freq = self.__current_freq - self.__freq_inc
            
            self.set_freq(self.__current_freq)
        return self.__current_freq
    
    def set_freq(self, freq):
        """
        Set freq
        
        Arguments:
            freq    --  to this frequency
            
        """
        
        self.__adjust_vfo(freq)
        self.__current_freq = freq
        if self.__direction == CH_RX:
            self.__con.cmd_exchange(M_ID.R1_FREQ, [self.__current_freq])
        
    # Helpers --------------------------------------------------------------
    def __mouse_over(self, digit):
        """
        Callback from the mouse over digit
        
        Arguments:
            digit   --  accessible name of the VFO digit object
            
        """
        
        if digit == None:
            self.__freq_inc = None
        else:
            self.__freq_inc = self.__freq_inc_lookup[digit]
    
    def __adjust_vfo(self, freq):
        """
        Set the VFO to the given frequency
        
        freq    --  float freq to set in MHz
        
        """
        
        freq_str = str((int(freq*1000000))).zfill(9)
        self.__d_100MHz.setText(freq_str[0])
        self.__d_10MHz.setText(freq_str[1])
        self.__d_1MHz.setText(freq_str[2])
        self.__d_100KHz.setText(freq_str[3])
        self.__d_10KHz.setText(freq_str[4])
        self.__d_1KHz.setText(freq_str[5])
        self.__d_100Hz.setText(freq_str[6])
        self.__d_10Hz.setText(freq_str[7])
        self.__d_1Hz.setText(freq_str[8])
        
        self.__last_freq = freq
        
"""
Event Filter Class for mouse over
"""
class mouseoverEvent(QObject):
    
    def __init__(self, parent, callback):
        """
        Constructor
        
        Arguments:
            parent      --  QT parent
            callback    --  with mouse over digit
            
        """
        
        super(mouseoverEvent, self).__init__(parent)
        self.__callback = callback
        
    def eventFilter(self, object, event):
        name = object.accessibleName()
        if name.find('MHz') != -1:
            normal_font = MHZ_FONT
        elif name.find('KHz') != -1:
            normal_font = KHZ_FONT
        else:
            normal_font = HZ_FONT
        if event.type() == QEvent.Enter:
            # Entered a digit
            object.setStyleSheet(object.styleSheet() + "QLabel {font: bold %spx}" % (normal_font))
            self.__callback(name)
            return True
        if event.type() == QEvent.Leave:
            # Left a digit
            object.setStyleSheet(object.styleSheet() + "QLabel {font: normal %spx}" % (normal_font))
            self.__callback(None)
            return True
        return False

"""
GUI Classes
"""
class VfoDigit(QLabel):
    
    def __init__(self, text, colour, size):
        super(VfoDigit, self).__init__(text)
        
        self.setStyleSheet('QLabel { color : %s; font-family: "Times New Roman"; font: normal %spx; border-style: none}' % (colour, str(size)))
        self.setWindowOpacity (0.0)
        self.setMinimumWidth (20)
        self.setMinimumHeight(20)
 
class VfoSep(QLabel):
    
    def __init__(self, text, colour, size):
        super(VfoSep, self).__init__(text)
        
        self.setStyleSheet('QLabel { color : %s; font-family: "Times New Roman"; font: normal %spx; border-style: none}' % (colour, str(size)));
        self.setWindowOpacity (0.0)
        
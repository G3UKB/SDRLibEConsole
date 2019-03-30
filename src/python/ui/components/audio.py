#!/usr/bin/env python
#
# audio.py
#
# Audio option panel for SDRLibEConsole
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
# Audio provides a selection of available options per receiver and transmitter
#==============================================================================================

#=====================================================
# Audio main class
#===================================================== 
class Audio(QWidget):
    
    #-------------------------------------------------
    # Constructor
    def __init__(self, id):
        """
        Constructor
        
        Arguments:
            id  -- the radio id
            
        """
        super(Audio, self).__init__()
        
        # radio id
        self.__id = id
        
        # Get the connector instance
        self.__con = getInstance('conn_inst')
        
        # Create the UI
        # Set the back colour
        palette = QPalette()
        palette.setColor(QPalette.Background, QColor(41,41,41,255))
        self.setPalette(palette)
        self.setWindowFlags(
            Qt.CustomizeWindowHint |
            Qt.FramelessWindowHint |
            Qt.WindowStaysOnTopHint
        )
        self.setWindowOpacity(0.97)
        # Set the layout
        grid = QGridLayout()
        self.setLayout(grid)
        
        # Get radio model audio section
        self.__radio_1_audio_model = Model.get_radio_model()[1]['AUDIO']
        self.__radio_2_audio_model = Model.get_radio_model()[2]['AUDIO']
        self.__radio_3_audio_model = Model.get_radio_model()[3]['AUDIO']
        
        # Populate the grid
        self.__setup_ui(grid)
    
    #-------------------------------------------------
    # Populate the UI
    def __setup_ui(self, grid) :
        
        # Output sinks
        # Select HPSDR or Local
        src_label = QLabel('Sink')
        src_label.setStyleSheet("QLabel {color: rgb(196,196,196)}")
        grid.addWidget(src_label, 0, 0)
        self.__sink_combo = QComboBox()
        self.__sink_combo.addItems((HPSDR, LOCAL_AF, LOCAL_IQ))
        grid.addWidget(self.__sink_combo, 0, 1, 1, 3)
        self.__sink_combo.activated.connect(self.__sink_evnt)
        
        # Select the audio device
        dev_label = QLabel('Device')
        dev_label.setStyleSheet("QLabel {color: rgb(196,196,196)}")
        grid.addWidget(dev_label, 1, 0)
        self.__dev_combo = QComboBox()
        output_list = self.__con.cmd_exchange(M_ID.ENUM_OUTPUTS, [])
        devices = []
        if output_list == None:
            print("Failed to enumerate audio outputs!")
        else:
            devices.append('No selection')
            for device in output_list['outputs']:
                devices.append('%s@%s' % (device['api'], device['name']))
        self.__dev_combo.addItems(devices)
        grid.addWidget(self.__dev_combo, 1, 1, 1, 3)
        self.__dev_combo.activated.connect(self.__dev_evnt)
        
        # Assign channel
        # Create an exclusive button group
        self.btn_grp = QButtonGroup()
        self.btn_grp.setExclusive(True)
        btn_panel = QWidget()
        btn_box = QHBoxLayout()
        btn_panel.setLayout(btn_box)
        grid.addWidget(btn_panel, 2, 1, 1, 3)
        ch_label = QLabel('Ch')
        ch_label.setStyleSheet("QLabel {color: rgb(196,196,196)}")
        grid.addWidget(ch_label, 2, 0)
        
        self.__ch_left = QCheckBox('Left')
        self.btn_grp.addButton(self.__ch_left)
        self.btn_grp.setId(self.__ch_left, 0)
        self.__ch_left.setStyleSheet("QCheckBox {color: rgb(196,196,196)}")
        btn_box.addWidget(self.__ch_left)
        self.__ch_left.setVisible(False)
        
        self.__ch_right = QCheckBox('Right')
        self.btn_grp.addButton(self.__ch_right)
        self.btn_grp.setId(self.__ch_right, 1)
        self.__ch_right.setStyleSheet("QCheckBox {color: rgb(196,196,196)}")
        btn_box.addWidget(self.__ch_right)
        
        self.__ch_both = QCheckBox('Both')
        self.btn_grp.addButton(self.__ch_both)
        self.btn_grp.setId(self.__ch_both, 2)
        self.__ch_both.setStyleSheet("QCheckBox {color: rgb(196,196,196)}")
        btn_box.addWidget(self.__ch_both)
        
        self.__ch_none = QCheckBox('None')
        self.btn_grp.addButton(self.__ch_none)
        self.btn_grp.setId(self.__ch_none, 3)
        self.__ch_none.setStyleSheet("QCheckBox {color: rgb(196,196,196)}")
        btn_box.addWidget(self.__ch_none)
        
        self.btn_grp.buttonClicked.connect(self.__ch_evnt)
        
        # 'Apply' buttons
        applybtn = QPushButton('Apply')
        applybtn.setToolTip('Apply this audio configuration')
        applybtn.resize(applybtn.sizeHint())
        applybtn.setEnabled(True)
        grid.addWidget(applybtn, 3, 2)
        applybtn.clicked.connect(self.__apply_evnt)
        applybtn.setStyleSheet("QPushButton {background-color: rgb(59,59,59); color: rgb(196,196,196); font: bold 10px}")
        
        # 'Cancel' buttons
        cancelbtn = QPushButton('Cancel')
        cancelbtn.setToolTip('Cancel changes')
        cancelbtn.resize(cancelbtn.sizeHint())
        cancelbtn.setEnabled(True)
        grid.addWidget(cancelbtn, 3, 3)
        cancelbtn.clicked.connect(self.__cancel_evnt)
        cancelbtn.setStyleSheet("QPushButton {background-color: rgb(59,59,59); color: rgb(196,196,196); font: bold 10px}")
        
        # Set UI state
        self.__reset_state()
        
    #==============================================================================================
    # PUBLIC
    #==============================================================================================
    
    #-------------------------------------------------
    # Set context according to id
    def set_context(self, callback, x, y, direction):
        
        """
        Set Context
        
        Arguments:  
            callback    --  callback here with mode string
            x           --  x coord of main window
            y           --  y coord of main window
            direction   --  RX or TX
            
        """
        self.callback = callback
        self.direction = direction
        # Position at top right corner of invoking button

        self.move( x, y)
        # Set UI state
        self.__reset_state()
        
    #==============================================================================================
    # PRIVATE
    #==============================================================================================
    
    #-------------------------------------------------
    # UI Events    
    #-------------------------------------------------
    
    #-------------------------------------------------
    def __sink_evnt(self, idx) :
        """
        Sink update
        
        Arguments:  
            
            
        """
        self.__sink = self.__sink_combo.currentText()
        self.__update_state()
    
    #-------------------------------------------------
    def __dev_evnt(self, idx) :
        """
        Device update
        
        Arguments:  
            
            
        """
  
        self.__dev = self.__dev_combo.currentText()
        self.__update_state()
    
    #-------------------------------------------------
    def __ch_evnt(self, btn) :
        """
        Ch update, any checkbox
        
        Arguments:  
            
            
        """
        if self.btn_grp.id(btn) == 0:
            ch = LEFT
        elif self.btn_grp.id(btn) == 1:
            ch = RIGHT
        elif self.btn_grp.id(btn) == 2:
            ch = BOTH
        elif self.btn_grp.id(btn) == 3:
            ch = NONE
        self.__ch = ch
    
    #-------------------------------------------------
    def __apply_evnt(self, e) :
        """
        Any button click event
        
        Arguments:  
            
            
        """
        
        # Set the model
        radio = self.__get_model()
        radio['SINK'] = self.__sink
        radio['DEV'] = self.__dev
        radio['CH'] = self.__ch
        # Reset the server audio routes
        self.__con.set_audio_routes(True)
        # It's a popup
        self.close()
    
    #-------------------------------------------------
    def __cancel_evnt(self, e) :
        """
        Any button click event
        
        Arguments:  
            
            
        """
        
        self.__sink = None
        self.__dev = None
        self.__ch = None
        # It's a popup
        self.close()
    
    #-------------------------------------------------
    # Helpers
    #-------------------------------------------------
    
    #-------------------------------------------------
    def __get_model(self):
        if self.__id == RX_1:
            return self.__radio_1_audio_model
        elif self.__id == RX_2:
            return self.__radio_2_audio_model
        else:
            return self.__radio_3_audio_model
    
    #-------------------------------------------------
    def __get_other_models(self):
        if self.__id == RX_1:
            return self.__radio_2_audio_model, self.__radio_3_audio_model
        elif self.__id == RX_2:
            return self.__radio_1_audio_model, self.__radio_3_audio_model
        else:
            return self.__radio_1_audio_model, self.__radio_2_audio_model
        
    #-------------------------------------------------
    # Set UI to last model state
    def __reset_state(self):
        # Set holding values
        radio = self.__get_model()
        self.__sink = radio['SINK']
        self.__dev = radio['DEV']
        self.__ch = radio['CH']
        
        # Update UI to last known state
        # Sink
        self.__sink_combo.setCurrentIndex(self.__sink_combo.findText(self.__sink))
        # Device
        if radio['DEV'] != None:
            index = self.__dev_combo.findText(self.__dev)
        else:
            index = 0
        self.__dev_combo.setCurrentIndex(index)
        # Set widget context
        self.__manage_dev()
        self.__manage_ch()
    
    #-------------------------------------------------
    # Set UI to last local state
    def __update_state(self):
        self.__manage_dev()
        self.__manage_ch()
        
    #-------------------------------------------------
    # Set device visability
    def __manage_dev(self):
        if self.__sink == HPSDR:
            self.__dev_combo.setVisible(False)
        else:
            self.__dev_combo.setVisible(True)
    
    #-------------------------------------------------
    # Set channel visibility
    def __manage_ch(self):
        # NO, use id and other radios
        # Available channels
        # We have to check not only what the setting is for this receiver but what channels are available
        # by checking all other recevers.
        other_radio_1, other_radio_2 = self.__get_other_models()
        if self.__dev == NONE or self.__sink == HPSDR:
            # No device defined or not local so we can't set a channel
            self.__ch_left.setVisible(False)
            self.__ch_right.setVisible(False)
            self.__ch_both.setVisible(False)
            self.__ch_none.setVisible(False)
        else :
            # We have a device so check the saved channel
            # Set all visible first
            self.__ch_left.setVisible(True)
            self.__ch_right.setVisible(True)
            self.__ch_both.setVisible(True)
            self.__ch_none.setVisible(True)
            # Set the channel according to model
            if self.__ch == NONE:
                self.__ch_none.setCheckState(Qt.Checked)
            elif self.__ch == LEFT:
                self.__ch_left.setCheckState(Qt.Checked)                
            elif self.__ch == RIGHT:
                self.__ch_right.setCheckState(Qt.Checked)    
            elif self.__ch == BOTH:
                self.__ch_both.setCheckState(Qt.Checked)
            
            # Then turn off those which are allocated elsewhere
            if  other_radio_1['CH'] == LEFT or other_radio_2['CH'] == LEFT:
                if  self.__dev == other_radio_1['DEV'] or \
                    self.__dev == other_radio_2['DEV']:
                    self.__ch_left.setVisible(False)
                    self.__ch_both.setVisible(False)
                    if self.__ch == LEFT:
                        print("Audio problem, more than one channel on the same device allocated LEFT ch!")
            
            if  other_radio_1['CH'] == RIGHT or other_radio_2['CH'] == RIGHT:
                if  self.__dev == other_radio_1['DEV'] or \
                    self.__dev == other_radio_2['DEV']:
                    self.__ch_right.setVisible(False)
                    self.__ch_both.setVisible(False)
                    if self.__ch == RIGHT:
                        print("Audio problem, more than one channel on the same device allocated RIGHT ch!")
            
            if  other_radio_1['CH'] == BOTH or other_radio_2['CH'] == BOTH:
                if  self.__dev == other_radio_1['DEV'] or \
                    self.__dev == other_radio_2['DEV']:
                    self.__ch_left.setVisible(False)
                    self.__ch_right.setVisible(False)
                    self.__ch_both.setVisible(False)
                    if self.__ch == LEFT or self.__ch == RIGHT or self.__ch == BOTH:
                        print("Audio problem, more than one channel on the same device conflicts with BOTH ch!")
                        

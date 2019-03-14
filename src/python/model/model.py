#!/usr/bin/env python
#
# model.py
#
# Application, options and state model for the SDRLibEConsole application
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
import os, sys
import json
import pickle
import traceback
import pprint
pp = pprint.PrettyPrinter(indent=4)

# Application imports

#==============================================================================================
# The model for SDRLibEConsole
#==============================================================================================

#=====================================================
# Main model class
#=====================================================
class Model:
    
    #-------------------------------------------------
    # Constructor
    def __init__(self):
        
        # This is the default model used for first run or after recovery
        self.__default_model = {
            'APP' : {
                'SERVER-IP' : '127.0.0.1',
                'SERVER-CMD-PORT' : 10010,
            },
            'RADIO' : {
                1 : {
                    'FREQ' : 7100000,
                    'MODE' : 0,
                    'FILTER' : 0,
                },
                2 : {
                    
                },
                3 : {
                    
                }
            },
            'STATE' : {
                
            }
        }
        # Restore point
        self.__model = None
        # Model is saved to the current directory
        self.__PATH = 'SDRLibEConsole.model'
    
    #==============================================================================================
    # PUBLIC
    #==============================================================================================
    
    #-------------------------------------------------
    # Restore from disk   
    def restore_model(self):
        model = self.__restore_model()
        if model == None:
            # Failed to restore model
            # Use the default model
            self.__model = self.__default_model
        else:
            self.__model = model
    
    #-------------------------------------------------
    # Save to disk   
    def save_model(self):
        self.__save_model()
    
    #-------------------------------------------------
    # Get model   
    def get_model(self):
        return self.__model 
        
    #==============================================================================================
    # PRIVATE
    #==============================================================================================
    
    #-------------------------------------------------
    # Implementation of restore from disk
    def __restore_model(self):
        """
        Restore the saved model
        
        Arguments:
                
        """
        
        model = None
        if os.path.exists(self.__PATH):
            try:       
                f = open(self.__PATH, 'rb')
                model = pickle.load(f)
            except Exception as e:
                # Error retrieving model file
                QMessageBox.information(None, 'Model File - Exception','Exception [%s]' % (str(e)), QtGui.QMessageBox.Ok)
            finally:
                try:
                    f.close()
                except:
                    pass
        return model

    #-------------------------------------------------
    # Implementation of save to disk
    def __save_model(self):
        """
        Save the model
        
        Arguments:

        """
        
        try:       
            f = open(self.__PATH, 'wb')
            pickle.dump(self.__model, f)
        except Exception as e:
            # Error saving model file
            QMessageBox.information(self, 'Model File - Exception','Exception [%s]' % (str(e)), QtGui.QMessageBox.Ok)
        finally:
            try:
                f.close()
            except:
                pass
            
/*
properties.cpp

Properties save/restore for the Juce Console

Copyright (C) 2019 by G3UKB Bob Cowdery

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

The authors can be reached by email at:

	bob@bobcowdery.plus.com

*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "properties.h"


//==============================================================================
// Display Panel
//==============================================================================

//==============================================================================
// Public

//==============================================================================
// Constructor/Destructor
Properties::Properties()
{
	printf("Here\n");
	// Local vars
	fprop = new File("props.cfg");
	PropertiesFile::Options options = PropertiesFile::Options::Options();
	options.applicationName = "JuceConsole";
	//options.filenameSuffix = "cfg";
	//options.folderName = ".";
	
	properties_file = new PropertiesFile(*fprop, options);
	//std::unique_ptr<XmlElement> el = properties_file->createXml("RX-1");
	//XmlElement* e = el.get();
	//e->
	//properties_file->setValue("a property", &el);
	properties_file->setValue("Property 1", var(100));
	properties_file->setValue("Property 2", var(100));
	properties_file->save();
	printf("Saved\n");
	properties_file->reload();
	StringPairArray & p = properties_file->getAllProperties();
	printf("%s\n", p.getValue("Property 1", "Default"));
}

Properties::~Properties()
{
}
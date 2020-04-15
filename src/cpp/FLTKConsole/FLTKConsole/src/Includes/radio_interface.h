/*
radio_interface.h

Header for interface to the co-located server for the FLTK Console

Copyright (C) 2020 by G3UKB Bob Cowdery

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

#define RX 0
#define TX 1

enum class MODES {
	CH_LSB,
	CH_USB,
	CH_DSB,
	CH_CWL,
	CH_CWU,
	CH_FM,
	CH_AM,
	CH_DIGU,
	CH_SPEC,
	CH_DIGL,
	CH_SAM,
	CH_DRM
};

//==============================================================================
// Access methods to the co-located 'C' server.
class RadioInterface
{
public:
	// Construct/destroy
	//==============================================================================
	RadioInterface();
	~RadioInterface() {};

	//==============================================================================
	// Method prototypes
	void ri_make_wisdom();
	void cold_start();
	bool restart();
	void reset_radio_state();
	bool ri_set_default_audio();
	void set_audio_paths();
	bool ri_server_start();
	bool ri_server_terminate();
	bool ri_radio_discover();
	bool ri_radio_start(int wbs);
	bool ri_radio_stop();
	void ri_server_set_mode(int channel, int mode);
	void ri_server_set_filter_freq(int channel, int filter);
	void ri_server_cc_out_set_freq(int radio, unsigned int freq_in_hz);
	void ri_server_cc_out_set_duplex(bool state);
	void ri_server_cc_out_set_mox(bool state);

private:
	//==============================================================================
	// State variables
	Preferences* p;

	//==============================================================================
	// Method prototypes
	void get_filter_limits(int filter, int* low, int* high);
	void set_mode_filter(int channel, int mode, int filter_id, bool set_radio);
	void set_frequencies();
	void set_modes();
	void set_filters();
};
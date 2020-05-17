/*
hamlib.cpp

CAT interface to any rig

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

/*
	Provides a high level CAT interface to nearly all rigs.

*/

#include "../Includes/includes.h"

extern "C" {
	//RIG* rig_init(rig_model_t  	rig_model);
	//int rig_load_all_backends();
}
//==============================================================================
// Defines
#define SERIAL_PORT "COM3"

// Hamlib sample program
void test_hamlib()
{
	RIG *my_rig; // handle to rig (nstance) 
	freq_t freq; // frequency 
	rmode_t rmode; // radio mode of operation 
	pbwidth_t width;
	vfo_t vfo; // vfo selection 
	int strength; // S-Meter level 
	int rit = 0; // RIT status 
	int xit = 0; // XIT status 
	int retcode; // generic return code from functions 
	rig_model_t myrig_model;
	printf("hamlib: Hello, Hamlib here!\n");
	// Turn off backend debugging ouput 
	//rig_set_debug_level(RIG_DEBUG_NONE);
	
	// allocate memory, setup & open port
	
	hamlib_port_t myport;
	printf("1\n");
	// may be overriden by backend probe
	myport.type.rig = RIG_PORT_SERIAL;
	printf("2\n");
	myport.parm.serial.rate = 9600;
	myport.parm.serial.data_bits = 8;
	myport.parm.serial.stop_bits = 2;
	myport.parm.serial.parity = RIG_PARITY_NONE;
	myport.parm.serial.handshake = RIG_HANDSHAKE_NONE;
	strncpy_s(myport.pathname, SERIAL_PORT, FILPATHLEN - 1);
	printf("2 %s\n", myport.pathname);
	rig_load_all_backends();
	//myrig_model = rig_probe(&myport);
	printf("3\n");
	myrig_model = atoi("120");
	printf("4\n");
	my_rig = rig_init(120);
	printf("5\n");
	if (!my_rig) {
		fprintf(stderr, "Unknown rig num: %d\n", myrig_model);
		fprintf(stderr, "Please check riglist.h\n");
		exit(1); // whoops! something went wrong (mem alloc?) 
	}
	//printf("6\n");
	//strncpy_s(my_rig->state.rigport.pathname, SERIAL_PORT, FILPATHLEN - 1);
	//retcode = rig_open(my_rig);
	//if (retcode != RIG_OK) {
	//	printf("rig_open: error = %s\n", rigerror(retcode));
	//	exit(2);
	//}
	printf("Port %s opened ok\n", SERIAL_PORT);
/*
	* Below are examples of set/get routines.
	* Must add checking of functionality map prior to command execution -- FS
	*
	
	
	* Example of setting rig paameters
	* and some error checking on the return code.
	
	retcode = rig_set_vfo(my_rig, RIG_VFO_B);
	if (retcode != RIG_OK) {
		printf("rig_set_vfo: error = %s \n", rigerror(retcode));
	}
	
	* Lets try some frequencies and modes. Return code is not checked.
	* Examples of checking return code are further down.
	*
	
	// 10m FM Narrow 
	printf("\nSetting 10m FM Narrow...\n");
	retcode = rig_set_freq(my_rig, RIG_VFO_CURR, 29620000); // 10m 
	retcode = rig_set_mode(my_rig, RIG_VFO_CURR, RIG_MODE_FM,
		rig_passband_narrow(my_rig, RIG_MODE_FM));
	if (retcode != RIG_OK) {
		printf("rig_set_freq: error = %s \n", rigerror(retcode));
	}
	rig_get_freq(my_rig, RIG_VFO_CURR, &freq);
	rig_get_mode(my_rig, RIG_VFO_CURR, &rmode, &width);
	printf(" Freq: %.6f MHz, Mode: %s, Passband: %.3f kHz\n\n",
		freq / 1000000, rig_strrmode(rmode), width / 1000.0);
	Sleep(1); // so you can see it -- FS 
	// 15m USB 
	printf("Setting 15m USB...\n");
	retcode = rig_set_freq(my_rig, RIG_VFO_CURR, 21235175);
	retcode = rig_set_mode(my_rig, RIG_VFO_CURR, RIG_MODE_USB,
		rig_passband_normal(my_rig, RIG_MODE_USB));
	if (retcode != RIG_OK) {
		printf("rig_set_freq: error = %s \n", rigerror(retcode));
	}
	rig_get_freq(my_rig, RIG_VFO_CURR, &freq);
	rig_get_mode(my_rig, RIG_VFO_CURR, &rmode, &width);
	printf(" Freq: %.6f MHz, Mode: %s, Passband: %.3f kHz\n\n",
		freq / 1000000, rig_strrmode(rmode), width / 1000.0);
	Sleep(1);
	// 40m LSB 
	printf("Setting 40m LSB...\n");
	retcode = rig_set_freq(my_rig, RIG_VFO_CURR, 7250100);
	retcode = rig_set_mode(my_rig, RIG_VFO_CURR, RIG_MODE_LSB, RIG_PASSBAND_NORMAL);
	if (retcode != RIG_OK) {
		printf("rig_set_freq: error = %s \n", rigerror(retcode));
	}
	rig_get_freq(my_rig, RIG_VFO_CURR, &freq);
	rig_get_mode(my_rig, RIG_VFO_CURR, &rmode, &width);
	printf(" Freq: %.6f MHz, Mode: %s, Passband: %.3f kHz\n\n",
		freq / 1000000, rig_strrmode(rmode), width / 1000.0);
	Sleep(1);
	// 80m AM Narrow 
	printf("Setting 80m AM Narrow...\n");
	retcode = rig_set_freq(my_rig, RIG_VFO_CURR, 3885000);
	retcode = rig_set_mode(my_rig, RIG_VFO_CURR, RIG_MODE_AM,
		rig_passband_narrow(my_rig, RIG_MODE_AM));
	if (retcode != RIG_OK) {
		printf("rig_set_freq: error = %s \n", rigerror(retcode));
	}
	rig_get_freq(my_rig, RIG_VFO_CURR, &freq);
	rig_get_mode(my_rig, RIG_VFO_CURR, &rmode, &width);
	printf(" Freq: %.6f MHz, Mode: %s, Passband: %.3f kHz\n\n",
		freq / 1000000, rig_strrmode(rmode), width / 1000.0);
	Sleep(1);
	// 160m CW Normal 
	printf("Setting 160m CW...\n");
	retcode = rig_set_freq(my_rig, RIG_VFO_CURR, 1875000); 
	retcode = rig_set_mode(my_rig, RIG_VFO_CURR, RIG_MODE_CW, RIG_PASSBAND_NORMAL);
	if (retcode != RIG_OK) {
		printf("rig_set_freq: error = %s \n", rigerror(retcode));
	}
	rig_get_freq(my_rig, RIG_VFO_CURR, &freq);
	rig_get_mode(my_rig, RIG_VFO_CURR, &rmode, &width);
	printf(" Freq: %.3f kHz, Mode: %s, Passband: %li Hz\n\n",
		freq / 1000, rig_strrmode(rmode), width);
	Sleep(1);
	// 160m CW Narrow -- The band is noisy tonight -- FS
	printf("Setting 160m CW Narrow...\n");
	retcode = rig_set_mode(my_rig, RIG_VFO_CURR, RIG_MODE_CW,
		rig_passband_narrow(my_rig, RIG_MODE_CW));
	if (retcode != RIG_OK) {
		printf("rig_set_freq: error = %s \n", rigerror(retcode));
	}
	rig_get_freq(my_rig, RIG_VFO_CURR, &freq);
	rig_get_mode(my_rig, RIG_VFO_CURR, &rmode, &width);
	printf(" Freq: %.3f kHz, Mode: %s, Passband: %li Hz\n\n",
		freq / 1000, rig_strrmode(rmode), width);
	Sleep(1);
	// AM Broadcast band 
	printf("Setting Medium Wave AM...\n");
	retcode = rig_set_freq(my_rig, RIG_VFO_CURR, 770000); 
	retcode = rig_set_mode(my_rig, RIG_VFO_CURR, RIG_MODE_AM, RIG_PASSBAND_NORMAL);
	if (retcode != RIG_OK) {
		printf("rig_set_freq: error = %s \n", rigerror(retcode));
	}
	rig_get_freq(my_rig, RIG_VFO_CURR, &freq);
	rig_get_mode(my_rig, RIG_VFO_CURR, &rmode, &width);
	printf(" Freq: %.3f kHz, Mode: %s, Passband: %.3f kHz\n\n",
		freq / 1000, rig_strrmode(rmode), width / 1000.0);
	Sleep(1);
	// 20m USB on VFO_A 
	printf("Setting 20m on VFO A with two functions...\n");
	retcode = rig_set_vfo(my_rig, RIG_VFO_A);
	retcode = rig_set_freq(my_rig, RIG_VFO_CURR, 14250375); 
	if (retcode != RIG_OK) {
		printf("rig_set_freq: error = %s \n", rigerror(retcode));
	}
	rig_get_freq(my_rig, RIG_VFO_CURR, &freq);
	rig_get_vfo(my_rig, &vfo);
	printf(" Freq: %.6f MHz, VFO: %s\n\n", freq / 1000000, rig_strvfo(vfo));
	Sleep(1);
	// 20m USB on VFO_A , with only 1 call 
	printf("Setting 20m on VFO A with one function...\n");
	retcode = rig_set_freq(my_rig, RIG_VFO_A, 14295125); 
	if (retcode != RIG_OK) {
		printf("rig_set_freq: error = %s \n", rigerror(retcode));
	}
	rig_get_freq(my_rig, RIG_VFO_CURR, &freq);
	rig_get_vfo(my_rig, &vfo);
	printf(" Freq: %.6f MHz, VFO: %s\n\n", freq / 1000000, rig_strvfo(vfo));
	Sleep(1);
#if 0
	retcode = rig_set_freq(my_rig, RIG_VFO_CURR, 145100000);
	sleep(2);
	retcode = rig_set_freq(my_rig, RIG_VFO_CURR, 435125000);
	sleep(2);
#endif
	printf("Setting rig Mode to LSB.\n");
	retcode = rig_set_mode(my_rig, RIG_VFO_CURR, RIG_MODE_LSB, RIG_PASSBAND_NORMAL);
	if (retcode != RIG_OK) {
		printf("rig_set_mode: error = %s \n", rigerror(retcode));
	}
	Sleep(1);
	printf("Setting rig PTT ON.\n");
	retcode = rig_set_ptt(my_rig, RIG_VFO_A, RIG_PTT_ON); 
	if (retcode != RIG_OK) {
		printf("rig_set_ptt: error = %s \n", rigerror(retcode));
	}
	Sleep(1);
	printf("Setting rig PTT OFF.\n");
	retcode = rig_set_ptt(my_rig, RIG_VFO_A, RIG_PTT_OFF);
	if (retcode != RIG_OK) {
		printf("rig_set_ptt: error = %s \n", rigerror(retcode));
	}
	Sleep(1);
	
	* Simple examples of getting rig information -- FS
	*
	
	printf("\nGet various raw rig values:\n");
	retcode = rig_get_vfo(my_rig, &vfo); 
	if (retcode == RIG_OK) {
		printf("rig_get_vfo: vfo = %i \n", vfo);
	}
	else {
		printf("rig_get_vfo: error = %s \n", rigerror(retcode));
	}
	retcode = rig_get_freq(my_rig, RIG_VFO_CURR, &freq);
	if (retcode == RIG_OK) {
		printf("rig_get_freq: freq = %d\n", freq);
	}
	else {
		printf("rig_get_freq: error = %s \n", rigerror(retcode));
	}
	retcode = rig_get_mode(my_rig, RIG_VFO_CURR, &rmode, &width);
	if (retcode == RIG_OK) {
		printf("rig_get_mode: mode = %i \n", rmode);
	}
	else {
		printf("rig_get_mode: error = %s \n", rigerror(retcode));
	}
	retcode = rig_get_strength(my_rig, RIG_VFO_CURR, &strength);
	if (retcode == RIG_OK) {
		printf("rig_get_strength: strength = %i \n", strength);
	}
	else {
		printf("rig_get_strength: error = %s \n", rigerror(retcode));
	}
	if (rig_has_set_func(my_rig, RIG_FUNC_RIT)) {
		retcode = rig_set_func(my_rig, RIG_VFO_CURR, RIG_FUNC_RIT, 1);
		printf("rig_set_func: Setting RIT ON\n");
	}
	if (rig_has_get_func(my_rig, RIG_FUNC_RIT)) {
		retcode = rig_get_func(my_rig, RIG_VFO_CURR, RIG_FUNC_RIT, &rit);
		printf("rig_get_func: RIT: %d\n", rit);
	}
	if (rig_has_set_func(my_rig, RIG_FUNC_XIT)) {
		retcode = rig_set_func(my_rig, RIG_VFO_CURR, RIG_FUNC_XIT, 1);
		printf("rig_set_func: Setting XIT ON\n");
	}
	if (rig_has_get_func(my_rig, RIG_FUNC_XIT)) {
		retcode = rig_get_func(my_rig, RIG_VFO_CURR, RIG_FUNC_XIT, &xit);
		printf("rig_get_func: XIT: %d\n", xit);
	}
	rig_close(my_rig); 
	rig_cleanup(my_rig); 
	printf("port %s closed ok \n", SERIAL_PORT);

	return 0;
*/
}

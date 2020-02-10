# server_api.pxd

# Common
cdef extern from "<time.h>":
    pass
cdef extern from "E:\\Projects\\SDRLibE\\trunk\\server\\src\\common\\defs.h":
    pass
cdef extern from "E:\\Projects\\SDRLibE\\trunk\\server\\src\\ringbuffer\\ringb.h":
    pass
cdef extern from "E:\\Projects\\SDRLibE\\libs\\pthreads\\include\\pthread.h":
    pass

# Audio
cdef extern from "E:\\Projects\\SDRLibE\\libs\\portaudio\\include\\portaudio.h":
    pass
cdef extern from "E:\\Projects\\SDRLibE\\trunk\\server\\src\\audio\\local_audio.h":
    pass

cdef extern from "E:\\Projects\\SDRLibE\\trunk\\server\\src\\server\\server.h":
    # Prototypes
    # General
    int c_server_init()
    void c_server_set_in_rate(int rate)
    void c_server_set_out_rate(int rate)
    void c_server_set_iq_blk_sz(int blk_sz)
    void c_server_set_mic_blk_sz(int blk_sz)
    void c_server_set_duplex(int duplex)
    void c_server_set_fft_size(int size)
    void c_server_set_window_type(int window_type)
    void c_server_set_av_mode(int mode)
    void c_server_set_display_width(int width)
    int c_server_start()
    int c_server_terminate()
    int c_radio_discover()
    int c_radio_start(int wbs)
    int c_radio_stop()
    void c_server_mox(int mox_state)
    # RX & TX
    void c_server_set_rx_mode(int channel, int mode)
    void c_server_set_rx_filter_run(int channel, int run)
    void c_server_set_rx_filter_freq(int channel, int low, int high)
    void c_server_set_rx_filter_window(int channel, int window)
    void c_server_set_agc_mode(int channel, int mode)
    void c_server_set_rx_gain(int rx, float gain)
    double c_server_get_rx_meter_data(int channel, int which)
    void c_server_set_tx_mode(int channel, int mode)
    void c_server_set_tx_filter_run(int channel, int run)
    void c_server_set_tx_filter_freq(int channel, int low, int high)
    void c_server_set_tx_filter_window(int channel, int window)
    double c_server_get_tx_meter_data(int channel, int which)
    void c_server_set_mic_gain(float gain)
    void c_server_set_rf_drive(float drive)
    short c_server_get_peak_input_level()
    # Displays
    void c_server_set_display(int ch_id, int display_width)
    int c_server_get_display_data(int display_id, void *display_data)
    void c_server_process_wbs_frame(char *ptr_in_bytes)
    int c_server_get_wbs_data(int width, void *wbs_data)
    # Audio
    #DeviceEnumList* c_server_enum_audio_inputs();
    #DeviceEnumList* c_server_enum_audio_outputs();
    #void c_server_set_audio_route(int direction, char* location, int receiver, char* host_api, char* dev, char* channel);
    #int c_server_clear_audio_routes();
    #int c_server_restart_audio_routes();
    #void c_server_change_audio_outputs(int rx, char* audio_ch);
    #void c_server_revert_audio_outputs();
    #int c_server_local_audio_run(int runstate);
    # Wisdom
    void c_server_make_wisdom(char *dir)
    # CC data
    void c_server_cc_out_alex_attn(int attn)
    void c_server_cc_out_preamp(int preamp)
    void c_server_cc_out_alex_rx_ant(int ant)
    void c_server_cc_out_alex_rx_out(int out)
    void c_server_cc_out_alex_tx_rly(int rly)
    void c_server_cc_out_duplex(int duplex)
    void c_server_cc_out_alex_auto(int state)
    void c_server_cc_out_alex_hpf_bypass(int bypass)
    void c_server_cc_out_lpf_30_20(int setting)
    void c_server_cc_out_lpf_60_40(int setting)
    void c_server_cc_out_lpf_80(int setting)
    void c_server_cc_out_lpf_160(int setting)
    void c_server_cc_out_lpf_6(int setting)
    void c_server_cc_out_lpf_12_10(int setting)
    void c_server_cc_out_lpf_17_15(int setting)
    void c_server_cc_out_hpf_13(int setting)
    void c_server_cc_out_hpf_20(int setting)
    void c_server_cc_out_hpf_9_5(int setting)
    void c_server_cc_out_hpf_6_5(int setting)
    void c_server_cc_out_hpf_1_5(int setting)
    void c_server_cc_out_set_rx_1_freq(unsigned int freq_in_hz)
    void c_server_cc_out_set_rx_2_freq(unsigned int freq_in_hz)
    void c_server_cc_out_set_rx_3_freq(unsigned int freq_in_hz)
    void c_server_cc_out_set_tx_freq(unsigned int freq_in_hz)

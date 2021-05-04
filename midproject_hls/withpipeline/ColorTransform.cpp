#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif

#include "ColorTransform.h"

ColorTransform::ColorTransform( sc_module_name n ): sc_module( n )
{
	SC_THREAD( transform_rgb_to_grey );
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_resultr.clk_rst(i_clk, i_rst);
	i_resultg.clk_rst(i_clk, i_rst);
	i_resultb.clk_rst(i_clk, i_rst);
    o_greyr.clk_rst(i_clk, i_rst);
	o_greyg.clk_rst(i_clk, i_rst);
	o_greyb.clk_rst(i_clk, i_rst);
#endif
}

void ColorTransform::transform_rgb_to_grey() {
	{
#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_resultr.reset();
		i_resultg.reset();
		i_resultb.reset();
		o_greyr.reset();
		o_greyg.reset();
		o_greyb.reset();
#endif
		wait();
	}
	while (true) {
#ifndef NATIVE_SYSTEMC
		sc_dt::sc_uint<8> R = i_resultr.get();
		sc_dt::sc_uint<8> G = i_resultg.get();
		sc_dt::sc_uint<8> B = i_resultb.get();
#else
		sc_dt::sc_uint<8> R = i_resultr.read();
		sc_dt::sc_uint<8> G = i_resultg.read();
		sc_dt::sc_uint<8> B = i_resultb.read();
#endif
		sc_uint<32> grey_r = (R+G+B)/3;
		sc_uint<32> grey_g = (R+G+B)/3;
		sc_uint<32> grey_b = (R+G+B)/3;
		wait();
#ifndef NATIVE_SYSTEMC
		o_greyr.put(grey_r);
		o_greyg.put(grey_g);
		o_greyb.put(grey_b);
#else
		o_greyr.write(grey_r);
		o_greyg.write(grey_g);
		o_greyb.write(grey_b);
#endif
	}
}

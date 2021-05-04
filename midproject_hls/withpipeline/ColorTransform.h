#ifndef COLOR_TRANSFORM_H_
#define COLOR_TRANSFORM_H_
#include <systemc>
using namespace sc_core;

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

class ColorTransform: public sc_module
{
public:
	sc_in_clk i_clk;
	sc_in < bool >  i_rst;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_uint<8> >::in i_resultr;
	cynw_p2p< sc_dt::sc_uint<8> >::in i_resultg;
	cynw_p2p< sc_dt::sc_uint<8> >::in i_resultb;
	cynw_p2p< sc_dt::sc_uint<32> >::out o_greyr;
	cynw_p2p< sc_dt::sc_uint<32> >::out o_greyg;
	cynw_p2p< sc_dt::sc_uint<32> >::out o_greyb;
#else
	sc_fifo_in< sc_dt::sc_uint<8> > i_resultr;
	sc_fifo_in< sc_dt::sc_uint<8> > i_resultg;
	sc_fifo_in< sc_dt::sc_uint<8> > i_resultb;
	sc_fifo_out< sc_dt::sc_uint<32> > o_greyr;
	sc_fifo_out< sc_dt::sc_uint<32> > o_greyg;
	sc_fifo_out< sc_dt::sc_uint<32> > o_greyb;
#endif

	SC_HAS_PROCESS( ColorTransform );
	ColorTransform( sc_module_name n );
private:
	void transform_rgb_to_grey();
};
#endif

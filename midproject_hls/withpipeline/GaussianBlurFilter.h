#ifndef IMAGE_GRADIENT_H_
#define IMAGE_GRADIENT_H_
#include <systemc>
using namespace sc_core;
using namespace sc_core;
using namespace sc_dt;
#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

#include "filter_def.h"

class GaussianBlurFilter: public sc_module
{
public:
	sc_in_clk i_clk;
	sc_in < bool >  i_rst;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_uint<8> >::in i_r;
	cynw_p2p< sc_dt::sc_uint<8> >::in i_g;
	cynw_p2p< sc_dt::sc_uint<8> >::in i_b;
	cynw_p2p< sc_dt::sc_uint<8> >::out o_resultr;
	cynw_p2p< sc_dt::sc_uint<8> >::out o_resultg;
	cynw_p2p< sc_dt::sc_uint<8> >::out o_resultb;
#else
	sc_fifo_out<sc_uint<8> > o_resultr;
    sc_fifo_out<sc_uint<8> > o_resultg;
    sc_fifo_out<sc_uint<8> > o_resultb;
    sc_fifo_in<sc_uint<8> > i_r;
    sc_fifo_in<sc_uint<8> > i_g;
    sc_fifo_in<sc_uint<8> > i_b;
#endif

	SC_HAS_PROCESS( GaussianBlurFilter );
	GaussianBlurFilter( sc_module_name n );
	~GaussianBlurFilter();
private:
	void do_filter();
    int val[MASK_N];
};
#endif

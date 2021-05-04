#ifndef SOBEL_FILTER_H_
#define SOBEL_FILTER_H_
#include <systemc>
using namespace std;
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
	cynw_p2p< sc_dt::sc_uint<8> >::out o_r;
	cynw_p2p< sc_dt::sc_uint<8> >::out o_g;
	cynw_p2p< sc_dt::sc_uint<8> >::out o_b;
#else
	sc_fifo_out<sc_uint<8> > o_r;
    sc_fifo_out<sc_uint<8> > o_g;
    sc_fifo_out<sc_uint<8> > o_b;
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
  sc_time t1;
  sc_time t2;
};
#endif

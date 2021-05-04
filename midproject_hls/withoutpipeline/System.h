#ifndef SYSTEM_H_
#define SYSTEM_H_
#include <systemc>
#include <string>
#include "filter_def.h"
using namespace sc_core;

#include "Testbench.h"
#ifndef NATIVE_SYSTEMC
#include "GaussianBlurFilter_wrap.h"
#else
#include "GaussianBlurFilter.h"
#endif

class System: public sc_module
{
public:
	SC_HAS_PROCESS( System );
	System( sc_module_name n, std::string input_bmp, std::string output_bmp );
	~System();
private:
  Testbench tb;
#ifndef NATIVE_SYSTEMC
	GaussianBlurFilter_wrapper GaussianBlurFilter;
#else
	GaussianBlurFilter GaussianBlurFilter;
#endif
	sc_clock clk;
	sc_signal<bool> rst;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_uint<8> > source_r;
	cynw_p2p< sc_dt::sc_uint<8> > source_g;
	cynw_p2p< sc_dt::sc_uint<8> > source_b;
	cynw_p2p< sc_dt::sc_uint<8> > result_r;
	cynw_p2p< sc_dt::sc_uint<8> > result_g;
	cynw_p2p< sc_dt::sc_uint<8> > result_b;
#else
	cynw_p2p< sc_dt::sc_uint<8> > source_r;
	cynw_p2p< sc_dt::sc_uint<8> > source_g;
	cynw_p2p< sc_dt::sc_uint<8> > source_b;
	cynw_p2p< sc_dt::sc_uint<8> > result_r;
	cynw_p2p< sc_dt::sc_uint<8> > result_g;
	cynw_p2p< sc_dt::sc_uint<8> > result_b;
#endif

	std::string _output_bmp;
};
#endif

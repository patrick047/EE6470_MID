#ifndef SYSTEM_PIPELINE_H_
#define SYSTEM_PIPELINE_H_
#include <systemc>
using namespace sc_core;

#include "Testbench.h"
#ifndef NATIVE_SYSTEMC
#include "ColorTransform_wrap.h"
#include "GaussianBlurFilter_wrap.h"
#else
#include "ColorTransform.h"
#include "GaussianBlurFilter.h"
#endif

class SystemPipeline: public sc_module
{
public:
	SC_HAS_PROCESS( SystemPipeline );
	SystemPipeline( sc_module_name n, std::string input_bmp, std::string output_bmp );
	~SystemPipeline();
private:
  Testbench tb;
#ifndef NATIVE_SYSTEMC
	ColorTransform_wrapper color_transform;
	GaussianBlurFilter_wrapper GaussianBlur_Filter;
#else
	ColorTransform color_transform;
	GaussianBlurFilter GaussianBlur_Filter;
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
	cynw_p2p< sc_dt::sc_uint<32> > grey_r;
	cynw_p2p< sc_dt::sc_uint<32> > grey_g;
	cynw_p2p< sc_dt::sc_uint<32> > grey_b;
	
#else
	cynw_p2p< sc_dt::sc_uint<8> > source_r;
	cynw_p2p< sc_dt::sc_uint<8> > source_g;
	cynw_p2p< sc_dt::sc_uint<8> > source_b;
	cynw_p2p< sc_dt::sc_uint<8> > result_r;
	cynw_p2p< sc_dt::sc_uint<8> > result_g;
	cynw_p2p< sc_dt::sc_uint<8> > result_b;
	sc_fifo< sc_dt::sc_uint<32> > grey_r;
	sc_fifo< sc_dt::sc_uint<32> > grey_g;
	sc_fifo< sc_dt::sc_uint<32> > grey_b;
	
#endif

	std::string _output_bmp;
};
#endif

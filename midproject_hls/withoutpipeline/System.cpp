#include "System.h"
System::System( sc_module_name n, string input_bmp, string output_bmp ): sc_module( n ), 
	tb("tb"), GaussianBlurFilter("GaussianBlurFilter"), clk("clk", CLOCK_PERIOD, SC_NS), rst("rst"), _output_bmp(output_bmp)
{
	tb.i_clk(clk);
	tb.o_rst(rst);
	GaussianBlurFilter.i_clk(clk);
	GaussianBlurFilter.i_rst(rst);
	tb.o_r(source_r);
    tb.o_g(source_g);
    tb.o_b(source_b);
    tb.i_r(result_r);
    tb.i_g(result_g);
    tb.i_b(result_b);
	GaussianBlurFilter.i_r(source_r);
	GaussianBlurFilter.i_g(source_g);
	GaussianBlurFilter.i_b(source_b);
	GaussianBlurFilter.o_r(result_r);
	GaussianBlurFilter.o_g(result_g);
	GaussianBlurFilter.o_b(result_b);

  tb.read_bmp(input_bmp);
}

System::~System() {
  tb.write_bmp(_output_bmp);
}

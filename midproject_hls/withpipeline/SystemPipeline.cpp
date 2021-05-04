#include "SystemPipeline.h"
SystemPipeline::SystemPipeline( sc_module_name n, string input_bmp, string output_bmp ): sc_module( n ), 
	tb("tb"), color_transform("color_transform"), GaussianBlur_Filter("GaussianBlur_Filter"), 
	clk("clk", CLOCK_PERIOD, SC_NS), rst("rst"), _output_bmp(output_bmp)
{
	tb.i_clk(clk);
	tb.o_rst(rst);
	tb.i_greyr(grey_r);
	tb.i_greyg(grey_g);
	tb.i_greyb(grey_b);
	tb.o_r(source_r);
    tb.o_g(source_g);
    tb.o_b(source_b);

    GaussianBlur_Filter.i_clk(clk);
	GaussianBlur_Filter.i_rst(rst);
	GaussianBlur_Filter.i_r(source_r);
	GaussianBlur_Filter.i_g(source_g);
	GaussianBlur_Filter.i_b(source_b);
	GaussianBlur_Filter.o_resultr(result_r);
	GaussianBlur_Filter.o_resultg(result_g);
	GaussianBlur_Filter.o_resultb(result_b);

	color_transform.i_clk(clk);
	color_transform.i_rst(rst);
	color_transform.i_resultr(result_r);
	color_transform.i_resultg(result_g);
	color_transform.i_resultb(result_b);
	color_transform.o_greyr(grey_r);
	color_transform.o_greyg(grey_g);
	color_transform.o_greyb(grey_b);


  tb.read_bmp(input_bmp);
}

SystemPipeline::~SystemPipeline() {
  tb.write_bmp(_output_bmp);
}

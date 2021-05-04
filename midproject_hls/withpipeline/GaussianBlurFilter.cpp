#include <cmath>
#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif
#include "GaussianBlurFilter.h"


GaussianBlurFilter::GaussianBlurFilter(sc_module_name n) : sc_module(n)
{
#ifndef NATIVE_SYSTEMC
	HLS_FLATTEN_ARRAY(val);
#endif
	SC_THREAD(do_filter);
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_r.clk_rst(i_clk, i_rst);
	i_g.clk_rst(i_clk, i_rst);
	i_b.clk_rst(i_clk, i_rst);
    o_resultr.clk_rst(i_clk, i_rst);
    o_resultg.clk_rst(i_clk, i_rst);
    o_resultb.clk_rst(i_clk, i_rst);
#endif
}
GaussianBlurFilter::~GaussianBlurFilter() {}
//  GaussianBlurFilter(5x5) mask
const int filter[MASK_X][MASK_Y] = {
	{1, 4, 6, 4, 1},
	{4, 16, 24, 16, 4},
	{6, 24, 36, 24, 6},
	{4, 16, 24, 16, 4},
	{1, 4, 6, 4, 1}
	};

const int factor = 256;

void GaussianBlurFilter::do_filter()
{
	sc_uint<8> source_r, source_g, source_b, i, j;
	sc_uint<32> R, G, B; // color of R, G, B
	{
#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_r.reset();
		i_g.reset();
		i_b.reset();
		o_resultr.reset();
		o_resultg.reset();
		o_resultb.reset();
#endif
		wait();
	}
	while (true)
	{
		R = G = B = 0;

		for (unsigned int v = 0; v < MASK_Y; ++v)
		{
			for (unsigned int u = 0; u < MASK_X; ++u)
			{
				
#ifndef NATIVE_SYSTEMC
				{
					HLS_DEFINE_PROTOCOL("input");
					source_r = i_r.get();
                    source_g = i_g.get();
                    source_b = i_b.get();
					wait();
				}
#else
				    source_r = i_r.read();
                    source_g = i_g.read();
                    source_b = i_b.read();
#endif
				{
					HLS_CONSTRAIN_LATENCY(0, 1, "lat01");
					R += (sc_uint<32>)source_r * filter[u][v];
					G += (sc_uint<32>)source_g * filter[u][v];
					B += (sc_uint<32>)source_b * filter[u][v];
				}
			}
		}
		
#ifndef NATIVE_SYSTEMC
		{
			HLS_DEFINE_PROTOCOL("output");
			o_resultr.put((sc_uint<8>)(R/factor));
            o_resultg.put((sc_uint<8>)(G/factor));
            o_resultb.put((sc_uint<8>)(B/factor));
			wait();
		}
#else
		o_resultr.write((sc_uint<8>)(R/factor));
        o_resultg.write((sc_uint<8>)(G/factor));
        o_resultb.write((sc_uint<8>)(B/factor));
#endif
	}
}

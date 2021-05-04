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
    o_r.clk_rst(i_clk, i_rst);
    o_g.clk_rst(i_clk, i_rst);
    o_b.clk_rst(i_clk, i_rst);
#endif
}

GaussianBlurFilter::~GaussianBlurFilter() {}

// sobel mask
const int filter[MASK_X][MASK_Y] ={
  {1,  4,  6,  4,  1},
  {4, 16, 24, 16,  4},
  {6, 24, 36, 24,  6},
  {4, 16, 24, 16,  4},
  {1,  4,  6,  4,  1}
};

const int factor = 256;


void GaussianBlurFilter::do_filter()
{
	sc_uint<8> source_r, source_g, source_b, i, j;
	//sc_uint<24> source_rgb, result_rgb;
	sc_uint<32> R, G, B; // color of R, G, B
	t1 = SC_ZERO_TIME;
	t2 = SC_ZERO_TIME;
	{
#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_r.reset();
		i_g.reset();
		i_b.reset();
		o_r.reset();
		o_g.reset();
		o_b.reset();
#endif
		wait();
	}
	while (true)
	{
	    t1 = sc_time_stamp();
		R = G = B = 0;
		for (unsigned int v = 0; v < 5; ++v)
		{
			for (unsigned int u = 0; u < 5; ++u)
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
		    t2 = sc_time_stamp() - t1;
		}
		std::cout<< t2  <<std::endl;

#ifndef NATIVE_SYSTEMC
		{
			HLS_DEFINE_PROTOCOL("output");
            sc_uint<32> grey=(R+G+B)/3;
            //std::cout <<std::setw(12)<< red/16 <<std::setw(12)<<green/16std::endl;
			o_r.put((sc_uint<8>)(grey/factor));
            o_g.put((sc_uint<8>)(grey/factor));
            o_b.put((sc_uint<8>)(grey/factor));
			wait();
		}
#else
		o_r.write((sc_uint<8>)(R/factor));
        o_g.write((sc_uint<8>)(G/factor));
        o_b.write((sc_uint<8>)(B/factor));
#endif
	}
}

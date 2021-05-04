#include <cmath>
#include <algorithm>
#include <cstdio>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <systemc>
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
	//i_width.clk_rst(i_clk, i_rst);
#endif
}

GaussianBlurFilter::~GaussianBlurFilter() {}

// GaussianBliur(5x5) mask
const int filter[MASK_X][MASK_Y] = {
	{1, 4, 6, 4, 1},
	{4, 16, 24, 16, 4},
	{6, 24, 36, 24, 6},
	{4, 16, 24, 16, 4},
	{1, 4, 6, 4, 1}};

const int factor = 256;

void GaussianBlurFilter::do_filter()
{
	{
		wait();
	}
	// sc_uint<32> width = i_width.get();
	// sc_uint<32> R, G, B;
	// int r[5][width], g[5][width], b[5][width];
	// int m, n;
	// int flag = 0;
	{
#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_r.reset();
		i_g.reset();
		i_b.reset();
		o_resultr.reset();
		o_resultg.reset();
		o_resultb.reset();
		//i_width.reset();
#endif
		wait();
	}
	//int width = i_width.get();
	// std::cout <<  width  << std::endl;
	sc_uint<32> R, G, B;
	//unsigned char r[5][widthx], g[5][widthx], b[5][widthx];
	int m, n;
	int flag = 0;
	for (m = 0; m < MASK_X; ++m) //setup array(m列n行)
	{
		for (n = 0; n < widthx; ++n)
		{
			r[m][n] = 0;
			g[m][n] = 0;
			b[m][n] = 0;
		}
	}
	while (true)
	{
		if (flag == 0)
		{
			for (m = 0; m < MASK_X; ++m)
			{
				for (n = 0; n < widthx; ++n)
				{
#ifndef NATIVE_SYSTEMC
					{
						HLS_DEFINE_PROTOCOL("input");
						r[m][n] = i_r.get();
						g[m][n] = i_g.get();
						b[m][n] = i_b.get();
						wait();
					}
#else
					r[m][n] = i_r.read();
					g[m][n] = i_g.read();
					b[m][n] = i_b.read();
					wait();
#endif
				}
				flag = 1;
			}
		}
		if (flag != 0)
		{
			for (int n = 0; n < widthx; ++n)
			{

				r[0][n] = r[1][n];
				g[0][n] = g[1][n];
				b[0][n] = b[1][n];
				r[1][n] = r[2][n];
				g[1][n] = g[2][n];
				b[1][n] = b[2][n];
				r[2][n] = r[3][n];
				g[2][n] = g[3][n];
				b[2][n] = b[3][n];
				r[3][n] = r[4][n];
				g[3][n] = g[4][n];
				b[3][n] = b[4][n];
				r[4][n] = g[4][n] = b[4][n] = 0;
			}

			for (int n = 0; n < widthx; ++n)
			{
#ifndef NATIVE_SYSTEMC

				r[4][n] = i_r.get();
				g[4][n] = i_g.get();
				b[4][n] = i_b.get();
#else
				r[4][n] = i_r.read();
				g[4][n] = i_g.read();
				b[4][n] = i_b.read();
#endif
			}
			wait();

			
		}

		wait();
		for (int d = 0; d < widthx; ++d)
		{
			R = G = B = 0;
			wait();
			for (unsigned int v = 0; v < 5; ++v)
			{
				for (unsigned int u = 0; u < 5; ++u)
				{

					{
						//HLS_CONSTRAIN_LATENCY(0, 1, "lat01");
						R += r[u][v + d] * filter[u][v];
						G += g[u][v + d] * filter[u][v];
						B += b[u][v + d] * filter[u][v];
					}
				}
			}

#ifndef NATIVE_SYSTEMC
			{
				HLS_DEFINE_PROTOCOL("output");
				sc_uint<32> grey = (R + G + B) / 3;
				o_resultr.put((sc_uint<8>)(grey / factor));
				o_resultg.put((sc_uint<8>)(grey / factor));
				o_resultb.put((sc_uint<8>)(grey / factor));
				wait();
			}
#else
			o_resultr.write((sc_uint<8>)(grey / factor));
			o_resultg.write((sc_uint<8>)(grey / factor));
			o_resultb.write((sc_uint<8>)(grey / factor));
#endif
		}
	}
}

#include <cmath>
#include <iomanip>
#include <systemc>
#include "GaussianBlurFilter.h"
#include "filter_def.h"

GaussianBlurFilter::GaussianBlurFilter(sc_module_name n) : sc_module(n), t_skt("t_skt"), base_offset(0)
{
  SC_THREAD(do_GaussianBlurFilter);
  t_skt.register_b_transport(this, &GaussianBlurFilter::blocking_transport);
}

// GaussianBlurFilter mask
const int filter[MASK_X][MASK_Y] ={
  {1,  4,  6,  4,  1},
  {4, 16, 24, 16,  4},
  {6, 24, 36, 24,  6},
  {4, 16, 24, 16,  4},
  {1,  4,  6,  4,  1}
};

const int factor = 256;

void GaussianBlurFilter::do_GaussianBlurFilter()
{
  {
    wait(CLOCK_PERIOD, SC_NS);
  }
  width = i_width.read();

  int r[5][width], g[5][width], b[5][width];
  int m, n;
  int flag = 0;
  for (m = 0; m < MASK_X; ++m) //setup array(m列n行)
  {
    for (n = 0; n < width; ++n)
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
        for (n = 0; n < width; ++n)
        {
          r[m][n] = i_r.read();
          g[m][n] = i_g.read();
          b[m][n] = i_b.read();
        }
      }
      flag = 1;
    }
    if (flag != 0)
    {

      for (int n = 0; n < width; ++n)
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

      for (int n = 0; n < width; ++n)
      {
        r[4][n] = i_r.read();
        g[4][n] = i_g.read();
        b[4][n] = i_b.read();
      }
    }

    wait(CLOCK_PERIOD, SC_NS);

    for (int d = 0; d < width; ++d)
    {
      double red = 0.0, green = 0.0, blue = 0.0;
      wait(CLOCK_PERIOD, SC_NS);
      for (unsigned int v = 0; v < MASK_Y; ++v)
      {
        for (unsigned int u = 0; u < MASK_X; ++u)
        {

          red += r[u][v + d] * filter[u][v];
          green += g[u][v + d] * filter[u][v];
          blue += b[u][v + d] * filter[u][v];

          wait(CLOCK_PERIOD, SC_NS);
        }
      }
      std::cout << std::setw(12) << red / 16 << std::setw(12) << green / 16 << std::setw(12) << blue / 16 << std::endl;
      unsigned int grey=(red+green+blue)/3;
      o_result_r.write(grey / factor);
      o_result_g.write(grey / factor);
      o_result_b.write(grey / factor);
    }
  }
}

void GaussianBlurFilter::blocking_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay)
{
  sc_dt::uint64 addr = payload.get_address();
  addr = addr - base_offset;
  unsigned char *mask_ptr = payload.get_byte_enable_ptr();
  unsigned char *data_ptr = payload.get_data_ptr();
  word buffer;

  

  switch (payload.get_command())
  {
  case tlm::TLM_READ_COMMAND:
    switch (addr)
    {
    case GaussianBlur_FILTER_RESULT_ADDR:
      buffer.uc[0] = o_result_r.read();
      buffer.uc[1] = o_result_g.read();
      buffer.uc[2] = o_result_b.read();

      break;
    default:
      std::cerr << "Error! SobelFilter::blocking_transport: address 0x"
                << std::setfill('0') << std::setw(8) << std::hex << addr
                << std::dec << " is not valid" << std::endl;
      break;
    }
    data_ptr[0] = buffer.uc[0];
    data_ptr[1] = buffer.uc[1];
    data_ptr[2] = buffer.uc[2];
    data_ptr[3] = buffer.uc[3];

    break;

  case tlm::TLM_WRITE_COMMAND:
    switch (addr)
    {
    case GaussianBlur_FILTER_R_ADDR:
      if (flag == 0)
      {
         buffer.uc[0]=data_ptr[0] ;
         buffer.uc[1]=data_ptr[1] ;
         buffer.uc[2]=data_ptr[2] ;
         buffer.uc[3]=data_ptr[3] ;
        i_width.write(buffer.uint);
        flag = 1;
      }
      else //(flag = 1)
      {
         
        if (mask_ptr[0] == 0xff)
        {
          i_r.write(data_ptr[0]);
        }
        if (mask_ptr[1] == 0xff)
        {
          i_g.write(data_ptr[1]);
        }
        if (mask_ptr[2] == 0xff)
        {
          i_b.write(data_ptr[2]);
        }
      }
      break;
    default:
      std::cerr << "Error! SobelFilter::blocking_transport: address 0x"
                << std::setfill('0') << std::setw(8) << std::hex << addr
                << std::dec << " is not valid" << std::endl;
      break;
    }
    break;

  case tlm::TLM_IGNORE_COMMAND:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  default:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  }
  payload.set_response_status(tlm::TLM_OK_RESPONSE); // Always OK
}
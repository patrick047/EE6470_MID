
#ifndef GaussianBlur_FILTER_H_
#define GaussianBlur_FILTER_H_
#include <systemc>
using namespace sc_core;
#include "tlm"
#include "tlm_utils/simple_target_socket.h"
#include "filter_def.h"

class GaussianBlurFilter : public sc_module {
public:
  tlm_utils::simple_target_socket<GaussianBlurFilter> t_skt;

  sc_fifo<unsigned char> i_r;
  sc_fifo<unsigned char> i_g;
  sc_fifo<unsigned char> i_b;
  sc_fifo<unsigned char> o_result_r;
  sc_fifo<unsigned char> o_result_g;
  sc_fifo<unsigned char> o_result_b;
  sc_fifo<unsigned int> i_width;  

  SC_HAS_PROCESS(GaussianBlurFilter);
  GaussianBlurFilter(sc_module_name n);
  ~GaussianBlurFilter() = default;

private:
  void do_GaussianBlurFilter();
  double red = 0.0, green = 0.0, blue = 0.0;
  int width = 0;
  int flag = 0;
  unsigned int base_offset;
  void blocking_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay);
};
#endif
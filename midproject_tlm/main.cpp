#include <iostream>
#include <string>
using namespace std;

// Wall Clock Time Measurement
#include <sys/time.h>

#include "GaussianBlurFilter.h"
#include "Testbench.h"
#include "SimpleBus.h" //lab04

// TIMEVAL STRUCT IS Defined ctime
// use start_time and end_time variables to capture
// start of simulation and end of simulation
struct timeval start_time, end_time;

// int main(int argc, char *argv[])
int sc_main(int argc, char **argv)
{
  if ((argc < 3) || (argc > 4))
  {
    cout << "No arguments for the executable : " << argv[0] << endl;
    cout << "Usage : >" << argv[0] << " in_image_file_name out_image_file_name"<< endl;
    return 0;
  }
  Testbench tb("tb");
  SimpleBus<1, 1> bus("bus"); //lab04
  bus.set_clock_period(sc_time(CLOCK_PERIOD, SC_NS));  //lab04
  GaussianBlurFilter GaussianBlur_filter("GaussianBlur_filter");
  tb.initiator.i_skt(bus.t_skt[0]);
  bus.setDecode(0, GaussianBlur_MM_BASE, GaussianBlur_MM_BASE + GaussianBlur_MM_SIZE - 1);  //lab04
  bus.i_skt[0](GaussianBlur_filter.t_skt);  //lab04

  tb.read_bmp(argv[1]);
  sc_start();
  std::cout << "Simulated time == " << sc_core::sc_time_stamp() << std::endl;
  tb.write_bmp(argv[2]);

  return 0;
}

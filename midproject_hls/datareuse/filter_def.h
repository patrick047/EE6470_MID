#ifndef FILTER_DEF_H_
#define FILTER_DEF_H_

#define MASK_N 2
#define MASK_X 5
#define MASK_Y 5
#define widthx 256

const int GaussianBlur_FILTER_R_ADDR = 0x00000000;
const int GaussianBlur_FILTER_RESULT_ADDR = 0x00000004;

union word {
  int sint;
  unsigned int uint;
  unsigned char uc[4];
};
#endif

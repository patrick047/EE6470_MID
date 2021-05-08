# EE6470 Midterm project
### Gaussian Blur Filter(5x5) with grey
student:趙庭慶 number:109061616

## Implemation
### 5x5 Gaussian Blur Filter(systemC) on HLS
#### Without pipeline and data reuse alogrithm
##### Structure
![](https://github.com/patrick047/EE6470_MID/blob/main/%E5%9C%96%E7%89%871.png)
#### With pipeline
##### Structure
![](https://github.com/patrick047/EE6470_MID/blob/main/messageImage_1619888669490.jpg)
#### With data reuse alogrithm
##### Structure
![](https://github.com/patrick047/EE6470_MID/blob/main/%E5%9C%96%E7%89%871.png)
##### Alogrithm
![](https://github.com/patrick047/EE6470_MID/blob/main/alogrithm1.PNG)
![](https://github.com/patrick047/EE6470_MID/blob/main/alogrithm2.PNG)
### 5x5 Gaussian Blur Filter(systemC) on TLM 2.0 bus
#### With data reuse alogrithm
##### Structure
![](https://github.com/patrick047/EE6470_MID/blob/main/messageImage_1619890635320.jpg)

## Results 
### Comparison
![](https://github.com/patrick047/EE6470_MID/blob/main/comparison1.PNG)
### Image
#### Original
![](https://github.com/patrick047/EE6470_MID/blob/main/ph3.png)
#### After 3x3 Gaussian Blur Filter
![](https://github.com/patrick047/EE6470_MID/blob/main/ph4.png)
#### After 5x5 Gaussian Blur Filter
![](https://github.com/patrick047/EE6470_MID/blob/main/ph5.png)
#### After 3x3 Gaussian Blur Filter with grey
![](https://github.com/patrick047/EE6470_MID/blob/main/%E5%9C%96%E7%89%872.png)

## Source Code Introduction
#### GaussianBlurFilter(5x5) mask
```
const int filter[MASK_X][MASK_Y] = {
	{1, 4, 6, 4, 1},
	{4, 16, 24, 16, 4},
	{6, 24, 36, 24, 6},
	{4, 16, 24, 16, 4},
	{1, 4, 6, 4, 1}};

const int factor = 256;

```

#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <stdlib.h>

/*
Note, this file was taken direcly from my 2d game at
https://github.com/EricKnudsen256/2DSpring2021/tree/SummerWork01
*/


void init_random();

/**
* @brief returns a random float between 0 and 1
*/
float random();


/**
* @brief returns a int between int min and int max
*/
int random_int_range(int min, int max);


#endif
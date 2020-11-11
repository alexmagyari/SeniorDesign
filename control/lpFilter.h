/*
Author: Leslie Nix
Date Created: 27 October 2020
Date Updated: 29 October 2020
Status: Probably Fine

Low Pass Filter
*/

#ifndef LP_FILTER_H_
#define LP_FILTER_H_

typedef struct{
  float prevOutput, Fc; //cutoff frequency in Hz
} low_pass_t;

float applyLowPass(low_pass_t *low_pass, float input, float dt);

#endif

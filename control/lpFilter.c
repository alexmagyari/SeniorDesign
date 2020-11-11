/*
Author: Leslie Nix
Date Created: 27 October 2020
Date Updated: 29 October 2020
Status: Probably Fine

Approximates First-Order Low Pass Filter:
H(s) = 1/(tau*s +1)

prevOutput is previous output from filter
input is new input to filter
Fc is cuttoff frequency in Hz
dt is time between last filter time
*/

#include "lpFilter.h"

#define M_PIf 3.141592653897932384f

float applyLowPass(low_pass_t *low_pass, float input, float dt){
  //time constant
  const float tau = 1.0f/(2.0f*M_PIf*low_pass->Fc);

  const float alpha = dt/(tau + dt);

  // y(n) = y(n-1) + alpha*(u(n) - y(n))
  float output = low_pass->prevOutput + alpha*(input - low_pass->prevOutput);
  low_pass->prevOutput = output;
  return output;
}

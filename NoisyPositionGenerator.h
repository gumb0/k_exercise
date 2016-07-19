#ifndef __noisy_position_generator_h__
#define __noisy_position_generator_h__

#include "PositionGenerator.h"

// Decorator adding noise to original signal

PositionGeneratorPtr CreateNoisyPositionGenerator(PositionGeneratorPtr generator, double amplitudeMeters);

#endif
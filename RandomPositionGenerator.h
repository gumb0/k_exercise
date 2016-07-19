#ifndef __random_position_generator_h__
#define __random_position_generator_h__

#include "PositionGenerator.h"

PositionGeneratorPtr CreateRandomPositionGenerator(int sensorId, double frequencyHz, double playingFieldSizeMeters, 
	double maxSpeedMetersPerSec);

#endif
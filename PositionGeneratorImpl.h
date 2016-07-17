#ifndef __position_generator_impl_h__
#define __position_generator_impl_h__

#include "PositionGenerator.h"

// TODO maybe RandomPositionGenerator ?
PositionGeneratorPtr CreateRandomPositionGenerator(int sensorId, double frequencyHz, double playingFieldSizeMeters, 
	double maxSpeedMetersPerSec);

#endif
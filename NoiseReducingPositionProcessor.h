#ifndef __noise_reducing_position_processor_impl_h__
#define __noise_reducing_position_processor_impl_h__

#include "PositionProcessor.h"

// Decorator compensating noise before passing it to original processor

PositionProcessorPtr CreateNoiseReducingPositionProcessor(PositionProcessorPtr processor, int filterLength);

#endif
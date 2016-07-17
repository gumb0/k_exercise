CC=g++
CFLAGS=-Wall -std=c++14 -O3 -o position_stats

main: 
	$(CC) $(CFLAGS) \
	main.cpp \
	Common.cpp \
	PositionGeneratorImpl.cpp \
	PositionGeneratorRunner.cpp \
	PositionFileLogger.cpp \
	PositionProcessorImpl.cpp \
	StatisticsFileLogger.cpp \
	NoisyPositionGenerator.cpp \
	NoiseReducingPositionProcessor.cpp

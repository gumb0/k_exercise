CC=g++
CFLAGS=-Wall -std=c++14 -pthread -O3 -o position_stats

main: 
	$(CC) $(CFLAGS) \
	main.cpp \
	Common.cpp \
	RandomPositionGenerator.cpp \
	PositionGeneratorGroup.cpp \
	PositionFileLogger.cpp \
	PositionProcessor.cpp \
	StatisticsFileLogger.cpp \
	NoisyPositionGenerator.cpp \
	NoiseReducingPositionProcessor.cpp

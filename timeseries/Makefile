.PHONY: all clean

all: 
	for dir in 'AnomalyDetectorSample' 'AutoForecasterSamples' 'DSPFilterBandpassExample' 'DSPFilterFiniteSample' 'KMeansClusteringControlSignalSample' 'STD2Samples'; do\
		make -C $$dir || exit 1; \
	done

clean:
	for dir in 'AnomalyDetectorSample' 'AutoForecasterSamples' 'DSPFilterBandpassExample' 'DSPFilterFiniteSample' 'KMeansClusteringControlSignalSample' 'STD2Samples'; do\
		make -C $$dir clean || exit 1; \
	done


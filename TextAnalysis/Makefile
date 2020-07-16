.PHONY: all clean

all: 
	for dir in 'BigInsightsStarterKitApp' 'TextAnalyticsToGo'; do\
		make -C $$dir || exit 1; \
	done

clean:
	for dir in 'BigInsightsStarterKitApp' 'TextAnalyticsToGo'; do\
		make -C $$dir clean || exit 1; \
	done


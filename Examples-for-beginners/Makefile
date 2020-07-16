.PHONY: all clean

all: 
	for dir in $(shell ls -d -1 */); do\
		make -C $$dir || exit 1; \
	done

clean:
	for dir in $(shell ls -d -1 */); do\
		make -C $$dir clean || exit 1; \
	done


JAVA_HOME = /usr/local/openjdk8
OUTPUT_DIR = target
LIBS = $(OUTPUT_DIR)/libserial.so

all: $(LIBS)

clean:
	rm $(LIBS)

$(OUTPUT_DIR)/libserial.so:
	mkdir -p $(dir $@)
	cc -fPIC -D_REENTRANT -g -O3 -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/freebsd -shared -o $@ serial.c

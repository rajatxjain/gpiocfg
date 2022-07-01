CFLAGS = -Wall

HEADERS = gpiocfg.h
OBJECTS = gpiocfg.o icelake.o kabylake.o cometlake.o jasperlake.o tigerlake.o alderlake.o

default: gpiocfg
all: default

%.o: %.c $(HEADERS)
	gcc -c $< -o $@

gpiocfg: $(OBJECTS)
	gcc $(OBJECTS) -o $@

clean:
	rm -f $(OBJECTS)
	rm -f gpiocfg 

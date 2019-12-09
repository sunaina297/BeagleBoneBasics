CC_C = arm-linux-gnueabihf-gcc
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror
LIBS= -pthread

TARGETS= as2 sorter udp
PUBDIR = $(HOME)/cmpt433/public/myApps
OUTDIR = $(PUBDIR)
OUTPUTS= $(addprefix $(OUTDIR)/, $(TARGETS))


all: main

%.o : %.c
	$(CC_C) -c $(CFLAGS) $<

main: as2.o sorter.o udp.o sort.o helper.o beagle.o
	$(CC_C) -o as2 as2.o sorter.o udp.o sort.o helper.o beagle.o $(LIBS)
	cp as2 $(OUTDIR)


clean:
		rm $(OUTDIR)/as2
		rm sorter.o
		rm as2.o
		rm udp.o
		rm sort.o
		rm helper.o
		rm beagle.o

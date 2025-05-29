NAMES=f05x10 f06x12 f07x14 f08x16 f09x17 f12x24
FONTS=$(patsubst %, %.pcf.gz, ${NAMES})

CXXFLAGS=-std=gnu++11

all: ${FONTS} ${BFONTS} make_table


install: 
	cp $(FONTS) /usr/share/fonts/bitmap/efont-slazav

%.pcf.gz: %.bdf
	bdftopcf $< | gzip -9c > $@.tmp && \
	mv $@.tmp $@

clean:
	rm -f *.BAK *.pcf.gz make_table
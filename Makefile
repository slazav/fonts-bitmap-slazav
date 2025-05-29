NAMES=h10 h12 h14 h16 h17 h24
FONTS=$(patsubst %, %.pcf.gz, ${SIZES})

CXXFLAGS=-std=gnu++11

all: ${FONTS} ${BFONTS} make_table


install: 
	cp $(FONTS) /usr/share/fonts/bitmap/efont-slazav

%.pcf.gz: %.bdf
	bdftopcf $< | gzip -9c > $@.tmp && \
	mv $@.tmp $@

clean:
	rm -f *.BAK *.pcf.gz make_table
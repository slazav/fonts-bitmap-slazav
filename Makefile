SIZES=10 12 14 16 24
FONTS=$(patsubst %, h%.pcf.gz, ${SIZES})
BFONTS=$(patsubst %, h%_b.pcf.gz, ${SIZES})

CXXFLAGS=-std=gnu++11

all: ${FONTS} ${BFONTS} make_table


install: 
	cp $(FONTS) /usr/share/fonts/bitmap/efont-slazav

%.pcf.gz: %.bdf
	bdftopcf $< | gzip -9c > $@.tmp && \
	mv $@.tmp $@

clean:
	rm -f *.BAK *.pcf.gz make_table
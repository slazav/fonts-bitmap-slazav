## fonts-bitmap-slazav


This is a quick project to get a nice set of unicode bitmap console fonts for my xterm.
It's based on Efont project: http://openlab.ring.gr.jp/efont.
All modifications are done manually with `xmbdfed` program.

Main modfications:

- I have only 5x10, 6x12, 7x14, 8x16, 9x17, 11x23 fonts without bold and italic versions.

- 9x17 font is created from bold 8x16 by increasing spacing, 11x23 is based on 12x24.

- Different shape of similar Cyrillic and Latin letters:
  а-a А-A В-B e-е Е-E u-и k-к K-К m-м M-М H-Н о-o О-O n-п p-р P-Р c-с C-С y-у Y-У x-х X-Х

- Different shape of letters and numbers: 0-O-О 3-З

- Add and fix many letters (mostly Cyrillic)

- Fix box drawing symbols

#### Font examples

<img src="https://raw.githubusercontent.com/slazav/fonts-bitmap-slazav/master/table_v3.png">

#### X resources for xterm
```
*vt100.utf8Fonts.font:  -slazav-fixed-medium-r-*-*-16-*-*-*-*-*-iso10646-1
*vt100.utf8Fonts.font2: -slazav-fixed-medium-r-*-*-10-*-*-*-*-*-iso10646-1
*vt100.utf8Fonts.font3: -slazav-fixed-medium-r-*-*-12-*-*-*-*-*-iso10646-1
*vt100.utf8Fonts.font4: -slazav-fixed-medium-r-*-*-14-*-*-*-*-*-iso10646-1
*vt100.utf8Fonts.font5: -slazav-fixed-medium-r-*-*-16-*-*-*-*-*-iso10646-1
*vt100.utf8Fonts.font6: -slazav-fixed-medium-r-*-*-17-*-*-*-*-*-iso10646-1
*vt100.utf8Fonts.font7: -slazav-fixed-medium-r-*-*-23-*-*-*-*-*-iso10646-1
'''

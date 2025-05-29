## fonts-bitmap-slazav

This is my quick project to get a nice set of unicode bitmal console fonts for my xterm.
It's based on Efont project: http://openlab.ring.gr.jp/efont.
All modifications are done manually with `xmbdfed` program.

Main modfications:

- I have only 5x10, 6x12, 7x14, 8x16, 9x17, 12x20 fonts without bold and italic versions.

- 9x17 font is created from bold 8x16 by increasing spacing.

- Different shape of similar Cyrillic and Latin letters:
  а-a А-A В-B e-е Е-E u-и k-к K-К m-м M-М H-Н о-o О-O n-п p-р P-Р c-с C-С y-у Y-У x-х X-Х

- Different shape of letters and numbers: 0-O-О 3-З

- Add missing Tadjik letters, fix errors in 5x10 and 6x12: ғҒ ӣӢ қҚ ȳȲ ҳҲ ҷҶ


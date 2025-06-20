#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <stdexcept>
#include <memory>
#include <codecvt>
#include <locale>

struct font_t: public std::map<int, std::vector<int> > {
  size_t W,H;

  font_t(): W(0), H(0) {}
};

struct img_t{
  std::shared_ptr<unsigned char> data;
  size_t W,H; // width, height

  size_t len() const {return H*((W+7)/8);}

  // constructor
  img_t(size_t W=0, size_t H=0): W(W), H(H) {
    auto L = len();
    try{
      data = std::shared_ptr<unsigned char>(
        new unsigned char[L], std::default_delete<unsigned char[]>());
    }
    catch (const std::bad_alloc & e) {
      throw std::logic_error(std::string("can't allocate memory for image: ") + e.what());
    }
    for (size_t i=0; i<L; i++) data.get()[i] = 0x00;
  }

  // set pixel; ignore overflows
  void set(const size_t x, const size_t y, const bool v){
    size_t b = y*((W+7)/8) + x/8; // byte
    size_t o = 7-x%8; // offset
    if (b >= len()) return;
    uint8_t old = data.get()[b];
    data.get()[b] = v? old|(1<<o) : old&~(1<<o);
  }


  void
  write(const font_t & fn, int & x, int & y, const std::u32string & str, bool vert = false){
    int wd = 8*((fn.W+7)/8);
    int x0(x), y0(x);
    for (const auto c: str){

      if (c == 10){ // newline
        if (vert){y=y0; x+=fn.H;}
        else {x=x0; y+=fn.H;}
        continue;
      }

      auto fch = fn.find(c);
      if (fch != fn.end()){
        for (size_t j = 0; j<fn.H; ++j){
          auto v = fch->second[j];
          for (size_t i = 0; i<fn.W; ++i){
            if (vert) set(x+j, y+i, v & (1<<(wd-i-1)));
            else set(x+i, y+j, v & (1<<(wd-i-1)));
          }
        }
      }
      else { // missing char
        for (size_t j = 0; j<fn.H; ++j){
          for (size_t i = 0; i<fn.W; ++i){
            if (vert) set(x+j, y+i, 1);
            else set(x+i, y+j, (i+j)%2);
          }
        }
      }
      if (vert) y+=fn.W;
      else x+=fn.W;
    }
  }

  void
  write(const font_t & fn, int & x, int & y, const std::string & str, bool vert = false){
    std::wstring_convert<std::codecvt_utf8<char32_t>,char32_t> cv;
    auto s = cv.from_bytes(str);
    write(fn, x, y, s, vert);
  }

  void
  hline(const int x1, const int x2, const int y){
    for (int x = x1; x<=x2; ++x) set(x, y, 1);
  }

  void
  save_pbm(const std::string & fname){
    std::ofstream ss(fname);
    ss << "P4\n"
       << W << " " << H << "\n";
    for (size_t i=0; i<len(); i++) ss << data.get()[i];
  }

};

int
str_to_int(const std::string & s, bool hex = false){
  std::istringstream ss(s);
  int val;
  if (hex) ss >> std::hex;
  else ss >> std::dec;
  ss >> val;
  if (ss.fail() || !ss.eof())
    throw std::logic_error("can't parse value: " + s);
  return val;
}


font_t
read_font(const std::string & fname){
  font_t ret;
  std::ifstream ff(fname);
  if (!ff) throw std::logic_error("can't open file: " + fname);

  int ch = -1; // current character

  size_t ln = 0;
  for (std::string line; std::getline(ff, line);){
    ln++;
    std::istringstream ss(line);
    std::string w;
    ss >> w;
    if (w == "FONTBOUNDINGBOX" && ch==-1){
      ss >> ret.W >> ret.H;
      continue;
    }
    if (w == "STARTCHAR" && ch==-1){
      continue;
    }
    if (w == "ENCODING" && ch==-1){
      ss >> w;
      ch = str_to_int(w, false);
      ret[ch] = {};
      continue;
    }
    if (w == "ENDCHAR" && ch!=-1){
      ch = -1;
      continue;
    }
    if (w == "BBX" && ch!=-1){
      int W,H,X,Y;
      ss >> W >> H >> X >> Y;
      if (W!=ret.W || H!=ret.H)  throw std::logic_error("bad character bbox");
      continue;
    }
    if (w == "BITMAP" && ch!=-1){
      for (size_t i=0; i<ret.H; ++i){
        ff >> w;
        ret[ch].push_back(str_to_int(w, true));
      }
      continue;
    }
  }
  return ret;
}

void print_font_stat(const font_t & fn){
  int xmin = fn.W-1, xmax = 0;
  int ymin = fn.H-1, ymax = 0;
  for (const auto & f: fn){
    int hh=f.second.size()-1;
    int ymin1=0;
    while (ymin1<=hh && f.second[ymin1]==0) ymin1++;
    if (ymin>ymin1) ymin = ymin1;

    int ymax1=hh;
    while (ymax1>=0 && f.second[ymax1]==0) ymax1--;
    if (ymax<ymax1) ymax = ymax1;

    for (const auto d: f.second) {
      int ww = sizeof(d)*8-1;
      int xmin1 = 0;
      while (xmin1<=ww && (((d >> xmin1) & 1)==0)) xmin1++;
      if (xmin>xmin1) xmin = xmin1;

      int xmax1 = ww;
      while (xmax1>=0 && (((d >> xmax1) & 1)==0)) xmax1--;
      if (xmax<xmax1) xmax = xmax1;
    }

  }
  std::cerr << "H: " << fn.W << " x " << fn.H << "\n";
  std::cerr << "x: " << xmin << " .. " << xmax << "\n";
  std::cerr << "y: " << ymin << " .. " << ymax << "\n";
}

int
main(){
  std::vector<std::string> fnts = {"f05x10", "f06x12", "f07x14", "f08x16", "f09x17", "f11x23"};

  /***************************/
  // make test text

  auto test1=
    "ru: абвгдеёжзийклмнопрстуфхцчшщъыьэюя\n"
    "    АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ\n"
    "en: abcdefghijklmnopqrstuvwxyz\n"
    "    ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
    "lat-cyr: aа eе uи kк mм oо nп pр cс yу xх\n"
    "         AА BВ EЕ 3З KК MМ HН O0О PР CС YУ XХ\n"
    "tj: ҒғӢӣҚқӮӯҲҳҶҷ mn: ӨөҮү fi: äöæåø ÄÖÆÅØ\n"
    "!@#$%^&*()_+[]{}`\'\"\\|/<>?,.0123456789\n"
    U"\x2554\x2566\x2550\x2557 \x2552\x2564\x2550\x2555 \x2553\x2565\x2500\x2556 \x250C\x252C\x2500\x2510 \x250F\x2533\x2501\x2513\n"
    U"\x2551\x2551 "   "\x2551 \x2502\x2502 "   "\x2502 \x2551\x2551 "   "\x2551 \x2502\x2502 "   "\x2502 \x2503\x2503 "   "\x2503\n"
    U"\x2560\x256C\x2550\x2563 \x255E\x256A\x2550\x2561 \x255F\x256B\x2500\x2562 \x251C\x253C\x2500\x2524 \x2523\x254B\x2501\x252B\n"
    U"\x255A\x2569\x2550\x255D \x2558\x2567\x2550\x255B \x2559\x2568\x2500\x255C \x2514\x2534\x2500\x2518 \x2517\x253B\x2501\x251B\n"
  ;

  img_t img(520, 1280);
  int x=10, y=10;
  for (const auto fname: fnts){
    auto fn = read_font(fname + ".bdf");
    y+=10;
    img.hline(x, img.W-10, y);
    y+=2;
    img.write(fn, x,y, fname + ":\n");
    img.write(fn, x,y, test1);

    std::cerr << ">>> " << fname << "\n";
    print_font_stat(fn);
  }

  img.save_pbm("out.pbm");

  /***************************/
  // make tables

  for (const auto fname: fnts){
    auto fn = read_font(fname + ".bdf");
    img_t img(fn.W*16, fn.H*16);
    for (size_t y = 0; y<16; ++y){
      for (size_t x = 0; x<16; ++x){
        if (x==0 && y==0) continue;
        if (x==16 && y==16) continue;
        int x0 = x*fn.W, y0=y*fn.H;
        int tab = 4;
        std::u32string s(1, x + 16*y + 256*tab);
        img.write(fn, x0, y0, s);
      }
    }
    img.save_pbm("tab0.pbm");
  }

  return 0;
}



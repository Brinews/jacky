JPEGINC=../../libs/jpeg-6b/
BOOSTINC=../../libs/boost/include/
JPEGLIB=../../libs/jpeg-6b/
BOOSTLIB=../../libs/boost/lib/

all: blur

blur: main.cpp hw6.cpp image.cpp
	g++ -g -o $@ $^ -I$(JPEGINC) -L$(JPEGLIB) -I$(BOOSTINC) -L$(BOOSTLIB) -ljpeg

clean:
	rm blur

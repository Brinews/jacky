#http://www.cnblogs.com/java20130722/archive/2013/03/27/3207082.html
all: blindsign RsaBlindSign

blindsign: blindsign.cpp bigint.cpp
	g++ -o blindsign blindsign.cpp bigint.cpp
RsaBlindSign: RsaBlindSign.cpp
	g++ -o $@ $^

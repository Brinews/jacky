all: blindsign RsaBlindSign

blindsign: blindsign.cpp bigint.cpp
	g++ -o blindsign blindsign.cpp bigint.cpp
RsaBlindSign: RsaBlindSign.cpp
	g++ -o $@ $^

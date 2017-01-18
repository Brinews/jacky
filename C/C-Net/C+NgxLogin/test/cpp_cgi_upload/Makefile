cc=g++

all: up.cgi login.cgi update.cgi

up.cgi: upload_pic.cpp
	g++ $^ -o $@ -lfcgi -lfcgi++ -luuid -ljsoncpp

login.cgi: login.cpp FCgiIO.cpp consrv.cpp
	g++ $^ -o $@ -lfcgi -lfcgi++ -luuid -ljsoncpp -lcgicc

update.cgi: update.cpp FCgiIO.cpp consrv.cpp
	g++ $^ -o $@ -lfcgi -lfcgi++ -luuid -ljsoncpp -lcgicc

clean:
	rm login.cgi up.cgi update.cgi

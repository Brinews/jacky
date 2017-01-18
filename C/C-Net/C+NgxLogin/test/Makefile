cc=g++
LDFLAGS=  -lmysqlclient -ljsoncpp
CFLAGS= 


all: testdb myserver

objs = Test.o sqlhelper.o

%.o:%.cpp
	$(cc) $(CFLAGS) -c $^

testdb: $(objs)
	$(cc) -o $@ $^ $(LDFLAGS)

myserver: server.cpp sqlhelper.o
	g++ -o $@ $^ $(LDFLAGS) $(CFLAGS)

clean:
	rm *.o testdb myserver

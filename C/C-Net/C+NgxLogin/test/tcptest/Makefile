all: TCPEchoServer TCPEchoClient epoclient

TCPEchoClient: TCPEchoClient.cpp
	g++ -g -o $@ $^ -I"../json-cpp/output/include" ../json-cpp/output/lib/libjson_libmt.a

epoclient: epoclient.cpp
	g++ -g -o $@ $^ -I"../json-cpp/output/include" ../json-cpp/output/lib/libjson_libmt.a

TCPEchoServer: TCPEchoServer.cpp
	g++ -g -o $@ $^ -I"../json-cpp/output/include" ../json-cpp/output/lib/libjson_libmt.a

clean:
	rm TCPEchoServer TCPEchoClient epoclient

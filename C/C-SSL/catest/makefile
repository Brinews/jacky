TARGET = ca_client ca_server

all:$(TARGET)

$(TARGET):%:%.c
	gcc -o $@ $^ -L.. -lssl -lcrypto -std=c99 -g -ldl

clean:
	-rm $(TARGET)

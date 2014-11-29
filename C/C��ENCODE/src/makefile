all : main

main: icode.o main.o
	gcc -o $@ $^ -fexec-charset=GBK

%.o : %.c

clean:
	rm *.o main.exe

test:
	#test table build ---> table.h
	./main -b test_utf8.txt test_gbk.txt
	./main -b utf8-chn.txt utf8-gbk.txt
	#test encoding converter ---> gbk_result.txt
	./main -u utf8-chn.txt

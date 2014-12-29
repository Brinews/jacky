all: win_cl_analyzer.exe

clean:
	rm  win_cl_analyzer.exe  main.obj util.obj symbtab.obj analyze.obj

win_cl_analyzer.exe: main.obj  util.obj symbtab.obj analyze.obj  scan.obj parse.obj parse_util.obj parse_print.obj
	cl -o win_cl_analyzer.exe  main.obj  util.obj symbtab.obj analyze.obj  scan.obj parse.obj parse_util.obj parse_print.obj

# There is warning by cl saying /Fe should be used, like /Feanalyzer
# /Fe works in command line, not in make file. maybe a bug. 
# Also, seems like use *.o is not a good choice, 
# For cl *.o works, but fur MinGW gcc, *.o does not work. 

# There is a make error 53, when target is built using eclipse ide, but on command line it is ok. 
# The solution is to start eclipse on command line, AFTER vcvars32.bat is called, so  
# doing this the link error is avoided.

main.obj: ../../main.c
	cl -c ../../main.c

util.obj: ../../util.c
	cl -c ../../util.c

symbtab.obj: ../../symbtab.c
	cl -c ../../symbtab.c
	
analyze.obj: ../../analyze.c
	cl -c ../../analyze.c
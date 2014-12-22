all: win_cl_parser

clean:
	rm win_cl_parser.exe  main.obj util.obj parse.obj parse_util.obj parse_print.obj

win_cl_parser: main.obj scan.obj util.obj parse.obj parse_util.obj parse_print.obj
	cl -o win_cl_parser  main.obj scan.obj util.obj parse.obj parse_util.obj parse_print.obj

main.obj: ../../main.c
	cl -c ../../main.c

util.obj: ../../util.c
	cl -c ../../util.c

parse.obj: ../../parse.c
	cl -c ../../parse.c
	
parse_util.obj: ../../parse_util.c
	cl -c ../../parse_util.c

parse_print.obj: ../../parse_print.c
	cl -c ../../parse_print.c
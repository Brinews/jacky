/* Extended precision hexadecimal integer calculator
 * Implements +, -, and * operations
 *
 * Skeleton code written by Jianzhong Qi, March 2016
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*#include <strings.h>*/
#include <ctype.h>

#define INT_SIZE	101	/* max number of digits (and sign) per integer value */
#define LINE_LEN	106	/* maximum length of any input line */
#define NUM_VARS	10	/* number of different variables */
#define BASE_HEX 	16	/* base of hexadecimal system */

#define CH_ZERO		'0'	/* character 0 */

#define POS_SGN		'+'	/* sign for positive number */
#define NEG_SGN		'-'	/* sign for negative number */

#define ASN_OP		'='	/* addition operator */
#define ECH_OP		'?'	/* addition operator */
#define ADD_OP		'+'	/* addition operator */
#define SUB_OP		'-'	/* addition operator */
#define MUL_OP		'*'	/* addition operator */

#define EXIT_CMD 	"exit"	/* command to exit */
#define PROMPT		"> "	/* command prompt */
#define HEXA		"0x"	/* hexadecimal prefix */
#define CMT_FLAG	'%'		/* indicator for comment line */

/* add your own defined constants here */

typedef char longhex_t[INT_SIZE];	/* one large hexadecimal "variable" */

#define A 10
#define B 11
#define C 12
#define D 13
#define E 14
#define F 15 
/****************************************************************/

/* function prototypes */
void read_line(char *line, int max_len);
void init(longhex_t vars[], int lens[]); 
void echo(longhex_t var, int len);
void assign(longhex_t vars[], int lens[], int index, char *parameter);
void add(longhex_t vars[], int lens[], int index, char *parameter);
void subtract(longhex_t vars[], int lens[], int index, char *parameter);
void multiply(longhex_t vars[], int lens[], int index, char *parameter);

/* add your own function prototypes here */
int compare_without_sig(longhex_t var_a, int len_a, 
		longhex_t var_b, int len_b);

/* vars[index] > var_b */
void do_subtract(longhex_t vars[], int lens[], int index,
		longhex_t var_b, int len_b);

void do_add(longhex_t vars[], int lens[], int index,
		longhex_t var_b, int len_b);

/* If parameter is 0x..., assgin it to temp_var;
 * Else parameter is nX.., temp_var[idx] = vars[X];
 */
void do_assign(longhex_t vars[], int lens[],  
	char *parameter, longhex_t temp_var[], int temp_len[], int idx);

int my_value(char p);
int my_max(int a, int b);
char my_char(int v);
void zero_justify(longhex_t vars[], int lens[], int index);
void digit_shift(longhex_t vars[], int lens[], int index);
/****************************************************************/

/* main program controls all the action */
int
main(int argc, char *argv[]) {
	/* DO NOT CHANGE THE MAIN FUNCTION */
	
	char line[LINE_LEN+1];		/* to hold the input line */
	longhex_t vars[NUM_VARS];	/* to hold 10 large hexadecimal integers */
	int lens[NUM_VARS];			/* to hold the length of the 10 vars */
	
	int index;					/* index of the first variable in command */
	char op;					/* operator in command */
	
	init(vars, lens);
	
	while (1) {
		printf(PROMPT);						/* print prompt */
		read_line(line, LINE_LEN);			/* read one line of command */

		if (line[0] == CMT_FLAG) {			/* print comment in the test data */ 
			printf("%s\n", line);			/* used to simplify marking */
			continue;
		}

		if (strcmp(line, EXIT_CMD) == 0) {	/* see if command is "exit" */
			return 0;
		} 
		
		index = line[1] - CH_ZERO;			/* first var number at line[1] */
		op = line[2];						/* operator at line[2] */

		if (op == ECH_OP) {					/* print out the variable */
			echo(vars[index], lens[index]);
			continue;
		} 
		
		/* do the calculation, second operand starts at line[3] */
		if (op == ASN_OP) {	
			assign(vars, lens, index, line+3);
		} else if (op == ADD_OP) {
			add(vars, lens, index, line+3);
		} else if (op == MUL_OP) {
			multiply(vars, lens, index, line+3);
		} else if (op == SUB_OP) {
			subtract(vars, lens, index, line+3);
		}
	}
	
	/* all done; take some rest */
	return 0;
}

/* read a line of input into the array passed as argument;
 * no need to change this function.
 */
void
read_line(char *line, int max_len) {
	int i = 0, c;
	while (((c = getchar()) != EOF) && (c != '\n')) {
		if (i < max_len) {
			line[i++] = c;
		} else {
			printf("Invalid input line, toooooooo long.\n");
			exit(0);
		}
	}
	line[i] = '\0';
}

/* print out a large hexadecimal integer;
 * no need to change this function, 
 * unless you changed the hexadecimal representation.
 */
void 
echo(longhex_t var, int len) {
	int i;
	
	/* print sign */
	if (var[0] == NEG_SGN) {
		printf("%c", NEG_SGN);
	}
	
	/* print "0x" prefix */
	printf(HEXA);
	
	/* print the digits in a reverse order */
	for (i = len; i > 0; i--) {
		putchar(var[i]);
	}
	
	printf("\n");
}

/****************************************************************/

/* set the vars array to all zero values */
void
init(longhex_t vars[], int lens[]) {
	/*initialize each longhex_t variables in arrary vars to be 0*/
	int i;
	for (i = 0; i < NUM_VARS; i++){
		vars[i][0] = POS_SGN;
		vars[i][1] = '0';
		lens[i] = 1;	
	}
}

/* process the '=' operator */
void
assign(longhex_t vars[], int lens[], int index, char *parameter) {
	/* if the first degit of parameter is '0', it will be a number, 
	 so it means to assigning a constant value to a variable, otherwise
	it's assigning the value of a variable to another variable*/
	int i, j;
	int len;

	vars[index][0] = POS_SGN;

	if (parameter[0] == '-') {
		vars[index][0] = NEG_SGN;
		parameter = parameter + 1;
	}

	if (parameter[0] == '0'){
		len = strlen(parameter+2); /* omit 0x */
		for (i = 0; i < len; i++){
			vars[index][i+1]= parameter[2+(len-i-1)];
		} 
		lens[index] = len;
	} else {
		i = parameter[1] - '0';
		for (j = 1; j <= lens[i]; j++)
			vars[index][j] = vars[i][j];
		lens[index] = lens[i];

		/* different sign or not */
		if (vars[index][0] == vars[i][0]) vars[index][0] = POS_SGN;
		else vars[index][0] = NEG_SGN;
	}
}

		
/* process the '+' operator */
void 
add(longhex_t vars[], int lens[], int index, char *parameter) {
	/* if the first degit of parameter is '0', it will be a number, 
	 so it means to add a constant value to a variable, otherwise
	it's adding the value of a variable to another variable*/

	longhex_t temp_var[1];
	int temp_len[1];

	/* 1. assign the adder to temp_var[0] */
	do_assign(vars, lens, parameter, temp_var, temp_len, 0);

	/* 2. do add */
	do_add(vars, lens, index, temp_var[0], temp_len[0]);
}

/* process the '*' operator */
void 
multiply(longhex_t vars[], int lens[], int index, char *parameter) {

	int i, j;
	longhex_t temp_var[2];
	int temp_len[2];
	char sign = POS_SGN;

	/* 1. assign the multiplyer to temp_var[0] */
	do_assign(vars, lens, parameter, temp_var, temp_len, 0);

	/* 2. assign the vars[index] to temp_var[1] */
	temp_len[1] = lens[index];
	for (i = 0; i <= lens[index]; i++)
		temp_var[1][i] = vars[index][i];

	if (temp_var[0][0] != temp_var[1][0]) {
		sign = NEG_SGN;
	}

	/* keep all to POS_SGN */
	temp_var[0][0] = temp_var[1][0] = vars[index][0] = POS_SGN;

	vars[index][1] = '0';
	lens[index] = 1;

	/* 3. multiply change to add */
	for (i = 1; i <= temp_len[0]; i++) {
		for (j = 1; j <= my_value(temp_var[0][i]); j++) {
			do_add(vars, lens, index, temp_var[1], temp_len[1]);
		}
		digit_shift(temp_var, temp_len, 1);
	}

	zero_justify(vars, lens, index);

	vars[index][0] = sign; /* set the sign */
}

/* process the '-' operator */
void 
subtract(longhex_t vars[], int lens[], int index, char *parameter) {
	/* change to <add> */
	longhex_t temp_var[1];
	int temp_len[1];

	/* 1. assign the subtractor to temp_var[0] */
	do_assign(vars, lens, parameter, temp_var, temp_len, 0);

	if (temp_var[0][0] == POS_SGN) {
		temp_var[0][0] = NEG_SGN;
	}
	else {
		temp_var[0][0] = POS_SGN;
	}

	do_add(vars, lens, index, temp_var[0], temp_len[0]);
}

/****************************************************************/

/* add supporting functions here */

/* compare two hex, no considering the signed 
 * return: 1-bigger, 0-equal, -1-smaller
 */
int 
compare_without_sig(longhex_t var_a, int len_a, 
		longhex_t var_b, int len_b) {
	int i;

	if (len_a > len_b) return 1;
	if (len_a < len_b) return -1;

	for (i = len_a; i > 0; i--) {
		if (var_a[i] > var_b[i]) return 1;
		if (var_b[i] > var_a[i]) return -1;
	}

	return 0;
}

void
do_add(longhex_t vars[], int lens[], int index,
		longhex_t var_b, int len_b) {

	int i;
	int cmp;
	int carry = 0;

	longhex_t temp_var[2];
	int temp_len[2];

	/* 1. do assign: temp_var[1] + temp_var[0] */
	temp_len[0] = len_b;
	for (i = 0; i <= len_b; i++)
		temp_var[0][i] = var_b[i];

	temp_len[1] = lens[index];
	for (i = 0; i <= lens[index]; i++)
		temp_var[1][i] = vars[index][i];

	/* 2. vars[index] = temp_var[0] + temp_var[1] */
	if (vars[index][0] == temp_var[0][0]) {
		/* add directly */
		lens[index] = my_max(temp_len[0], temp_len[1]) + 1;
		for (i = 1; i <= lens[index]; i++) 
			vars[index][i] = 0;

		/* overflow */
		if (lens[index] > INT_SIZE-1) lens[index] = INT_SIZE - 1;

		for (i = 1; i <= lens[index]; i++) {
			/* be sure the beyond value is 0 */
			if (i > temp_len[0]) temp_var[0][i] = '0';
			if (i > temp_len[1]) temp_var[1][i] = '0';

			vars[index][i] = my_char((carry + my_value(temp_var[0][i]) 
					+ my_value(temp_var[1][i]))%16);
			carry = (carry + my_value(temp_var[0][i]) 
					+ my_value(temp_var[1][i])) / 16;
		}

		zero_justify(vars, lens, index); /* remove higher zero */
		
	} else {
		/* do subtract */
		cmp = compare_without_sig(vars[index], lens[index],
					temp_var[0], temp_len[0]);
		if (cmp > 0) {
			do_subtract(vars, lens, index,
					temp_var[0], temp_len[0]);
		} else if (cmp < 0) {
			do_subtract(temp_var, temp_len, 0,
					vars[index], lens[index]);

			lens[index] = temp_len[0];
			for (i = 0; i <= lens[index]; i++)
				vars[index][i] = temp_var[0][i];

		} else {
			/* eval vars[index] = 0 */
			vars[index][0] = POS_SGN;
			vars[index][1] = '0';
			lens[index] = 1;	
		}
	}
}

void
do_subtract(longhex_t vars[], int lens[], int index,
		longhex_t var_b, int len_b) {
	/* vars[index] > var_b, no considering sign */

	int i;
	int borrow = 0;
	int v;

	/* subtract one by one */
	for (i = 1; i <= lens[index]; i++) {
		v = my_value(vars[index][i]) - borrow - my_value(var_b[i]);

		if (my_value(vars[index][i]) > 0) borrow = 0;
		if (v < 0) {
			v = v + 16;
			borrow = 1;
		}

		vars[index][i] = my_char(v%16);
	}

	zero_justify(vars, lens, index);
}

int my_max(int a, int b) {
	return a > b ? a : b;
}

/* get hex value */
int my_value(char p) {
	int i;
	char *hash = "0123456789ABCDEF";

	for (i = 0; i < strlen(hash); i++) {
		if (p == hash[i]) return i;
	}

	return 0;
}

/* get hex char */
char my_char(int v) {
	char *hash = "0123456789ABCDEF";
	return hash[v];
}

/* remove ahead zeros */
void zero_justify(longhex_t vars[], int lens[], int index) {
	int len = lens[index];
	while (len > 0 && vars[index][len] == '0') {
		len--;
	}
	lens[index] = len;

	if (len == 0) {
		lens[index] = 1;
		vars[index][1] = '0';
		vars[index][0] = POS_SGN;
	}
}

/* shift data << */
void digit_shift(longhex_t vars[], int lens[], int index) {
	int i;

	if (lens[index] == 1 && vars[index][1] == '0') return;

	/* if overflow */
	if (lens[index] >= INT_SIZE - 1) lens[index] = INT_SIZE - 2;

	/* shift left */
	for (i = lens[index]; i >= 1; i--) {
		vars[index][i+1] = vars[index][i];
	}

	vars[index][1] = '0';
	lens[index]++;
}

/* If parameter is 0x..., assgin it to temp_var;
 * Else parameter is nX.., temp_var[idx] = vars[X];
 */
void do_assign(longhex_t vars[], int lens[],  
	char *parameter, longhex_t temp_var[], int temp_len[], int idx)
{
	/* if the first degit of parameter is '0', it will be a number, 
	 so it means to assigning a constant value to a variable, otherwise
	it's assigning the value of a variable to another variable*/
	int i, j;
	int len;

	temp_var[idx][0] = POS_SGN;

	if (parameter[0] == '-') {
		temp_var[idx][0] = NEG_SGN;
		parameter = parameter + 1;
	}

	if (parameter[0] == '0'){
		len = strlen(parameter+2); /* omit 0x */
		for (i = 0; i < len; i++){
			temp_var[idx][i+1]= parameter[2+(len-i-1)];
		} 
		temp_len[idx] = len;
	} else {
		i = parameter[1] - '0';
		for (j = 0; j <= lens[i]; j++)
			temp_var[idx][j] = vars[i][j];
		temp_len[idx] = lens[i];
	}
}


#include "bigint.h"
#include "rational.h"
#include "bigfloat.h"
#include "rpn.h"

#include <string.h>
#include <iostream>

typedef enum {
	TOKENS,
	INFIX2RPN,
	POSTFIX,
	INFIX,
	F_POSTFIX,
	F_INFIX,
	ILLEGAL
} eMode;

eMode getmodetype(int args, char **arg)
{
	eMode ret = INFIX;
	int f = 0;

	for (int i = 1; i < args; i++)
	{
		if (strcmp(arg[i], "--tokens") == 0)
			ret = TOKENS;

		if (strcmp(arg[i], "--infix2rpn") == 0)
			ret = INFIX2RPN;

		if (strcmp(arg[i], "--postfix") == 0)
			ret = POSTFIX;

		if (strcmp(arg[i], "--infix") == 0)
			ret = INFIX;

		if (strcmp(arg[i], "--approximate") == 0)
			f = 1;
	}

	if (ret == INFIX && f == 1) ret = F_INFIX;
	if (ret == POSTFIX && f == 1) ret = F_POSTFIX;

	return ret;
}

int main(int argc, char **argv)
{
	//create all the appropriate class objects
	tokenifier tok;
	infix2rpn infix(&tok);
	//rpneval frominfix<Rational>(&infix); // extra code need to choose template
	//rpneval fromrpn<Rational>(&tok);
	rpneval<Rational> frominfix(&infix); // extra code need to choose template
	rpneval<Rational> fromrpn(&tok);

	rpneval<BigFloat> f_frominfix(&infix); // extra code need to choose template
	rpneval<BigFloat> f_fromrpn(&tok);

	//set this pointer to the appropriate one
	tokenstream *processor;

	int readin = 0;

#define MAIN
#ifndef MAIN
	BigFloat a("100");
	BigFloat b("125000000000000000000");
	BigFloat c("123812391023980129990000");

	std::cout << a + b << std::endl;
	std::cout << a - b << std::endl;
	std::cout << a * b << std::endl;
	std::cout << a / b << std::endl;
	std::cout << sqrt(c) << std::endl;
#endif

#ifdef MAIN
	if (argc < 2) 
	{
		std::cout << "Arguments Ill-Format.\n";
		return -1;
	}
#else
	//argv[1] = "--tokens";
	//argv[1] = "--infix2rpn";
	argv[1] = "--infix";
	//argv[1] = "--postfix";
	//argv[2] = "9 19 12 9 * + -";
	argv[2] = "  3/ 7*4 + pi";
	//argv[2] = "  19 - sqrt( 9 ) + 111";
	argv[3] = "9000000000099999999 -   (19   + 12)*9+sqrt(16  + 9+ 12*2) - 12  ";
	//argv[3] = "19 10 - 1 - ";
	//argv[3] = "99-28*2+34/2+sqrt(8+9)-19";
	//argv[4] = "99-28*(2+34)/2";
	argc = 4;
#endif

	int i;
	eMode mode = getmodetype(argc, argv);

	if (mode == TOKENS)
		processor = &tok;
	else if (mode == INFIX2RPN)
		processor = &infix;
	else if (mode == POSTFIX)
		processor = &fromrpn;
	else if (mode == INFIX)
		processor = &frominfix;
	else if (mode == F_INFIX)
		processor = &f_frominfix;
	else if (mode == F_POSTFIX)
		processor = &f_fromrpn;

	//Now just call the virutal method through the pointer
	//and print the results, polymorphism takes care of the rest
	for ( i = 1; i < argc; ++i)
	{
		if (strlen(argv[i]) > 2 
			&& argv[i][0] == '-' && argv[i][1] == '-')
			continue;

		std::vector<std::string> output;
		if (!processor->process(argv[i], output))
			std::cout << "error"<<std::endl;
		else
		{
			//print the result with spaces
			for (unsigned int j = 0; j < output.size()-1; ++j)
				std::cout << output[j] << ' ';
			if (output.size())
				std::cout << output[output.size()-1];

			std::cout << std::endl;
		}

		readin = 1;
	}

	std::string str;
	//if (argc == 2 && argv[1][0] == '-')
	if (readin == 0)
	{
		while (getline(std::cin, str))
		{
			std::vector<std::string> output;
			if (!processor->process(str, output))
				std::cout << "error"<<std::endl;
			else
			{
				//print the result with spaces
				for (unsigned int j = 0; j < output.size()-1; ++j)
					std::cout << output[j] << ' ';
				if (output.size())
					std::cout << output[output.size()-1];

				std::cout << std::endl;
			}
		}
	}

	//system("pause");

	return 0;
}

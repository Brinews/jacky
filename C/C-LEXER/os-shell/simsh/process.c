#include	<stdio.h>
#include	"simsh.h"

/* process.c   - supports builtins
 * command processing
 * 
 * The process(char **arglist) function is called by the main loop
 * It sits in front of the execute() function.  This layer handles
 * such processing:
 * 	built-in functions (e.g. exit, cd, history )
 */

int builtin_command(char **, int *);

int process(char **args)
/*
 * purpose: process user command
 * returns: result of processing command
 * details: if a built-in then call appropriate function, if not execute()
 *  errors: arise from subroutines, handled there
 */
{
	int		rv = 0;

	if ( args[0] == NULL )
		return rv;

	if ( !builtin_command(args, &rv) )
		rv = execute(args);

	return rv;
}

/*
 * exprpars.c - Pcal routines concerned with parsing if{n}def expressions
 *
 * Contents:
 *
 *		do_xxx
 *		lookup_token
 *		next_token
 *		parse_expr
 *
 * Revision history:
 *
 *	4.8.0	B.Marr	2004-11-15	Remove Ctl-L (page eject) characters 
 *					from source file.  Rename 'find_sym()'
 *					routine to 'find_sym_name()' for
 *					unambiguous searches now that we have
 *					new 'find_sym_val()' routine.
 * 
 *	4.7	AWR	02/24/1998	add prototypes (using PROTO macro)
 *					to function pointer declarations
 *
 *			12/21/1997	clean up gcc warnings in -Wall mode
 *
 *			07/27/1997	delete obsolete FPR and PRT macros
 *
 *	4.6	AWR	12/05/1995	delete vestiges of never-pursued idea
 *					from next_token(): permit "if -<flag>"
 *					syntax, which would have determined
 *					whether or not "-<flag>" had been set
 *
 *	4.5	AWR	04/28/1993	restructure function definitions so
 *					function name appears in first column
 *					(to facilitate searching for definition
 *					by name)
 *
 *	4.0	AWR	02/06/1991	Author
 *
 */

/*
 * Standard headers:
 */

#include <ctype.h>
#include <string.h>
#include <stdio.h>

/*
 * Pcal-specific definitions:
 */

#include "pcaldefs.h"
#include "pcalglob.h"

/*
 * Macros:
 */

/*
 * token type code definitions:
 */

#define TK_UNKNOWN	 0		/* codes returned by next_token() */
#define TK_IDENT	 1
#define TK_LPAREN	 2
#define TK_RPAREN	 3
#define TK_UNARYOP	 4
#define TK_BINARYOP	 5
#define TK_ENDINPUT	 6
#define TK_STARTINPUT	 7		/* special code for start symbol */

/* bit position for token type codes (cf. where_ok[] below) */
#define ID_OK		(1 << TK_IDENT)
#define LP_OK		(1 << TK_LPAREN)
#define RP_OK		(1 << TK_RPAREN)
#define UO_OK		(1 << TK_UNARYOP)
#define BO_OK		(1 << TK_BINARYOP)
#define ST_OK		(1 << TK_STARTINPUT)
#define NEVER_OK	0

/* is token "curr" legal after "prev"? (cf. where_ok[] below) */
#define IS_LEGAL(curr, prev)	(where_ok[curr] & (1 << (prev)))

#define NT_RETURN(p, t, v) \
	do { *ptype = t; *pvalue = v; return p; } while (0)

/*
 * operator-related definitions:
 */

#define OP_AND		0	/* operator subcodes */
#define OP_OR		1
#define OP_XOR		2
#define OP_NEGATE	3

#define ENDINPUT_PREC	-1	/* arbitrary number < lowest op. prec  */
#define OR_PREC		 1	/* operator precedence levels */
#define XOR_PREC	 2
#define AND_PREC	 3
#define NEGATE_PREC	 4
#define PAREN_PREC	 8	/* arbitrary number > highest op. prec */

/* lower bits of operator stack entry are code; higher are precedence */
#define OPR_BITS	4
#define OPR_MASK	((1 << OPR_BITS) - 1)
#define PREC(op)	((op) >> OPR_BITS)
#define OPCODE(op)	((op) & OPR_MASK)
#define MAKE_OPR(p, o)	(((p) << OPR_BITS) | (o))

#define MAX_OP		20	/* size of operand and operator stacks */

/*
 * Globals:
 */

typedef short OPERAND;		/* types for operand and operator stacks */
typedef short OPERATOR;

typedef struct {
	char	*name;		/* token spelling */
	short	type;		/* token type code */
	short	value;		/* associated value */
	} TOKEN;

/* token table - note that substrings must follow longer strings */

TOKEN token_tbl[] = {
	{ "&&",	TK_BINARYOP,	OP_AND },	/* synonym for "&" */
	{ "&",	TK_BINARYOP,	OP_AND },
	{ "||",	TK_BINARYOP,	OP_OR },	/* synonym for "|" */
	{ "|",	TK_BINARYOP,	OP_OR },
	{ "!",	TK_UNARYOP,	OP_NEGATE },
	{ "^",	TK_BINARYOP,	OP_XOR },
	{ "(",	TK_LPAREN,	0 },
	{ ")",	TK_RPAREN,	0 },
	{ NULL,	TK_UNKNOWN,	0 }		/* must be last entry */
};

typedef struct {
	short prec;		/* precedence */
	short type;		/* token type (TK_UNARYOP or TK_BINARYOP) */
	OPERAND (*pfcn) PROTO((OPERAND *));	/* dispatch function */
	} OPR;

/* operator table - entries must be in same order as OP_XXX */

#ifdef PROTOS
static OPERAND do_and(OPERAND *);
static OPERAND do_or(OPERAND *);
static OPERAND do_xor(OPERAND *);
static OPERAND do_negate(OPERAND *);
#else
static OPERAND do_and(), do_or(), do_xor(), do_negate();   /* dispatch fcns */
#endif

OPR opr_tbl[] = {
	{ AND_PREC,	TK_BINARYOP,	do_and },	/* OP_AND	*/
	{ OR_PREC,	TK_BINARYOP,	do_or },	/* OP_OR	*/
	{ XOR_PREC,	TK_BINARYOP,	do_xor },	/* OP_XOR	*/
	{ NEGATE_PREC,	TK_UNARYOP,	do_negate }	/* OP_NEGATE	*/
};


/* set of tokens which each token may legally follow (in TK_XXX order) */

int where_ok[] = {
	NEVER_OK                                      ,	 /* TK_UNKNOWN	*/
	ST_OK         | LP_OK         | UO_OK | BO_OK ,	 /* TK_IDENT	*/
	ST_OK         | LP_OK         | UO_OK | BO_OK ,	 /* TK_LPAREN	*/
	        ID_OK | LP_OK | RP_OK                 ,	 /* TK_RPAREN	*/
	ST_OK         | LP_OK                 | BO_OK ,	 /* TK_UNARYOP	*/
	        ID_OK         | RP_OK                 ,	 /* TK_BINARYOP	*/
	ST_OK | ID_OK         | RP_OK                    /* TK_ENDINPUT	*/
};


/*
 * do_xxx - dispatch functions for operators
 */

static OPERAND
#ifdef PROTOS
do_and(OPERAND *ptop)
#else
do_and(ptop)
	OPERAND *ptop;
#endif
{
	return ptop[0] && ptop[-1];
}


static OPERAND
#ifdef PROTOS
do_or(OPERAND *ptop)
#else
do_or(ptop)
	OPERAND *ptop;
#endif
{
	return ptop[0] || ptop[-1];
}


static OPERAND
#ifdef PROTOS
do_xor(OPERAND *ptop)
#else
do_xor(ptop)
	OPERAND *ptop;
#endif
{
	return (ptop[0] ^ ptop[-1]) != 0;
}


static OPERAND
#ifdef PROTOS
do_negate(OPERAND *ptop)
#else
do_negate(ptop)
	OPERAND *ptop;
#endif
{
	return ! ptop[0];
}


/*
 * lookup_token - look up token in table; return pointer to table entry
 */
static TOKEN *
#ifdef PROTOS
lookup_token(char *p)
#else
lookup_token(p)
	char *p;
#endif
{
	TOKEN *ptok;

	for (ptok = token_tbl;
	     ptok->name && strncmp(p, ptok->name, strlen(ptok->name));
	     ptok++)
		;

	return ptok;
}


/*
 * next_token - fetch next token from input string; fill in its type and value
 * and return pointer to following character
 */
static char *
#ifdef PROTOS
next_token(char *p,
	   int *ptype,
	   int *pvalue)
#else
next_token(p, ptype, pvalue)
	char *p;
	int *ptype;
	int *pvalue;
#endif
{
	TOKEN *ptok;
	char tokbuf[STRSIZ], *pb;

	while (*p && isspace(*p))	/* skip whitespace */
		p++;

	if (*p == '\0')			/* end of input? */
		NT_RETURN(p, TK_ENDINPUT, 0);

	if (isalpha(*p)) {		/* identifier */

		pb = tokbuf;		/* make local copy and look up */
		while (*p && (isalpha(*p) || isdigit(*p) || *p == '_'))
			*pb++ = *p++;
		*pb = '\0';

		NT_RETURN(p, TK_IDENT, find_sym_name(tokbuf));
	}

	ptok = lookup_token(p);		/* other token */
	NT_RETURN(p + (ptok->name ? strlen(ptok->name) : 1), ptok->type,
		ptok->value);
}


/*
 * parse_expr - parses expression consisting of identifiers and logical
 * operators; return TRUE if expression is true (identifier defined => true);
 * FALSE if false; EXPR_ERR if syntax error in expression
 */
int
#ifdef PROTOS
parse_expr(char *pbuf)
#else
parse_expr(pbuf)
	char *pbuf;
#endif
{
	OPERAND opd_stack[MAX_OP];	/* operand stack - TRUE/FALSE values */
	OPERATOR opr_stack[MAX_OP];	/* operator stack - precedence | op */
	int value, token, plevel, prec, result, npop, opr, opd, prev_token, op;

	plevel = 0;			/* paren nesting level */
	opd = opr = -1;			/* indices of stack tops */
	prev_token = TK_STARTINPUT;	/* to detect null expressions */

	if (DEBUG(DEBUG_PP))
		fprintf(stderr, "evaluating expression '%s'\n", pbuf);
	do {
		pbuf = next_token(pbuf, &token, &value);

		/* check that the current token may follow the previous one */
		if (! IS_LEGAL(token, prev_token))
			return EXPR_ERR;

		switch(token) {

		case TK_IDENT:		/* identifier => 1 if def, 0 if not */
			opd_stack[++opd] = value != PP_SYM_UNDEF;
			break;

		case TK_LPAREN:		/* left paren - bump nesting level */
			++plevel;
			break;

		case TK_RPAREN:		/* right paren - decrement nesting */
			if (--plevel < 0)
				return EXPR_ERR;
			break;

		case TK_ENDINPUT:	/* end-of-input - treat as operator */
			if (prev_token == TK_STARTINPUT)
				return FALSE;	/* null expr => FALSE */
			/* fall through */

		case TK_UNARYOP:
		case TK_BINARYOP:

			/* get precedence of operator, adjusting for paren
			 * nesting (TK_ENDINPUT has the lowest precedence
			 * of all, to unwind operand/operator stacks at end)
			 */

			prec = token == TK_ENDINPUT ? ENDINPUT_PREC :
				(plevel * PAREN_PREC) + opr_tbl[value].prec;

			/* pop (and perform) any equal- or higher-precedence
			 * operators on operator stack: extract operator,
			 * check for operand stack underflow, execute
			 * operator, adjust operand stack height and place
			 * result of operator on top
			 */

			for ( ;
			     opr >= 0 && PREC(opr_stack[opr]) >= prec;
			     opr--) {
				op = OPCODE(opr_stack[opr]);
				npop = opr_tbl[op].type == TK_UNARYOP ? 0 : 1;
				if (opd < npop)
					return EXPR_ERR;
				result = (*opr_tbl[op].pfcn)(opd_stack + opd);
				opd_stack[opd -= npop] = result;
			}

			/* push operator (if any) onto stack */

			if (token != TK_ENDINPUT)
				opr_stack[++opr] = MAKE_OPR(prec, value);

			break;

		default:		/* should never get here */
			return EXPR_ERR;
			break;
		}

		prev_token = token;

	} while (token != TK_ENDINPUT);

	/* done - check for dangling parens, and leftover operand/operators */

	if (DEBUG(DEBUG_PP))
		fprintf(stderr, "evaluated to %s\n",
			opd_stack[0] ? "TRUE" : "FALSE");
	return plevel != 0 || opd != 0 || opr != -1 ?
		EXPR_ERR :		/* leftover junk - return error */
		opd_stack[0];		/* all OK - return final value */
}

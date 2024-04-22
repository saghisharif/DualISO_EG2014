/* arg.h: definitions for argument parsing package */

#ifndef ARG_HDR
#define ARG_HDR

/* $Header: /usr/cvsroot/src/pub/libarg/arg.h,v 1.6 2007/05/15 17:21:33 mike Exp $ */

#include <stdio.h>
#include <stdarg.h>

#include "expr.h"

typedef struct arg_form {       /* ARGUMENT FORM */

    /* a "form" contains the format, doc string, and miscellaneous internal */
    /* info about an argument.  It's an argument descriptor, basically */

    struct arg_form *next;      /* next in linked list */
    char *format;               /* scanf-style format:    "-size %d %F" */
    char *flag;                 /* flag portion of format:"-size" */
    char *code;                 /* just the format codes: "dF" */
    char *doc;                  /* documentation string:  "set widget size" */
    short type;                 /* REGULAR | SIMPFLAG | PARAMFLAG |
                                        SUBRFLAG | SUBLISTFLAG | NOP */
    short nparam;               /* number of parameters to flag */
    int parammask;              /* bit i says ok to stop before param i, i=0..*/
    int **param;                /* parameter pointer list */
    int (*subr)();              /* subroutine to call for action (if any) */
    struct arg_form *sublist;   /* subordinate list (if any) */
    short rep;                  /* # times this flag repeated in arglist */
} Arg_form;

/* form type values */
#define ARG_REGULAR     1       /* a regular argument */
#define ARG_SIMPFLAG    2       /* a simple flag (no parameters) */
#define ARG_PARAMFLAG   3       /* a flag with parameters */
#define ARG_SUBRFLAG    4       /* a flag with subroutine action */
#define ARG_SUBLISTFLAG 5       /* a sub-formlist */
#define ARG_NOP         6       /* no arg or flag, just a doc string */

/* the following must be impossible pointer values (note: machine-dependent) */
#ifdef __x86_64__
    #define ARG_MASKNEXT    0x8000000000000000      /* mask for these NEXT flags */
    #define ARG_FLAGNEXT    0x8000000000000001
    #define ARG_SUBRNEXT    0x8000000000000002
    #define ARG_LISTNEXT    0x8000000000000003
#else
    #define ARG_MASKNEXT    0x80000000      /* mask for these NEXT flags */
    #define ARG_FLAGNEXT    0x80000001
    #define ARG_SUBRNEXT    0x80000002
    #define ARG_LISTNEXT    0x80000003
#endif

/* varargs tricks */
#define ARG_FLAG(ptr)           ARG_FLAGNEXT, (ptr)     /* for SIMPFLAG */
#define ARG_SUBR(ptr)           ARG_SUBRNEXT, (ptr)     /* for SUBRFLAG */
#define ARG_SUBLIST(ptr)        ARG_LISTNEXT, (ptr)     /* for SUBLISTFLAG */

/* error codes: BADCALL is a programmer error, the others are user errors */
#define ARG_BADCALL     -1      /* arg_parse call itself is bad */
#define ARG_BADARG      -2      /* bad argument given */
#define ARG_MISSING     -3      /* argument or parameter missing */
#define ARG_EXTRA       -4      /* extra argument given */

#define ARG_NARGMAX 10000       /* max number of allowed args */

extern int arg_debug, arg_doccol;
extern int arg_warning;         /* print warnings about repeated flags? */
Arg_form *arg_to_form1( va_list ap );
Arg_form *arg_find_flag( char *arg, Arg_form *form );
Arg_form *arg_find_reg();
void arg_init( Arg_form *form );
void space( FILE *fp, int c, int c1 );
void av_print( char *str, int ac, char **av );

#define arg_to_form(args...) __arg_to_form(NULL, args)

#ifdef __cplusplus
    extern "C" {
        int arg_parse(int ac, char **av, ...);
        int arg_parse_argv(int ac, char **av, Arg_form *form);
        int arg_parse_stream(FILE *fp, Arg_form *form);
        Arg_form *__arg_to_form(void *NOTUSED, ...);
        int arg_form_print(Arg_form *form);
    }
#else
    Arg_form *__arg_to_form(void *NOTUSED, ...);
    void arg_form_print( Arg_form *form );
#endif

#endif

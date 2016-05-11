#ifndef _DEBUG_H
#define _DEBUG_H

typedef enum {
    VERBOSE_SILENT,
    VERBOSE_PASS,
    VERBOSE_SUBPASS,
    VERBOSE_DETAIL
}Verbose_t;


static const int STEP = 2;
static int indent = 0;

void Trace_indent()
{
    indent += STEP;
}

void Trace_unindent()
{
    indent -= STEP;
}

void Trace_spaces()
{
    int i = indent;
    while (i--)
    {
        printf(" ");
    }
}


int Control_Verb_order(Verbose_t v1, Verbose_t v2) {
    return v1<=v2;
}

#define Verbose_TRACE(s, f, x, r, level)        \
    do{                                         \
        int exist = Control_Verb_order(level, VERBOSE_SILENT); \
        if (exist)                              \
        {                                       \
            Trace_spaces();                     \
            printf("%s starting\n", s);         \
            Trace_indent();                     \
        }                                       \
         r = f x;                               \
        if (exist)                              \
        {                                       \
            Trace_unindent();                   \
            Trace_spaces();                     \
            printf("%s finished\n", s);         \
        }                                       \
    }while(0)

#define Assert_ASSERT(e)        \
    do {                        \
        if (e)                  \
        ;                       \
        else{                   \
            fprintf(stderr,     \
                        "\e[35m\e[1massertion failed:\e[0m%s:%d> %s\n",   \
                        __FILE__, __LINE__, #e);                    \
            exit(1);                                                \
        }                       \
    }while(0)


#define ERROR(s)                    \
    do {                            \
        fprintf(stderr,             \
                    "\e[31m\e[1mERROR>\e[0m%s %s:%d\n",       \
                    s,              \
                    __FILE__,       \
                    __LINE__        \
               );                   \
        fflush(stderr);             \
        exit(1);                    \
    }while (0)



#endif

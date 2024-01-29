#ifndef INPUT_HELPER
#define INPUT_HELPER
#include "errorhelper.h"

#define BUFSIZE     10      /* buffer size for getch-ungetch */
#define CHUNKSIZE   20      /* the maximum chunk size for the chunk-reading input */
#define NUM_CMDS    8       /* the amount of possible commands to use */

/* command constants */
#define CMD_READ_NARGS  17
#define CMD_PRINT_NARGS 1
#define CMD_ADD_NARGS   3
#define CMD_SUB_NARGS   3
#define CMD_MULM_NARGS  3
#define CMD_MULS_NARGS  3
#define CMD_TRANS_NARGS 2
#define CMD_STOP_NARGS  0

/* ArgType: the type of argument */
typedef enum {
    argt_scalar, argt_name
} ArgType;

/* ArgValue:  the scalar of an instruction's argument */
typedef union {
    double scalar;
    char *strVal;
} ArgValue;

/* Command: represents the command of an instruction */
typedef enum {
    cmd_read,   /* read_mat */
    cmd_print,  /* print_mat */
    cmd_add,    /* add_mat */
    cmd_sub,    /* sub_mat */
    cmd_mulm,   /* mul_mat */
    cmd_muls,   /* mul_scalar */
    cmd_trans,  /* trans_mat */
    cmd_stop    /* stop */
} Command;

/* Argument: represents an argument for an instruction */
typedef struct {
    ArgType type;       /* the argument's type */
    ArgValue value;     /* the argument's scalar */
    int hasValue;       /* does this argument have a value? */
} Argument;

/* CmdArgument: represents a command's argument */
typedef struct {
    ArgType type;
    int required;   /* is the argument optional or required? */
} CmdArgument;

/* CmdId: represents a command's string 'strVal' and it's Command enum scalar */
typedef struct {
    char *name;
    Command cmd;
    CmdArgument *arguments;
    int numArgs;
} CmdId;

/* Instruction: this structure represents an instruction the program can execute */
typedef struct {
    Command cmd;        /* this instruction's command */
    Argument *args;     /* this instruction's arguments (length depends on the command) */
} Instruction;

const CmdId command_identifiers[NUM_CMDS];

int getNextLine(char **pline);
ErrCode lineToInstruction(char *str, Instruction *pi);
char *getEndOfToken(char *str);
int tryParseNumber(char *str, double *number);
char *skipEmpty(char *str);
int eqlToLen(char *str1, char *str2, int len);
char *dupTok(char *str);
int getTokLen(char *str);
int isDig(int c);
int isInTok(char c);
ErrCode strToCommand(char *str, Command *pcmd);
CmdId getCmdId(Command cmd);
CmdArgument *getArgTypes(Command cmd, int *numArgs);
void freeInstruction(Instruction *pi);
int getch(void);
void ungetch(int c);

#endif

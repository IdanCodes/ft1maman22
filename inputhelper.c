#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>       /* for double pow(double, double) */
#include "inputhelper.h"

/* getNextLine: get the next line from the input (formatted with all tabs and whitespaces to tabs).
 * returns the length of the line. (or 0 if there was insufficient memory) */
int getNextLine(char **pline) {
    int i, c, numChunks;

    numChunks = 1;
    i = 0;

    while ((c = getch()) == ' ' || c == '\t' || c == '\n')
        ;   /* wait until we find the next line of input */
    if (c == EOF)
        return 0;   /* reached end */

    *pline = (char *)malloc(sizeof(char) * CHUNKSIZE); /* allocate a chunk for *pline */

    if (*pline == NULL)
        return 0;   /* couldn't allocate enough memory */

    while (c != '\n' && c != EOF) {
        /* read the next token from input */
        while (i <= numChunks * CHUNKSIZE && (c = getch()) != '\n' && c != ' ' && c != '\t' && c != EOF) {
            (*pline)[i] = (char)c;
            i++;
        }

        /* if the loop stopped because of c's scalar, 'i' must be a valid index */
        if (i >= numChunks * CHUNKSIZE) {
            /* move *pline to a new address (or keep existing one) with another chunk allocated */
            numChunks++;
            *pline = (char *)realloc(*pline, numChunks * sizeof(char) * CHUNKSIZE);
            if (*pline == NULL)
                return 0;   /* couldn't allocate enough memory */
        }
        else if (c == ' ' || c == '\t') {
            /* add a whitespace and skip whitespaces and tabs */
            (*pline)[i++] = ' ';
            while ((c = getch()) == ' ' || c == '\t')
                ;   /* skip whitespaces and tabs */

            if (c == '\n' || c == EOF)
                i--;        /* replace the whitespace with '\0' after the loop stops */
            else
                ungetch(c); /* read one extra character */
        }
    }

    (*pline)[i] = '\0';  /* close off the line */

    return i;
}

/* lineToInstruction: convert a string to an instruction.
 * returns an error if there was one. (else err_none) */
ErrCode lineToInstruction(char *str, Instruction *pi) {
    int i, numArgs;
    ErrCode err;
    Command cmd;
    CmdArgument *argTypes;

    /* -- read command -- */
    err = strToCommand(str, &cmd);
    str = getEndOfToken(str); /* skip to the end of the command */

    if (err != err_none)
        return err;

    str = skipEmpty(str);

    if (*str == ',')
        return err_ill_com;

    /* -- read arguments -- */
    argTypes = getArgTypes(cmd, &numArgs);

    free(pi->args); /* free previous args to reallocate */
    pi->args = (Argument *)malloc(sizeof(Argument) * numArgs);

    if (pi->args == NULL)
        return err_insuf_mem;   /* couldn't allocate enough memory */

    for (i = 0; i < numArgs; i++) {
        if (*str == '\n' || *str == EOF) {
            if (argTypes[i].required)
                return err_arg_exp;     /* reached end of string */
            else {
                /* there are no required arguments after an optional one.
                 * initialize the rest of the arguments as having no value */
                pi->args[i].hasValue = 0;
                continue;
            }
        }

        pi->args[i].type = argTypes[i].type; /* configure the argument's type */

        if (argTypes[i].type == argt_name) {
            if (!isAlphaTok(str))
                return err_mat_exp;

            /* strVal is already freed (since we freed args) */
            pi->args[i].value.strVal = dupTok(str);
            if (pi->args[i].value.strVal == NULL)
                return err_insuf_mem;
        }
        else if (!tryParseNumber(str, &(pi->args[i].value.scalar))) /* parse the scalar into its designated address */
            return err_scl_exp;     /* argument is not a number */

        pi->args[i].hasValue = 1;

        str = getEndOfToken(str);
        str = skipEmpty(str);

        if (i < numArgs - 1) {
            if (*str != ',')
                return err_comm_exp;
            str = skipEmpty(str + 1);
            if (*str == ',')
                return err_mult_com;
        }
        else if (*str != '\n' && *str != EOF)
            return err_extra_text;
    }

    return err_none;
}

/* strToCommand: get the command type of the given string's first token */
ErrCode strToCommand(char *str, Command *cmd) {
    int i;
    ErrCode result;
    int cmdLen;

    cmdLen = getTokLen(str);

    /* result defaults to didn't find the command */
    result = err_undef_cmd;

    for (i = 0; i < NUM_CMDS; i++) {
        if (cmdLen != getTokLen(command_identifiers[i].name)       /* same length check */
            || eqlToLen(str, command_identifiers[i].name, cmdLen))  /* actual string check */
            continue;

        /* found the command */
        *cmd = command_identifiers[i].cmd;
        result = err_none;
        break;
    }

    return result;
}

/* equalFirstToken: returns wheter the first tokens of the given strings are equal */
int equalFirstToken(char *str1, char *str2) {
    char *end1, *end2;

    end1 = getEndOfToken(str1);
    end2 = getEndOfToken(str2);

    /* unequal lengths */
    if (end1 - str1 != end2 - str2)
        return 0;

    return eqlToLen(str1, str2, (int)(end1 - str1));
}

/* getEndOfToken: returns the address of the final character in the first token of the given string */
char *getEndOfToken(char *str) {
    for (; isInTok(*str); str++)
        ; /* wait until we encounter a character out of the token */
    return str;
}

/* tryParseNumber: tries to parse a string's first token to double.
 * returns whether the parsing was successful. (whether the string is a valid double) */
int tryParseNumber(char *str, double *number) {
    int sign;
    char *end;
    double power, result;

    end = getEndOfToken(str);
    sign = 1;
    result = 0;

    if (*str == '-') {
        sign = -1;
        str++;
    }
    else if (*str == '+')
        str++;

    if (str == end || (!isDig(*str) && *str != '.'))
        return 0; /* not a scalar, just a sign/empty token */

    /* read whole part */
    for (; str < end && *str != '.'; str++) {
        if (!isDig(*str))    /* not a digit nor a '.' */
            return 0;

        result *= 10;
        result += *str - '0';   /* scalar-ify */
    }

    if (str != end) {
        /* read decimal part */
        for (power = 1; str < end; str++) {
            if (!isDig(*str))
                return 0;

            result += (*str - '0') * pow(10, power++);
        }
    }

    *number = result * sign;
    return 1;
}

/* skipEmpty: get the address of the first char in the string after a stream of white characters */
char *skipEmpty(char *str) {
    for (; *str == ' ' || *str == '\t'; str++)
        ;
    return str;
}

/* eqlToLen: returns whether the first [len] characters of the given strings are equal.
 * assumes both strings are big enough */
int eqlToLen(char *str1, char *str2, int len) {
    for (; len > 0 && *str1 == *str2; str1++, str2++, len--)
        ;
    return len <= 0;
}

/* dupTok: duplicate the first token of the given string.
 * returns the address of the copy, or NULL is there was insufficient memory. */
char *dupTok(char *str) {
    char *tokEnd, *dup;

    tokEnd = getEndOfToken(str);
    dup = (char *)malloc(sizeof(char *) * (tokEnd - str));

    /* NULL if insufficient memory */
    return dup;
}

/* returns the length of the token */
int getTokLen(char *str) {
    return (int)(getEndOfToken(str) - str);
}

/* isDig: returns whether c is a digit (ASCII) */
int isDig(int c) {
    return '0' <= c && c <= '9';
}

/* isAlphaTok: returns whether the first token consists of only alphabetical characters */
int isAlphaTok(char *str) {
    char *end;
    end = getEndOfToken(str);

    for (; str < end; str++) {
        if (!isalpha(*str))
            return 0;
    }

    return 1;
}

/* isInTok: returns whether c is inside a token */
int isInTok(char c) {
    return c != ' ' && c != '\t' && c != '\n'
        && c != EOF && c != '\0' && c != ',';
}

/* getCmdId: get the CmdId of the given command */
CmdId getCmdId(Command cmd) {
    int i;

    for (i = 0; command_identifiers[i].cmd != cmd; i++)
        ;   /* wait until we find the matching command */

    return command_identifiers[i];
}

/* getArgTypes: get argument types given a command */
CmdArgument *getArgTypes(Command cmd, int *numArgs) {
    CmdId id;

    id = getCmdId(cmd);

    *numArgs = id.numArgs;
    return id.arguments;
}

static char buf[BUFSIZE];      /* buffer for ungetch */
static int bufp = 0;           /* next free position in buf */

/* getch: get a (possibly pushed back) character */
int getch(void) {
    return (bufp > 0) ? buf[--bufp] : getchar();
}

/* ungetch: push character back on input */
void ungetch(int c) {
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");   /* never really happens */
    else
        buf[bufp++] = (char)c;
}

static CmdArgument cmd_read_args[CMD_READ_NARGS] = {
        { argt_name, 1 },
        { argt_scalar, 0 },
        { argt_scalar, 0 },
        { argt_scalar, 0 },
        { argt_scalar, 0 },
        { argt_scalar, 0 },
        { argt_scalar, 0 },
        { argt_scalar, 0 },
        { argt_scalar, 0 },
        { argt_scalar, 0 },
        { argt_scalar, 0 },
        { argt_scalar, 0 },
        { argt_scalar, 0 },
        { argt_scalar, 0 },
        { argt_scalar, 0 },
        { argt_scalar, 0 },
        { argt_scalar, 0 },
};

static CmdArgument cmd_print_args[CMD_PRINT_NARGS] = {
        { argt_name, 1 }
};

static CmdArgument cmd_add_args[CMD_ADD_NARGS] = {
        { argt_name, 1 },
        { argt_name, 1 },
        { argt_name, 1 }
};

static CmdArgument cmd_sub_args[CMD_SUB_NARGS] = {
        { argt_name, 1 },
        { argt_name, 1 },
        { argt_name, 1 }
};

static CmdArgument cmd_mulm_args[CMD_MULM_NARGS] = {
        { argt_name, 1 },
        { argt_name, 1 },
        { argt_name, 1 }
};

static CmdArgument cmd_muls_args[CMD_MULS_NARGS] = {
        { argt_name, 1 },
        { argt_name, 1 },
        { argt_name, 1 }
};

static CmdArgument cmd_trans_args[CMD_TRANS_NARGS] = {
        { argt_name, 1 },
        { argt_name, 1 }
};

/* command_identifiers: this array represents the 'key-scalar' pairs of command names and their Command enum scalar */
const CmdId command_identifiers[NUM_CMDS] = {
        { "read_mat", cmd_read, cmd_read_args, CMD_READ_NARGS },
        { "print_mat", cmd_print, cmd_print_args, CMD_PRINT_NARGS },
        { "add_mat", cmd_add, cmd_add_args, CMD_ADD_NARGS },
        { "sub_mat", cmd_sub, cmd_sub_args, CMD_SUB_NARGS },
        { "mul_mat", cmd_mulm, cmd_mulm_args, CMD_MULM_NARGS },
        { "mul_scalar", cmd_muls, cmd_muls_args, CMD_MULS_NARGS },
        { "trans_mat", cmd_trans, cmd_trans_args, CMD_TRANS_NARGS },
        { "stop", cmd_stop, NULL /* no arguments */, CMD_STOP_NARGS }
};

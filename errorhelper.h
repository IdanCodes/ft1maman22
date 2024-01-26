#ifndef ERROR_HELPER
#define ERROR_HELPER

#define NUM_ERR_CODES   10

/* ErrCode: represents error codes for the program */
typedef enum {
    /* -- input error codes -- */
    err_none,       /* no errors */
    err_undef_mat,  /* undefined matrix strVal */
    err_undef_cmd,  /* undefined command strVal */
    err_arg_exp,    /* argument expected */
    err_mat_exp,    /* matrix strVal expected */
    err_comm_exp,   /* comma expected */
    err_scl_exp,    /* argument is not a scalar/scalar expected */
    err_extra_text, /* extraneous text after end of command */
    err_ill_com,    /* illegal comma */
    err_mult_com,   /* multiple consecutive commas */

    /* other error codes */
    err_insuf_mem   /* insufficient memory */
} ErrCode;

typedef struct {
    ErrCode code;
    char *message;
} ErrMessage;

const ErrMessage error_messages[NUM_ERR_CODES];

void logErr(ErrCode err);

#endif

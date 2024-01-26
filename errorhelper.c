#include <stdio.h>
#include "errorhelper.h"

/* logErr: print an error to the output */
void logErr(ErrCode err) {
    int i;

    if (err == err_none)
        return; /* no error, don't print anything */

    for (i = 0; err != error_messages[i].code; i++)
        ;       /* wait until we find the matching error */

    printf("%s\n", error_messages[i].message);
}

/* error_messages: this dictionary represents the 'error-message' pairs of commands and their error message to print */
const ErrMessage error_messages[NUM_ERR_CODES] = {
        { err_undef_mat, "Undefined Matrix Name" },
        { err_undef_cmd, "Undefined Command Name" },
        { err_arg_exp, "Argument Expected" },
        { err_mat_exp, "Matrix Name Expected" },
        { err_comm_exp, "Comma Expected" },
        { err_scl_exp, "Scalar Expected" },
        { err_extra_text, "Extraneous Text After End of Command" },
        { err_ill_com, "Illegal Comma" },
        { err_mult_com, "Multiple Consecutive Commas" },
        { err_insuf_mem, "Insufficient Memory" }
};

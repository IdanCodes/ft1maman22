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

/* handleError: handle a given error. returns whether the program should stop running
 * returns:
 * err_none - there was no error
 * err_light - there was an error the program can ignore
 * err_hard - there was an error that requires the program to stop */
int handleError(ErrCode err) {
    if (err == err_none)
        return err;

    logErr(err);

    if (err == err_insuf_mem) {
        printf("Stopping Program.\n");
        return err_hard;
    }

    return err_light;
}

/* error_messages: this dictionary represents the 'error-message' pairs of commands and their error message to print */
const ErrMessage error_messages[NUM_ERR_CODES] = {
        {err_undef_mat,  "Undefined Matrix Name" },
        {err_undef_cmd,  "Undefined Command Name" },
        {err_arg_exp,    "Argument Expected" },
        {err_mat_exp,    "Matrix Name Expected" },
        {err_comm_exp,   "Comma Expected" },
        {err_nan,        "Argument is not a real number" },
        {err_extra_text, "Extraneous Text After End of Command" },
        {err_ill_com,    "Illegal Comma" },
        {err_mult_com,   "Multiple Consecutive Commas" },
        {err_insuf_mem,  "Insufficient Memory" }
};

#include <stdio.h>
#include <stdlib.h>
#include "mymat.h"
#include "inputhelper.h"
#include "errorhelper.h"
#include "mathelper.h"

#define NUM_MATS    6       /* number of matrices */

#define GET_VAR_NAME(Variable) (#Variable)

int main(void) {
    int i, legalStop;
    char *line;
    ErrCode err;
    Instruction instr;
    mat MAT_A, MAT_B, MAT_C, MAT_D, MAT_E, MAT_F;

    /* register the matrices */
    MatId mats[NUM_MATS] = {
            {GET_VAR_NAME(MAT_A), NULL},
            {GET_VAR_NAME(MAT_B), NULL},
            {GET_VAR_NAME(MAT_C), NULL},
            {GET_VAR_NAME(MAT_D), NULL},
            {GET_VAR_NAME(MAT_E), NULL},
            {GET_VAR_NAME(MAT_F), NULL}
    };

    /* initialize matrices */
    mats[0].matrix = &MAT_A;
    mats[1].matrix = &MAT_B;
    mats[2].matrix = &MAT_C;
    mats[3].matrix = &MAT_D;
    mats[4].matrix = &MAT_E;
    mats[5].matrix = &MAT_F;

    legalStop = 0;  /* used to check if the program was stopped due to an error or a stop command */

    for (i = 0; i < NUM_MATS; i++)
        init_mat(mats[i].matrix);

    while (getNextLine(&line) != 0) {
        printf("%s\n", line);   /* print the line back */

        err = lineToInstruction(line, &instr);
        free(line);     /* finished using the line */

        if (((err = handleError(err)) == err_hard) || (legalStop = (instr.cmd == cmd_stop)))
            break;
        else if (err == err_light)  /* error that doesn't require stopping the program */
            continue;       /* skip execution */

        err = executeInstruction(instr, mats, NUM_MATS);
        handleError(err);

        freeInstruction(&instr);
    }

    if (err == err_none && !legalStop)
        printf("Warning: did not receive a stop instruction.");

    return 0;
}

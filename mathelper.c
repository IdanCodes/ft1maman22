#include <stdlib.h>
#include "mathelper.h"
#include "inputhelper.h"

static int toReadMatArgs(Argument args[], double *result);
static ErrCode getNMats(Argument args[], MatId mats[], int numMats, int numMatsToGet, int **matIndexes);
static ErrCode threeMatsOperation(Argument args[], MatId mats[], int numMats, void (*operation)(mat mat1, mat mat2, mat *dest));

static ErrCode exec_read_mat(Argument args[], MatId mats[], int numMats);
static ErrCode exec_print_mat(Argument args[], MatId mats[], int numMats);
static ErrCode exec_mul_scalar(Argument args[], MatId mats[], int numMats);
static ErrCode exec_trans_mat(Argument args[], MatId mats[], int numMats);

/* executeInstruction: execute a given instruction on a matrix from the given matrix array.
 * assuming the given instruction is a valid instruction */
ErrCode executeInstruction(Instruction instr, MatId mats[], int numMats) {
    switch (instr.cmd) {
        case cmd_read:
            return exec_read_mat(instr.args, mats, numMats);

        case cmd_print:
            return exec_print_mat(instr.args, mats, numMats);

        case cmd_add:
            return threeMatsOperation(instr.args, mats, numMats, add_mat);

        case cmd_sub:
            return threeMatsOperation(instr.args, mats, numMats, sub_mat);

        case cmd_mulm:
            return threeMatsOperation(instr.args, mats, numMats, mul_mat);

        case cmd_muls:
            return exec_mul_scalar(instr.args, mats, numMats);

        case cmd_trans:
            return exec_trans_mat(instr.args, mats, numMats);

        case cmd_stop:
            return err_none;
    }
}

/* getMatIndex: get the index of a matrix in the array from its strVal.
 * returns whteher there was an error (could only be err_none or err_undef_mat) */
ErrCode getMatIndex(char *str, MatId mats[], int numMats, int *matIndex) {
    int i;
    int tokLen;

    tokLen = getTokLen(str);

    for (i = 0; i < numMats; i++) {
        if (eqlToLen(str, mats[i].name, tokLen)) {
            *matIndex = i;
            return err_none;
        }
    }

    /* didn't find the matching matrix */
    return err_undef_mat;
}

/* toReadMatArgs: converts Argument[] to read_mat's required array type. (arg[0] is the matrix's strVal!)
 * assumes the given Arguments are valid. */
static int toReadMatArgs(Argument args[], double *result) {
    int i;

    for (i = 1; i < CMD_READ_NARGS && args[i].hasValue; i++)
        result[i - 1] = args[i].value.scalar;

    return i;
}

/* getNMats: get an array of the indexes of the first [numMatsToGet] matrices specified by name in args.
 * free matIndexes after use */
static ErrCode getNMats(Argument args[], MatId mats[], int numMats, int numMatsToGet, int **matIndexes) {
    int i;
    ErrCode err;

    *matIndexes = (int *)malloc(sizeof(int) * numMatsToGet);

    if (*matIndexes == NULL)
        return err_insuf_mem;

    for (i = 0; i < numMatsToGet; i++) {
        err = getMatIndex(args[i].value.strVal, mats, numMats, *matIndexes + i);
        if (err != err_none)
            return err;
    }

    return err_none;
}

/* threeMatsOperation: execute a given matrix operation that takes three matrices with the given arguments */
static ErrCode threeMatsOperation(Argument args[], MatId mats[], int numMats, void (*operation)(mat mat1, mat mat2, mat *dest)) {
    const int numMatsToGet = 3;
    ErrCode err;
    int *matIndexes;

    err = getNMats(args, mats, numMats, numMatsToGet, &matIndexes);
    if (err != err_none)
        return err;

    operation(*mats[matIndexes[0]].matrix, *mats[matIndexes[1]].matrix, mats[matIndexes[2]].matrix);

    free(matIndexes);
    return err_none;
}

/* exec_read_mat: execute the read_mat matrix operation with the given arguments */
static ErrCode exec_read_mat(Argument args[], MatId mats[], int numMats) {
    int matIndex, numArgs;
    double doubleArgs[CMD_READ_NARGS];
    ErrCode err;

    /* first given argument is the matrix to read into */
    err = getMatIndex(args[0].value.strVal, mats, numMats, &matIndex);
    if (err != err_none)
        return err;

    numArgs = toReadMatArgs(args, doubleArgs);

    read_mat(mats[matIndex].matrix, doubleArgs, numArgs);

    return err_none;
}

/* exec_print_mat: execute the print_mat matrix operation with the given arguments */
static ErrCode exec_print_mat(Argument args[], MatId mats[], int numMats) {
    int matIndex;
    ErrCode err;

    err = getMatIndex(args[0].value.strVal, mats, numMats, &matIndex);
    if (err != err_none)
        return err;

    print_mat(*(mats[matIndex].matrix));

    return err_none;
}

/* exec_mul_scalar: execute the mul_scalar matrix operation with the given arguments */
static ErrCode exec_mul_scalar(Argument args[], MatId mats[], int numMats) {
    ErrCode err;
    int mat1Index, mat2Index;
    double scalar;

    /* fetch first matrix */
    if ((err = getMatIndex(args[0].value.strVal, mats, numMats, &mat1Index)) != err_none)
        return err;

    /* fetch scalar */
    scalar = args[1].value.scalar;

    /* fetch second matrix */
    if ((err = getMatIndex(args[2].value.strVal, mats, numMats, &mat2Index)) != err_none)
        return err;

    mul_scalar(*(mats[mat1Index].matrix), scalar, mats[mat2Index].matrix);

    return err_none;
}

/* exec_trans_mat: execute the trans_mat matrix operation with the given arguments */
static ErrCode exec_trans_mat(Argument args[], MatId mats[], int numMats) {
    ErrCode err;
    int *matIndexes;

    /* get the 2 matrices from the given arguments */
    err = getNMats(args, mats, numMats, 2, &matIndexes);
    if (err != err_none)
        return err;

    trans_mat(*(mats[matIndexes[0]].matrix), mats[matIndexes[1]].matrix);

    free(matIndexes);
    return err_none;
}

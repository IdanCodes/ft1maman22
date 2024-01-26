#ifndef MAT_HELPER
#define MAT_HELPER

#include "mymat.h"
#include "inputhelper.h"

/* MatId: this structure represents the connection
 * between a matrix's strVal (how the user should refer to it in input) and it's address */
typedef struct {
    char *name;
    mat *matrix;
} MatId;

ErrCode executeInstruction(Instruction instr, MatId mats[], int numMats);
ErrCode getMatIndex(char *str, MatId mats[], int numMats, int *matIndex);

#endif

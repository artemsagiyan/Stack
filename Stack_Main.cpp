#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "Stack.h"

int main(int argc, char **argv) {
   
   Stack st     = {};
   Info  inf_st = {};

   inf_st.error = NO_ERROR;

   StackCtor(&st, &inf_st);

   for (int i = 0; i < 10; ++i) {
        StackPush(&st, sqrt(i), &inf_st);
    }         

   StackPrint(st, &inf_st);

   StackDtor(&st, &inf_st);
}
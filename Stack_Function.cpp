#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctime>

#include "Stack.h"

int StackCtor(Stack *stack_ptr, INFO_STACK){

    inf_st->File = fopen(LOG_FILE_NAME, "w+");
    if (inf_st->File == NULL) return NOT_OPEN_FILE;

    stack_ptr->size     = 0;
    stack_ptr->capacity = 0;

    char *new_data = (char *)calloc((stack_ptr->capacity + 1) * sizeof(Stack_t), sizeof(char));
    
    int mem = sizeof(Canary_t) * sizeof(char);
    char    *new_canary_data_front = new_data - mem;
             new_canary_data_front = (char *)calloc(1, mem);
    char    *new_canary_data_back  = new_data + stack_ptr->capacity;
             new_canary_data_back  = (char *)calloc(1, mem);
    
    stack_ptr->stack_front_canary = (Canary_t *)new_canary_data_front;
    stack_ptr->stack_back_canary  = (Canary_t *)new_canary_data_back;

    set_canary

    stack_ptr->data = (Stack_t *)(new_data);
    if (stack_ptr->data == NULL) inf_st->error = MEM_ERROR;

    stack_ptr->hash = StackHashLy(stack_ptr);

    ASSERT_OK(stack_ptr, inf_st);

    return NO_ERROR;
}

int StackPush(Stack *stack_ptr, Stack_t value, INFO_STACK){
    
    ASSERT_OK(stack_ptr, inf_st)

    if(stack_ptr->size >= stack_ptr->capacity) {
        StackResize(stack_ptr, stack_ptr->capacity * 2 + 1, inf_st);
    }

    stack_ptr->data[stack_ptr->size++] = value;

    stack_ptr->hash = StackHashLy(stack_ptr);
    
    ASSERT_OK(stack_ptr, inf_st)

    return NO_ERROR;
}

int StackPop(Stack *stack_ptr, INFO_STACK) {
    
    ASSERT_OK(stack_ptr, inf_st)

    Stack_t value = stack_ptr->data[stack_ptr->size--];
    stack_ptr->data[stack_ptr->size + 1] = 666;

    if(stack_ptr->size <= stack_ptr->capacity / 4) {
        StackResize(stack_ptr, stack_ptr->capacity + 1, inf_st);
        memset((stack_ptr->data + stack_ptr->capacity + 1), (int)666, 
              ((stack_ptr->capacity * 3) / 4 + 1));
    }

    stack_ptr->hash = StackHashLy(stack_ptr);

    ASSERT_OK(stack_ptr, inf_st)

    return value;
}

int StackResize(Stack *stack_ptr, int new_capacity, INFO_STACK){

    ASSERT_OK(stack_ptr, inf_st)

    char *new_data = (char *)realloc((char *)stack_ptr->data, new_capacity * sizeof(Stack_t));
    if(new_data == NULL){
        inf_st->error = REALLOC_ERROR;
        return REALLOC_ERROR;
    }

    stack_ptr->data = (Stack_t *)new_data;
    
    stack_ptr->capacity = new_capacity;

    set_canary

    stack_ptr->hash = StackHashLy(stack_ptr);

    ASSERT_OK(stack_ptr, inf_st)

    return NO_ERROR;
}

void StackDtor(Stack *stack_ptr, INFO_STACK) {
    
    ASSERT_OK(stack_ptr, inf_st)

    memset(stack_ptr->data, (Stack_t)666, 
           stack_ptr->capacity);
    memset(stack_ptr->stack_back_canary, 
          (Canary_t)666, sizeof(Canary_t));
    memset(stack_ptr->stack_front_canary, 
          (Canary_t)666, sizeof(Canary_t));

    free  (stack_ptr->data);
    free  (stack_ptr->stack_back_canary);
    free  (stack_ptr->stack_front_canary);


    stack_ptr->stack_back_canary  = (Canary_t *)13;
    stack_ptr->stack_front_canary = (Canary_t *)13;    
    stack_ptr->data               = (Stack_t *) 13;
    stack_ptr->size               =             -1;
    stack_ptr->capacity           =             -1;

    fclose(inf_st->File);
}

void StackPrint(const Stack stack, INFO_STACK) {
    
    ASSERT_OK((Stack *)&stack, inf_st)

    for (int i = 0; i <= stack.capacity + 1; ++i) {
        PrintElem(stack.data[i]);
    }
    printf("\n");
}

void PrintElem(int elem) {
    printf("%d ", elem);
}
void PrintElem(double elem) {
    printf("%.3lf ", elem);
}
void PrintElem(char elem) {
    printf("%c ", elem);
}

void FPrintElem(int size, int num, int elem, FILE *file) {
    int count_ent = CountEnt(size) - CountEnt(num);
    fprintf(file, "\t\t\t*[%d] ", num);

    for(int i = 0; i < count_ent; ++i) {
        fprintf(file, " ");
    }
    fprintf(file, "= %d\n", elem);
}

void FPrintElem(int size, int num, double elem, FILE *file) {
    int count_ent = CountEnt(size) - CountEnt(num);
    
    fprintf(file, "\t\t\t*[%d] ", num);

    for(int i = 0; i < count_ent; ++i) {
        fprintf(file, " ");
    }
    fprintf(file, "= %.2lf\n", elem);
}

void FPrintElem(int size, int num, char elem, FILE *file) {
    int count_ent = CountEnt(size) - CountEnt(num);
    fprintf(file, "\t\t\t*[%d] ", num);

    for(int i = 0; i < count_ent; ++i) {
        fprintf(file, " ");
    }
    fprintf(file, "= %c\n", elem);
}

int StackOk(const Stack *stack_ptr, INFO_STACK) {
    
    CheckError(stack_ptr->data == NULL          ,     INVALID_PTR_DATA      );
    CheckError(stack_ptr       == NULL          ,     INVALID_PTR_STACK     );    
    CheckError(stack_ptr->capacity < 0          ,     NEGATIVE_CAPACITY     );
    CheckError(stack_ptr->size     < 0          ,     NEGATIVE_SIZE_STACK   );
    CheckError(stack_ptr->capacity < 
               stack_ptr->size                  ,     CAPACITY_LOW_THEN_SIZE);
    CheckError(stack_ptr->canary_back  !=
               stack_ptr->stack_back_canary[0]  ,     INVALID_BACK_CANARY   );
    CheckError(stack_ptr->canary_front !=
               stack_ptr->stack_front_canary[0] ,     INVALID_FRONT_CANARY  );
    CheckError(stack_ptr->hash         !=
               StackHashLy(stack_ptr)           ,     INVALID_HASH_KEY      );
    
    //CheckError(IsReadBadPtr(stack_ptr->data),   INVALID_PTR           );
    return NO_ERROR;
}

int IsReadBadPtr(Stack_t *ptr) {
    pid_t res = fork();
    exit(res);
    if(res < 0) return FAILED_PTR;
    return 0;
}

void PrintCanary(const Stack *stack_ptr, INFO_STACK, int canary) {
    fprintf(inf_st->File, "\t\t\t*[C_1] = %d (CANARY)\n", stack_ptr->stack_front_canary[0]);
    fprintf(inf_st->File, "\t\t\t*[C_2] = %d (CANARY)\n", stack_ptr->stack_back_canary[0]);
    fprintf(inf_st->File, "\n");
}

void StackDump(const Stack *stack_ptr, INFO_STACK) {

    Stack_t canary = 1;

    if(inf_st->error == NO_ERROR) {

        fprintf(inf_st->File, "\n******************************"
        "************************************************************\n");

        fprintf(inf_st->File, "Stack <%s> (OK) [%p]\n", type_str(Stack_t), stack_ptr);
        fprintf(inf_st->File, "\t\tIn file : %s\n",  __FILE__);
        fprintf(inf_st->File, "\t\tFunction: %s\n",  __func__);
        fprintf(inf_st->File, "\t\tLine    : %d\n",  __LINE__);
        fprintf(inf_st->File, "\t\tSize    : %d\n",  stack_ptr->size);
        fprintf(inf_st->File, "\t\tCapacity: %d\n",  stack_ptr->capacity);
        fprintf(inf_st->File, "\t\tData [%p]\n\n" ,  stack_ptr->data);

        fprintf(inf_st->File, "\t\tElements:\n");   

        if (stack_ptr->size == 0)
            fprintf(inf_st->File, "\t\tThere aren't elements\n");

        for (int i = 0; i < stack_ptr->size; ++i) {
            FPrintElem(stack_ptr->size, i, stack_ptr->data[i], inf_st->File);
        }

        fprintf(inf_st->File, "\n");

        PrintCanary(stack_ptr, inf_st, canary);
        
        fprintf(inf_st->File, "ALL ELEMENTS HAVE FIILED IN POISON\n******************************"
        "************************************************************\n");
    
    }
    else {

        switch(inf_st->error) {
            case MEM_ERROR:
                CASE(str(MEM_ERROR))
                break;
            case REALLOC_ERROR:
                CASE(str(REALLOC_ERROR))
                break;
            case NOT_MEM_STACK:
                CASE(str(NOT_MEM_STACK))
                break;
            case NULL_SIZE_CAPACITY:
                CASE(str(NULL_SIZE_CAPACITY))
                break;
            case NEGATIVE_SIZE_STACK:
                CASE(str(NEGATIVE_SIZE_STACK))
                break;
            case INVALID_PTR_STACK:
                CASE(str(INVALID_PTR_STACK))
                break;
            case INVALID_PTR:
                CASE(str(INVALID_PTR))
                break;
            case INVALID_PTR_DATA:
                CASE(str(INVALID_PTR_DATA))
                break;
            case CAPACITY_LOW_THEN_SIZE:
                CASE(str(CAPACITY_LOW_THEN_SIZE))
                break;
            case FAILED_PTR:
                CASE(str(FAILED_PTR))
                break;
            default: 
                CASE(str(NO_ERROR))
        }
    }
}

int CountEnt(int num) {
    
    if (num == 0) return 0;

    int name = num;
    int i = -1;

    while(name != 0) {
        ++i;
        name /= 10;
    }

    return i;
}

void  SetCanary(Stack *stack_ptr, INFO_STACK) {
    
    ASSERT_OK(stack_ptr, inf_st)

    srandom(time(NULL));

    stack_ptr->stack_back_canary[0]  = stack_ptr->canary_back  = random();
    stack_ptr->stack_front_canary[0] = stack_ptr->canary_front = random();
}

Hash_t StackHashLy(const Stack *stack_ptr) {

    Hash_t hash = 0;

    //hashing stack elements

    for(int i = 0; i < stack_ptr->size; ++i) {
        
        hash += (Hash_t)(stack_ptr->data[i]);
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    //hashing canaries

    hash += (Hash_t)(stack_ptr->canary_back);
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    hash += (Hash_t)(stack_ptr->canary_front);
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

#undef Stack_t
#undef Canary_t
#undef LOG_FILE_NAME
#undef set_canary
#undef str
#undef type_str
#undef CheckError
#undef ASSERT_OK
#undef INFO_STACK
#undef CASE
#undef Hash_t
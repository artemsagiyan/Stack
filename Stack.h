#include <stdio.h>
#include <ctime>

//#define CANARY_TURN

//#define STACK_TYPE

//#define HASH_TURN

#ifndef Stack_t
#define Stack_t int
#endif

#ifndef CANARY_TURN
#define Canary_t int
  const Canary_t canary = 0xDFFDFFFD;
#endif

#ifndef HASH_TURN
#define Hash_t unsigned long int
#endif

#ifndef LOG_FILE_NAME
#define LOG_FILE_NAME  "LogStack.txt"      
#endif

#define str(type_first) #type_first
#define type_str(type) str(type)

#define CheckError(condition, error) if(condition) return error;

#define ASSERT_OK(stack_ptr, inf_st)                                                      \
                            inf_st->error = StackOk(stack_ptr, inf_st);                   \
                            if(inf_st->error == NO_ERROR)                                 \
                                StackDump(stack_ptr, inf_st);                             \

#define INFO_STACK struct Info *inf_st
//============================================================================================
//HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO0000=
//============================================================================================
//ERROR_VERSION_STACK_DUMP

#ifndef CANARY_TURN
#define PrintCan PrintCanary(stack_ptr, inf_st, canary);
#endif

#ifdef CANARY_TURN
#define PrintCan //not output canaries
#endif

#define CASE(ERROR) fprintf(inf_st->File, "\n******************************"                        \
        "************************************************************\n");                          \
                                                                                                    \
        fprintf(inf_st->File, "Stack <%s> (ERROR: %s) [%p]\n", type_str(Stack_t), ERROR, stack_ptr);\
        fprintf(inf_st->File, "\t\tIn file : %s\n",  __FILE__);                                     \
        fprintf(inf_st->File, "\t\tFunction: %s\n",  __func__);                                     \
        fprintf(inf_st->File, "\t\tLine    : %d\n",  __LINE__);                                     \
        fprintf(inf_st->File, "\t\tSize    : %d\n",  stack_ptr->size);                              \
        fprintf(inf_st->File, "\t\tCapacity: %d\n",  stack_ptr->capacity);                          \
        fprintf(inf_st->File, "\t\tData [%p]\n\n" ,  stack_ptr->data);                              \
                                                                                                    \
        fprintf(inf_st->File, "\t\tElements:\n");                                                   \
                                                                                                    \
        if (stack_ptr->size == 0)                                                                   \
            fprintf(inf_st->File, "\t\tThere aren't elements\n");                                   \
                                                                                                    \
        for (int i = 0; i <= stack_ptr->size; ++i) {                                                \
            FPrintElem(stack_ptr->capacity, i, stack_ptr->data[i], inf_st->File);                   \
        }                                                                                           \
        PrintCan                                                                                    \
        fprintf(inf_st->File, "\n");                                                                \
        fprintf(inf_st->File, "ALL ELEMENTS HAVE FIILED IN POISON\n******************************"  \
        "************************************************************\n");

//============================================================================================
//HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO0000=
//============================================================================================

/******************************************************
 * @brief Structure for a secure stack
 * 
 * @param [in] capacity             - Number of initialized memory cells
 * @param [in] size                 - Number of last stack element
 * @param [in] data                 - Pointer to the beginning of the stack memory
 * @param [in] stack_front_canary   - Pointer to the begginnig of the front canary
 * @param [in] stack_back_canary    - Pointer to the begginnig of the back  canary
 * @param [in] canary_front         - Front canary
 * @param [in] canary_back          - Back  canary            
 */

#ifndef CANARY_TURN
#define CANARY_FRONT const Canary_t canary_front = canary;
#endif

#ifdef CANARY_TURN
#define CANARY_FRONT //NOT CANARIES
#endif

#ifndef CANARY_TURN
#define CANARY_BACK const Canary_t canary_back = canary;
#endif

#ifdef CANARY_TURN
#define CANARY_BACK //NOT CANARIES
#endif

struct Stack {
    CANARY_FRONT

    int      capacity;
    int      size;
    Stack_t  *data;

    #ifndef HASH_TURN
        Hash_t hash;
    #endif

    CANARY_BACK
};

//============================================================================================
//HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO0000=
//============================================================================================

/********************************************************
 * @brief Structure for information of Stack
 * 
 * @param [in] file  - Number of current file
 * @param [in] line  - Number of current line
 * @param [in] error - Number of has done error
 * @param [in] File  - Output file
 * 
 */
struct Info {
    char *file   ;
    int   line   ;
    int   error  ;
    FILE *File   ;
};

//============================================================================================
//HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO0000=
//============================================================================================

/********************************************************
 * @details This enam contains memory 
 *          errors that can occur when
 *          going off the stack boundary,
 *          with an invalid pointer,
 *          and errors about changing 
 *          canaries when trying to hack memory. 
 */
enum errors {

    NO_ERROR                    =    0,

    MEM_ERROR                   =    101,
    REALLOC_ERROR               =    102,
    NOT_OPEN_FILE               =    103,
    INVALID_HASH_KEY            =    104,

    NOT_MEM_STACK               =    201,
    NULL_SIZE_CAPACITY          =    202,
    NEGATIVE_CAPACITY           =    203,
    NEGATIVE_SIZE_STACK         =    204,
    INVALID_PTR                 =    205,
    INVALID_PTR_STACK           =    206,
    INVALID_PTR_DATA            =    207,
    CAPACITY_LOW_THEN_SIZE      =    208,
    FAILED_PTR                  =    209,

    DIV_NONE                    =   1001

};

//============================================================================================
//HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO0000=
//============================================================================================

/*********************************************************
 * @brief Constructs a secure stack
 * 
 * @param stack_ptr      [in]  - Pointer to the beginning of the stack
 * 
 * @return int           [out] - Result about proccessing construct
 */
int   StackCtor         (Stack *stack_ptr, INFO_STACK);

//============================================================================================
//HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO0000=
//============================================================================================

/*********************************************************
 * @brief Push 1 element on top of the stack
 * 
 * @param stack_ptr      [in]  - Pointer to the beginning of the stack
 * @param value          [in]  - The lying element 
 * 
 * @return int           [out] - Result about proccessing pushing
 */
int   StackPush         (Stack *stack_ptr, Stack_t value, INFO_STACK);

//============================================================================================
//HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO0000=
//============================================================================================
                         
/**********************************************************
 * @brief Take 1 element on top of the stack
 *                       
 * @param stack_ptr      [in]  - Pointer to the beginning of the stack
 * 
 * @return int           [out] - Result about proccessing pushing
 */
int   StackPop          (Stack *stack_ptr, INFO_STACK);

//============================================================================================
//HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO0000=
//============================================================================================

/**********************************************************
 * @brief Destroys all stack data and floods them with poison
 * 
 * @param stack_ptr      [in]  - Pointer to the beginning of the stack
 */
void  StackDtor         (Stack *stack_ptr, INFO_STACK);

//============================================================================================
//HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO0000=
//============================================================================================

/***********************************************************
 * @brief Сhanges stack size and allocates new memory
 * 
 * @param stack_ptr      [in]  - Pointer to the beginning of the stack
 * @param new_capacity   [in]  - New count of allocate memery
 * 
 * @return int           [out] - Result about proccessing pushing
 */
int   StackResize       (Stack *stack_ptr, int new_capacity, INFO_STACK);

//============================================================================================
//HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO0000=
//============================================================================================

/***********************************************************
 * @brief Prints all stack elements
 * 
 * @param stack_ptr      [in]  - Pointer to the beginning of the stack
 */
void  StackPrint        (const Stack stack, INFO_STACK);

//============================================================================================
//HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO0000=
//============================================================================================

/***********************************************************
 * @brief Сhecking the stack for validity
 * 
 * @param stack_ptr      [in]  - Pointer to the beginning of the stack
 * 
 * @return int           [out] - Result of check on valid
 */
int   StackOk           (const Stack *stack_ptr, INFO_STACK);

//============================================================================================
//HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO0000=
//============================================================================================

/***********************************************************
 * @brief Сhecking for an invalid pointer
 * 
 * @param  ptr           [in]  - Pointer for cheching
 * @return int           [out] - Result   of checking
 */
int   IsReadBadPtr      (Stack_t *ptr);

//============================================================================================
//HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO0000=
//============================================================================================

/***********************************************************
 * @details displays information about 
 *          the current state of the stack
 *          or about an error, giving full 
 *          information 
 * 
 * @param stack_ptr 
 */
void  StackDump         (const Stack *stack_ptr, INFO_STACK);

//============================================================================================
//HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO0000=
//============================================================================================

/************************************************************
 * @brief Prints a stack element of display
 * 
 * @param elem            [in]  - element of stack
 */
void  PrintElem         (Stack_t elem);

//============================================================================================
//HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO0000=
//============================================================================================

/************************************************************
 * @brief Prints a stack element to a file
 * 
 * @param size            [in]  - size of stack
 * @param num             [in]  - number elements
 * @param elem            [in]  - element of stack
 * @param file            [in]  - output file
 */
void  FPrintElem        (int size, int num, char elem, FILE *file);

//============================================================================================
//HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO0000=
//============================================================================================

/*************************************************************
 * @brief Number of spaces to output
 * 
 * @param num             [in]  - number
 * @return int            [out] - count of spaces
 */
int   CountEnt          (int num);

//============================================================================================
//HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO0000=
//============================================================================================

#ifndef CANARY_TURN
/**************************************************************
 * @brief Set the Canary object
 * 
 * @param stack_ptr       [in]  - Pointer to the beginning of the stack
 */
void  SetCanary         (Stack *stack_ptr, INFO_STACK);

//============================================================================================
//HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO0000=
//============================================================================================

/***************************************************************
 * @brief Prints canaries
 * 
 * @param stack_ptr       [in]  - Pointer to the beginning of the stack
 * @param canary          [in]  - Value of canary
 */
void  PrintCanary       (const Stack *stack_ptr, INFO_STACK, Canary_t canary);
#endif
//============================================================================================
//HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO0000=
//============================================================================================
#ifndef HASH_TURN
/***************************************************************
 * @brief Make hash keys
 * 
 * @param stack_ptr       [in]  - Pointer to the beginning of the stack
 * 
 * @return                [out] - Hash key
 */
Hash_t StackHashLy      (const Stack *stack_ptr);
#endif
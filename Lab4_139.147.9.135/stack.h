// C code for stack data structure.
//
// Modified by <your-name-here>
//

#ifndef LAB02_STACK_H
#define LAB02_STACK_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h> // this is only needed for the error handling
#include <stddef.h>
#include <errno.h>


struct stack_node {
    struct stack_node *next;
    char   item;
};

// create and initialize a new stack node
// item   - value to store in the stack node
// return - pointer to the new stack node
struct stack_node *generate_stack_node(char item);

// obtain the value at the top of the stack
// stack  - pointer to top of the stack
// return - value stored at the top of the stack
char peek(struct stack_node *stack);

// add new node to top of the stack
// stack - pointer to top of the stack
// item  - item value to add to the top of the stack
// return - pointer to the new top of the stack
struct stack_node *push(struct stack_node *stack, char item);

// remove top of the stack
// stack - pointer to top of the stack
// return - pointer to the new top of the stack
struct stack_node *pop(struct stack_node *stack);

// print the stack top to bottom to the standard out
// stack - pointer to top of the stack
void display_stack(struct stack_node *stack);

#endif //LAB02_SOLUTION_STACK_H

// For the implementation of three different PDA machines.
//
// Modified by <your-name-here>
//

#include "stack.h" // Only this include is needed here.

// create and initialize a new stack node.
struct stack_node *generate_stack_node(char item) {

    struct stack_node *node = malloc(sizeof(struct stack_node) );

    // This is how you do error checking in C.
    if(node == NULL) {
        printf("ERROR: %s\n", __FUNCTION__);
        printf("     : %s\n", strerror(errno));

        // Ends the program.
        exit(-1);
    }

    // initialize new stack node.
    node->next = NULL;
    node->item = item;

    return node;
}

// Obtain the value at the top of the stack.
char peek(struct stack_node *stack) {

    // If not null, return item otherwise null value.
    if(stack)
        return stack->item;
    else
        return '\0';
}

// Add new node to top of the stack.
struct stack_node *push(struct stack_node *stack, char item) {

    struct stack_node* element = generate_stack_node(item);

    // add node to the stack
    element->next = stack;
    
    // return the top of the stack
    return element;
}

// Remove top of the stack.
struct stack_node *pop(struct stack_node *stack) {

    struct stack_node *next = stack->next;

    // Always free memory that is no longer needed.
    free(stack);

    // returns the new top of stack
    return next;
}

void display_stack(struct stack_node *stack) {

    struct stack_node *node = stack;

    printf("stack: ");
    while(node) {
        putchar(node->item);
        node = node->next;
    }
    puts("");
}
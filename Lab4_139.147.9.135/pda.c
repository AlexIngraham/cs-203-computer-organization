// For the implementation of three different PDA machines.
//
// Modified by <your-name-here>
//

#include "pda.h"

struct list_node *list = NULL;

int PDA1 (char *str) {

    enum state state = S0; // set the starting state
    enum state accepting_state = S3; // set the accepting state

    struct stack_node *stack = NULL;

    int i = 0;
    int len = strlen(str);
    while (i < len) {
        char current_char = str[i];

        switch (state) {
            case S0:
                if (current_char == 'a') {
                    stack = push(stack, 'a');
                    state = S1;
                } else {
                    return 0;
                }
                break;
            case S1:
                if (current_char == 'a') {
                    stack = push(stack, 'a');
                } else if (current_char == 'b') {
                    state = S2;
                } else {
                    return 0;
                }
                break;
            case S2:
                if (current_char == 'b') {
                    if (stack == NULL) {
                        return 0;
                    }
                    stack = pop(stack);
                } else {
                    return 0;
                }
                break;
            case S3:
                // This state is not used in this PDA
                return 0;
            default:
                return 0;
        }
        i++;
    }

    // Check if the stack is empty at the end
    if (stack == NULL && state == accepting_state) {
        return 1;
    }
    return 0;
}

int PDA2 (char *str) {

    enum state state = S0;
    enum state accepting_state = S2;

    struct stack_node *stack = NULL;

    int i = 0;
    int len = strlen(str);
    while (i < len) {
        char current_char = str[i];

        if (current_char == ' ') {
            i++;
            continue;
        }
        switch (state) {
            case S0:
            if (current_char == '(') {
                stack = push(stack, '(');
            } else if (current_char == ')') {
                if (stack == NULL) {
                    return 0;
                }
                stack = pop(stack);
            } else {
                return 0;
            }
            break;

        }
        i++;
    }
    if (state == S0 && stack == NULL) {
        return 1;
    }

    /** Implement the second push down automata. **/

    return 0;
}

int PDA3 (char *str) {

    enum state state = S0;
    struct stack_node *stack = NULL;
    int current_scope = 0;

    list = NULL;
    int i = 0;
    int len = strlen(str);

    while (i < len) {
        char current_char = str[i];
        if (current_char == ' ') {
            i++;
            continue;
        }

        switch (state) {
            case S0:
                if (current_char == '{') {
                    current_scope++;
                } else if (current_char == '}') {
                    list = remove_scope_from_list(list, current_scope, '\0');
                    current_scope--;
                    if (current_scope < 0) {
                        return 0;
                    }
                } else if (current_char >= 'a' && current_char <= 'z') {
                    list = add_variable_to_list(list, current_char, current_scope);
                } else if (current_char == ';') {
                    // do nothing
                } else {
                    return 0;
                }
                break;
            default:
                return 0;
        }
        i++;
    }

    return 0;
}

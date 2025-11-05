// List source code file.
//
// Modified by Alex Ingraham
//

#include "list.h" // no additional includes needed here

// Ensure the stack functions are declared
extern struct stack_node *push(struct stack_node *stack, char scope);
extern struct stack_node *pop(struct stack_node *stack);
extern char peek(struct stack_node *stack);

/** Add code for implementing you list functionality. **/

struct list_node *generate_list_node(char item) {

    struct list_node *new_node = malloc(sizeof(struct list_node));

    if (new_node == NULL) {
        fprintf(stderr, "Error: malloc failed in generate_list_node\n");
        return NULL;
    }

    new_node -> item = item;
    new_node -> next = NULL;
    new_node -> stack = NULL;

    return new_node;
}

struct list_node *add_variable_to_list(struct list_node *list, char item, int scope) {

    struct list_node *current = list;
    while (current != NULL) {
        if (current -> item == item) {
            // add to stack
            current -> stack = push(current -> stack, (char)('0' + scope));
            return list;
        }
        current = current -> next;
    }

    struct list_node *new_node = generate_list_node(item);
    new_node -> stack = push(new_node -> stack, (char)('0' + scope));
    new_node -> next = list;
    return new_node;

}


int find_variable_scope(struct list_node *list, char item) {

    struct list_node *current = list;
    while (current != NULL) {
        if (current -> item == item) {
            if (current -> stack != NULL) {
                char scope_char = peek(current -> stack);
                if (scope_char == '\0') return -1;
                return (int)(scope_char - '0');
            }
            return -1;
        }
        current = current -> next;
    }

    return -1;

}

struct list_node *remove_scope_from_list(struct list_node *list, int scope, char variable) {

    struct list_node *current = list;
    while (current != NULL) {
        if (current -> item == variable) {
            if (current -> stack != NULL) {
                current -> stack = pop(current -> stack);
            }
            break;
        }
        current = current -> next;
    }

    (void)scope;
    return list;

}

struct list_node *create_list() {
    return NULL;
}

int list_is_empty(struct list_node *list) {
    return list == NULL;
}

int list_size(struct list_node *list) {
    int count = 0;
    struct list_node *cur = list;
    while (cur) { count++; cur = cur->next; }
    return count;
}

void delete_list(struct list_node *list) {

    while (list != NULL) {
        struct list_node *next = list -> next;
        while (list -> stack != NULL) {
            list -> stack = pop(list -> stack);
        }
        free(list);
        list = next;
    }

}

void print_list (struct list_node *list) {

    printf ("List contents:\n");
    struct list_node *current = list;
    while (current != NULL) {
        printf ("Item: %c, Scopes: ", current -> item);
        if (current -> stack == NULL) {
            printf ("Variable '%c'", current -> item);
        } if (current -> stack != NULL) {
            printf (": Current scope %d\n", find_variable_scope(list, current -> item));
        } else {
            printf ("No scopes\n");
        }

        current = current -> next;
    }
    printf ("\n");
}
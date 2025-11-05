// C code stack unit-tests.
//
// Modified by <your-name-here>
//

#include <assert.h>
#include <stdio.h>

#include "stack.h"

int main () {

    struct stack_node *node = NULL;
    assert(peek(node) == '\0');

    node = generate_stack_node('2');
    assert(node->item == '2');
    assert(node->next == NULL);

    node = pop(node);
    assert(node == NULL);

    node = push(node, 'a');
    assert(peek(node) == 'a');

    node = push(node, 'a');
    node = push(node, 'b');
    node = push(node, 'c');

    display_stack(node);

    assert(node->item == 'c');
    assert(node->next->item == 'b');
    assert(node->next->next->item == 'a');

    node = pop(node);
    assert(node->item == 'b');
    assert(node->next->item == 'a');

    node = pop(node);
    assert(node->item == 'a');

    node = pop(node);
    assert(node->item == 'a');

    node = pop(node);
    assert(node == NULL);

    return 0;
}

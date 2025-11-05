// C code list unit-tests.
//
// Modified by <your-name-here>
//

#include <assert.h>
#include <stdio.h>
#include "list.h"

int main () {

    printf("Create and destroy empty list\n");
    struct list_node *list = create_list();
    assert(list == NULL);
    assert(list_is_empty(list));
    assert(list_size(list) == 0);
    delete_list(list);
    printf("Passed\n");


    printf("Add one variable\n");
    list = create_list();
    list = add_variable_to_list(list, 'a', 0);
    assert(!list_is_empty(list));
    assert(list_size(list) == 1);
    assert(find_variable_scope(list, 'a') == 0);
    assert(find_variable_scope(list, 'b') == -1);
    delete_list(list);
    printf("Passed\n");

    printf("Add multiple variables\n");
    list = create_list();
    list = add_variable_to_list(list, 'a', 0);
    list = add_variable_to_list(list, 'b', 0);
    list = add_variable_to_list(list, 'c', 1);
    assert(list_size(list) == 3);
    assert(find_variable_scope(list, 'a') == 0);
    assert(find_variable_scope(list, 'b') == 0);
    assert(find_variable_scope(list, 'c') == 1);
    delete_list(list);
    printf("Passed\n");


    printf("Variable shadowing\n");
    list = create_list();
    list = add_variable_to_list(list, 'a', 0);
    list = add_variable_to_list(list, 'b', 0);
    assert(list_size(list) == 2);
    assert(find_variable_scope(list, 'a') == 0);
    assert(find_variable_scope(list, 'b') == 0);
    delete_list(list);
    printf("Passed\n");

    printf("Remove scope from list\n");
    list = create_list();
    list = add_variable_to_list(list, 'a', 0);
    list = add_variable_to_list(list, 'a', 1);
    assert(find_variable_scope(list, 'a') == 1);
    list = remove_scope_from_list(list, 1, 'a');
    assert(find_variable_scope(list, 'a') == 0);
    delete_list(list);
    printf("Passed\n");


    printf("Node creation and delete\n");
    struct list_node *node = generate_list_node('a');
    assert (node != NULL);
    assert (node->item == 'a');
    assert (node->stack == NULL);
    assert (node->next == NULL);
    delete_list(node);
    printf("Passed\n");

    printf("All tests passed\n");
    return 0;
}

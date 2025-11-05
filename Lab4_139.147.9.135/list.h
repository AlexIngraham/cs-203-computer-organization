// C code for a singly linked list.
//
// Modified by Alex Ingraham
//

#ifndef LAB02_LIST_H
#define LAB02_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <errno.h>

#include "stack.h"

struct list_node {
    struct list_node *next;
    char item;
    struct stack_node* stack;
};

struct variable {
    int size;
    struct list_node *head;
};

struct list_node *generate_list_node(char item);
struct list_node *add_variable_to_list(struct list_node *list, char item, int scope);
int find_variable_scope(struct list_node *list, char item);
struct list_node *remove_scope_from_list(struct list_node *list, int scope, char variable);
void delete_list(struct list_node *list);
void print_list (struct list_node *list);
struct list_node *create_list();
int list_is_empty(struct list_node *list);
int list_size(struct list_node *list);



#endif //LAB02_LIST_H

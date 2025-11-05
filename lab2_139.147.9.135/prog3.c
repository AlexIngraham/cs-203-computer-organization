/*
* Prints the names of each file and directory in the file system tree, starting a point in the tree
* When run without arguments, it starts with working directory and prints contents
* in all subdirectories recursively. 
*
*
* ./prog3 traverses from current directory
* ./prog3 <directory> traverses from <directory>
*
* Author: Alex Ingraham
* Date: September 7, 2025
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

/*
* Prints the directory tree structure starting from the given path.
* @param path: The path to the directory
* @param depth: The current depth in the directory tree
*/
void print_tree(const char *path, int depth) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        for (int i = 0; i < depth; i++) {
            printf("  ");
        }
        printf("%s\n", entry->d_name);

        char sub_path[PATH_MAX];
        snprintf(sub_path, sizeof(sub_path), "%s/%s", path, entry->d_name);
        
        struct stat file_stat;
        if (stat(sub_path, &file_stat) == 0 && S_ISDIR(file_stat.st_mode)) {
            print_tree(sub_path, depth + 1);
        }
    } 

    closedir(dir);
}

/*
* Main function to parse command-line arguments and initiate directory tree printing.
* @param argc: Argument count
* @param argv: Argument vector
*/
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return 1;
    }

    print_tree(argv[1], 0);
    return 0;
}

/*
*
* File Information
*   
* Mimics behavior of the Unix stat command, calls stat system call
* on a given file directory, prints information, from metadata to
* information on alteration.
*
* Usage: ./prog1 <filename>
* Author: Alex Ingraham
* Date: 2025, Sept. 7
*/

#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

/* 
* printInfo: prints detailed information about the file or the directory
* @filename: Name of the file to display information for
* @file_info: The stat structure containing file information
*/

void printInfo(const char *filename, struct stat file_info) {
    // file metadata
    printf("File Name: %s\n", filename);
    printf("Size: %ld bytes\n", file_info.st_size);
    printf("Blocks: %ld\n", file_info.st_blocks);
    printf("Block Size: %ld bytes\n", file_info.st_blksize);
    printf("Links: %ld\n", file_info.st_nlink);
    printf("Inode Number: %llu\n", (unsigned long long)file_info.st_ino);
    printf("Permissions: %o\n", file_info.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
    printf("Device: %llu\n", (unsigned long long)file_info.st_dev);

    // file type
    if (S_ISREG(file_info.st_mode)) {
        printf("Type: Regular File\n");
    } else if (S_ISDIR(file_info.st_mode)) {
        printf("Type: Directory\n");
    } else if (S_ISLNK(file_info.st_mode)) {
        printf("Type: Symbolic Link\n");
    } else {
        printf("Type: Other\n");
    }

    // file change timestamps
    printf("Last Modified: %s", ctime(&file_info.st_mtime));
    printf("Last Status Changed: %s", ctime(&file_info.st_ctime));
    printf("Last Accessed: %s", ctime(&file_info.st_atime));
}

/*
* main: Entry point of the program
* @argc: Argument count
* @argv: Argument vector
*
* Returns 0 on success, -1 on failure
*/
int main(int argc, char *argv[]) {
    struct stat file_info;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (stat(argv[1], &file_info) == -1) {
        perror("Error getting file information");
        return EXIT_FAILURE;
    }
    printInfo(argv[1], file_info);
    return EXIT_SUCCESS;
}

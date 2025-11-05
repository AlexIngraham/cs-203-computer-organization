/*
* File: prog2.c
* Program lists files in a given directory
* When called without -l, prints only file names
* When invoked with -l, prints detailed info about each file
* Details permissions, owner, group, size, modifications
*
* Usage: ./prog2 [-l] [directory]
* ./prog2 [directory]
*
* Author: Alex Ingraham
* Date: 2025, Sep. 7
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

/*
* Prints the file permissions in a human-readable format.
* @mode: The mode from the stat structure
* @perm_str: The string to store the permission representation
*/
void print_permissions(mode_t mode, char *perm_str) 
{
    if (S_ISDIR(mode)) strcpy(perm_str, "d");
    else if (S_ISLNK(mode)) strcpy(perm_str, "l");
    else strcpy(perm_str, "-");

    perm_str[1] = (mode & S_IRUSR) ? 'r' : '-';
    perm_str[2] = (mode & S_IWUSR) ? 'w' : '-';
    perm_str[3] = (mode & S_IXUSR) ? 'x' : '-';
    perm_str[4] = (mode & S_IRGRP) ? 'r' : '-';
    perm_str[5] = (mode & S_IWGRP) ? 'w' : '-';
    perm_str[6] = (mode & S_IXGRP) ? 'x' : '-';
    perm_str[7] = (mode & S_IROTH) ? 'r' : '-';
    perm_str[8] = (mode & S_IWOTH) ? 'w' : '-';
    perm_str[9] = (mode & S_IXOTH) ? 'x' : '-';
    perm_str[10] = '\0';
}


/*
* Prints detailed information about a file.
* @filename: The name of the file
* @filepath: The full path to the file
*/
void print_detail(const char *filename, const char *filepath)
{
    struct stat file_stat;
    if (stat(filepath, &file_stat) == -1) {
        perror("stat");
        return;
    }

    char perm_str[11];
    print_permissions(file_stat.st_mode, perm_str);

    struct passwd *owner = getpwuid(file_stat.st_uid);
    struct group *group = getgrgid(file_stat.st_gid);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&file_stat.st_mtime));

    printf("%s %3ld %-8s %-8s %8ld %s %s\n",
           perm_str,
           file_stat.st_nlink,
           owner ? owner->pw_name : "unknown",
           group ? group->gr_name : "unknown",
           file_stat.st_size,
           time_str,
           filename);

}

/*
* Lists files in a directory. If detail is 1, prints detailed information about each file.
* @dir_path: The path to the directory
* @detail: Whether to print detailed information
*/
void list_directory(const char *dir_path, int detail)
{
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir))) {
        if (entry->d_name[0] == '.') continue;  // Skip hidden files

        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s/%s", dir_path, entry->d_name);

        if (detail) {
            print_detail(entry->d_name, filepath);
        } else {
            printf("%s\n", entry->d_name);
        }
    }

    closedir(dir);
}

/*
* Main function to parse command-line arguments and list directory contents.
* @argc: Argument count
* @argv: Argument vector
*
* Returns: 0 on success, -1 on failurei
*/
int main(int argc, char *argv[])
{
    int detail = 0;
    const char *dir_path = ".";

    if (argc > 1) {
        if (strcmp(argv[1], "-l") == 0) {
            detail = 1;
            if (argc > 2) dir_path = argv[2];
        } else {
            dir_path = argv[1];
        }
    }

    list_directory(dir_path, detail);
    return 0;
}


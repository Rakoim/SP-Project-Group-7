#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

void create_directory(const char *path) {
    if (mkdir(path, 0777) == 0) {
        printf("Directory created successfully: %s\n", path);
    } else {
        perror("Error creating directory");
    }
}

void remove_directory(const char *path) {
    if (rmdir(path) == 0) {
        printf("Directory removed successfully: %s\n", path);
    } else {
        perror("Error removing directory");
    }
}

void print_current_directory() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("Error getting current directory");
    }
}

void list_current_directory() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("Error getting current directory");
        return;
    }

    DIR *dir = opendir(cwd);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    printf("Contents of directory '%s':\n", cwd);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
}

int main() {
    int choice;
    char path[1024];

    do {
        printf("\nDirectory Operations Menu:\n");
        printf("1. Create Directory\n");
        printf("2. Remove Directory\n");
        printf("3. Print Current Working Directory\n");
        printf("4. List Current Directory Contents\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the path for the new directory: ");
                scanf("%s", path);
                create_directory(path);
                break;
            case 2:
                printf("Enter the path of the directory to remove: ");
                scanf("%s", path);
                remove_directory(path);
                break;
            case 3:
                print_current_directory();
                break;
            case 4:
                list_current_directory();
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}

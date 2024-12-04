#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <termios.h>

void print_menu();
void file_operations();
void directory_operations();
void keylogger();

// Function prototypes for individual operations
void create_file(const char *filename);
void change_permissions(const char *filename, mode_t mode);
void read_file(const char *filename);
void write_file(const char *filename, const char *content);
void delete_file(const char *filename);

void create_directory(const char *dirname);
void delete_directory(const char *dirname);
void list_directory(const char *dirname);

int main(int argc, char *argv[]) {
    if (argc > 1) {
        // Handle command-line options
        if (strcmp(argv[1], "-m") == 0 && argc >= 4) {
            int mode = atoi(argv[2]);  // Get the operation mode
            switch (mode) {
                case 1:  // File operations mode
                    if (argc == 5) {
                        int op = atoi(argv[3]);  // Get the file operation type
                        const char *filename = argv[4];
                        switch (op) {
                            case 1:  // Create file
                                create_file(filename);
                                break;
                            case 2:  // Write to file
                                printf("Enter content to write: ");
                                char content[256];
                                scanf("%s", content);
                                write_file(filename, content);
                                break;
                            case 3:  // Read file
                                read_file(filename);
                                break;
                            case 4:  // Delete file
                                delete_file(filename);
                                break;
                            default:
                                fprintf(stderr, "Invalid file operation.\n");
                        }
                    } else {
                        fprintf(stderr, "Invalid number of arguments for file operation.\n");
                    }
                    break;
                case 2:  // Directory operations mode
                    if (argc == 5) {
                        int op = atoi(argv[3]);  // Get the directory operation type
                        const char *dirname = argv[4];
                        switch (op) {
                            case 1:  // Create directory
                                create_directory(dirname);
                                break;
                            case 2:  // Delete directory
                                delete_directory(dirname);
                                break;
                            case 3:  // List directory contents
                                list_directory(dirname);
                                break;
                            default:
                                fprintf(stderr, "Invalid directory operation.\n");
                        }
                    } else {
                        fprintf(stderr, "Invalid number of arguments for directory operation.\n");
                    }
                    break;
                case 3:  // Keylogger operations mode
                    if (argc == 4) {
                        const char *logfile = argv[3];
                        keylogger(logfile);
                    } else {
                        fprintf(stderr, "Invalid number of arguments for keylogger operation.\n");
                    }
                    break;
                default:
                    fprintf(stderr, "Invalid mode.\n");
            }
            return 0;
        } else {
            fprintf(stderr, "Usage: ./supercommand -m <mode> <sub-mode> <parameters...>\n");
            return 1;
        }
    }

    // Interactive menu system (for users who prefer a menu-based system)
    while (1) {
        print_menu();
        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                file_operations();
                break;
            case 2:
                directory_operations();
                break;
            case 3:
                keylogger(NULL);  // Start keylogger without a file in interactive mode
                break;
            case 4:
                printf("Exiting program.\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

void print_menu() {
    printf("\nSuperCommand Menu:\n");
    printf("1. File Operations\n");
    printf("2. Directory Operations\n");
    printf("3. Keylogger (Demo)\n");
    printf("4. Exit\n");
}

void file_operations() {
    printf("\nFile Operations:\n");
    printf("1. Create a file\n");
    printf("2. Write to a file\n");
    printf("3. Read a file\n");
    printf("4. Delete a file\n");

    int choice;
    char filename[256], content[256];
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Enter filename: ");
            scanf("%s", filename);
            create_file(filename);
            break;
        case 2:
            printf("Enter filename: ");
            scanf("%s", filename);
            printf("Enter content: ");
            scanf("%s", content);
            write_file(filename, content);
            break;
        case 3:
            printf("Enter filename: ");
            scanf("%s", filename);
            read_file(filename);
            break;
        case 4:
            printf("Enter filename: ");
            scanf("%s", filename);
            delete_file(filename);
            break;
        default:
            printf("Invalid choice.\n");
    }
}

void directory_operations() {
    printf("\nDirectory Operations:\n");
    printf("1. Create a directory\n");
    printf("2. Delete a directory\n");
    printf("3. List contents of a directory\n");

    int choice;
    char dirname[256];
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Enter directory name: ");
            scanf("%s", dirname);
            create_directory(dirname);
            break;
        case 2:
            printf("Enter directory name: ");
            scanf("%s", dirname);
            delete_directory(dirname);
            break;
        case 3:
            printf("Enter directory name: ");
            scanf("%s", dirname);
            list_directory(dirname);
            break;
        default:
            printf("Invalid choice.\n");
    }
}

void create_file(const char *filename) {
    int fd = open(filename, O_CREAT | O_WRONLY, 0644);
    if (fd == -1) {
        perror("Error creating file");
        return;
    }
    printf("File created successfully: %s\n", filename);
    close(fd);
}

void write_file(const char *filename, const char *content) {
    int fd = open(filename, O_WRONLY | O_APPEND);
    if (fd == -1) {
        perror("Error opening file for writing");
        return;
    }
    if (write(fd, content, strlen(content)) == -1) {
        perror("Error writing to file");
    } else {
        printf("Content written successfully to file: %s\n", filename);
    }
    close(fd);
}

void read_file(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file for reading");
        return;
    }
    char buffer[256];
    ssize_t bytesRead;
    while ((bytesRead = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytesRead] = '\0';
        printf("%s", buffer);
    }
    printf("\n");
    close(fd);
}

void delete_file(const char *filename) {
    if (unlink(filename) == -1) {
        perror("Error deleting file");
    } else {
        printf("File deleted successfully: %s\n", filename);
    }
}

void create_directory(const char *dirname) {
    if (mkdir(dirname, 0755) == -1) {
        perror("Error creating directory");
    } else {
        printf("Directory created successfully: %s\n", dirname);
    }
}

void delete_directory(const char *dirname) {
    if (rmdir(dirname) == -1) {
        perror("Error deleting directory");
    } else {
        printf("Directory deleted successfully: %s\n", dirname);
    }
}

void list_directory(const char *dirname) {
    DIR *dir = opendir(dirname);
    if (!dir) {
        perror("Error opening directory");
        return;
    }
    struct dirent *entry;
    printf("Contents of directory %s:\n", dirname);
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
}

void keylogger(const char *logfile) {
    printf("\nKeylogger Demo: Press keys (Press ESC to exit)\n");

    struct termios oldt, newt;
    char ch;

    // Get current terminal attributes
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Disable canonical mode and echo
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    FILE *logFile = fopen(logfile ? logfile : "keylog.txt", "a");
    if (!logFile) {
        perror("Error opening log file");
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return;
    }

    while (1) {
        ch = getchar();

        // Exit on ESC key
        if (ch == 27) {
            break;
        }

        // Log pressed key to file
        fprintf(logFile, "%c", ch);
    }

    fclose(logFile);

    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

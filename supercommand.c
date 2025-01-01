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
#include <ctype.h>
#include <signal.h>
#include <time.h>

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
void print_current_directory();
void list_current_directory_contents();

// Signal handler to handle Ctrl+C (SIGINT)
void handle_sigint(int sig) {
    printf("\nExecution stopped by user. Exiting...\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    // Register signal handler for SIGINT
    signal(SIGINT, handle_sigint);

    if (argc > 1) {
        // Handle command-line options
        if (strcmp(argv[1], "-m") == 0 && argc >= 4) {
            int mode = atoi(argv[2]);  // Get the operation mode
            switch (mode) {
                case 1:  // File operations mode
                    if (argc >= 5) {
                        int op = atoi(argv[3]);  // Get the file operation type
                        const char *filename = argv[4];
                        switch (op) {
                            case 1:  // Create file
                                create_file(filename);
                                break;
                            case 2:  // Change permissions
                                mode_t mode = strtol(argv[5], NULL, 8);  // Read permissions in octal format
                                change_permissions(filename, mode);
                                break;
                            case 3:  // Read file
                                read_file(filename);
                                break;
                            case 4:  // Write to file
                                const char *content = argv[5];  // Content passed for writing
                                write_file(filename, content);
                                break;
                            case 5:  // Delete file
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
                    if (argc >= 4) {
                        int op = atoi(argv[3]);  // Get the directory operation type
                        const char *dirname = argv[4];
                        switch (op) {
                            case 1:  // Create directory
                                create_directory(dirname);
                                break;
                            case 2:  // Delete directory
                                delete_directory(dirname);
                                break;
                            case 3:  // Print current directory
                                print_current_directory();
                                break;
                            case 4:  // List current directory contents
                                list_current_directory_contents();
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
                        keylogger();
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

while (1) {
    print_menu();
    char input[256]; // Buffer to capture user input
    int choice;

    printf("Enter your choice: ");
    scanf("%s", input); // Read input as a string

    // Validate if the input is a valid number
    int is_number = 1;
    for (int i = 0; input[i] != '\0'; i++) {
        if (!isdigit(input[i])) {
            is_number = 0;
            break;
        }
    }

    if (!is_number) {
        printf("Value error: Please enter a valid number.\n");
        continue; // Restart the loop to prompt the user again
    }

    choice = atoi(input); // Convert the validated input to an integer

    switch (choice) {
        case 1:
            file_operations();
            break;
        case 2:
            directory_operations();
            break;
        case 3:
            keylogger();
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
    printf("3. Keylogger\n");
    printf("4. Exit\n");
}

void file_operations() {
    while(1){
        printf("\nFile Operations:\n");
        printf("1. Create a file\n");
        printf("2. Change permissions of a file\n");
        printf("3. Read a file\n");
        printf("4. Write to a file\n");
        printf("5. Delete a file\n");
        printf("6. Return to Main Menu\n");

        char input[256];
        int choice;

        while (1) { // Loop to handle invalid input
            printf("Enter your choice: ");
            scanf("%s", input); // Read input as a string

            // Validate if the input is a valid number
            int is_number = 1;
            for (int i = 0; input[i] != '\0'; i++) {
                if (!isdigit(input[i])) {
                    is_number = 0;
                    break;
                }
            }

            if (!is_number) {
                printf("Value error: Please enter a valid number.\n");
                continue; // Restart the loop to prompt the user again
            }

            choice = atoi(input); // Convert the validated input to an integer
            break; // Exit the loop once valid input is received
        }

        char filename[256], content[256];
        switch (choice) {
            case 1:
                printf("Enter filename: ");
                scanf("%s", filename);
                create_file(filename);
                break;
            case 2:
                mode_t mode; // Declare mode_t variable
                printf("Enter filename: ");
                scanf("%s", filename);
                printf("Enter permissions (in octal, e.g., 0644): ");
                scanf("%o", &mode); // Read octal permission input
                change_permissions(filename, mode);
                break;
            case 3:
                printf("Enter filename: ");
                scanf("%s", filename);
                read_file(filename);
                break;
            case 4:
                printf("Enter filename: ");
                scanf("%s", filename);
                getchar(); // Clear the newline left by scanf
                printf("Enter content: ");
                fgets(content, sizeof(content), stdin);
                content[strcspn(content, "\n")] = 0; // Remove trailing newline
                write_file(filename, content);
                break;
            case 5:
                printf("Enter filename: ");
                scanf("%s", filename);
                delete_file(filename);
                break;
            case 6:
                return;
            default:
                printf("Invalid choice.\n");
        }
    }
}

void directory_operations() {
    while (1) { // Keep looping until the user explicitly chooses to return to the main menu
        printf("\nDirectory Operations:\n");
        printf("1. Create a directory\n");
        printf("2. Delete a directory\n");
        printf("3. Print current working directory\n");
        printf("4. List contents of the current directory\n");
        printf("5. Return to Main Menu\n");

        char input[256];
        int choice;

        while (1) { // Loop to handle invalid input
            printf("Enter your choice: ");
            scanf("%s", input); // Read input as a string

            // Validate if the input is a valid number
            int is_number = 1;
            for (int i = 0; input[i] != '\0'; i++) {
                if (!isdigit(input[i])) {
                    is_number = 0;
                    break;
                }
            }

            if (!is_number) {
                printf("Value error: Please enter a valid number.\n");
                continue; // Restart the loop to prompt the user again
            }

            choice = atoi(input); // Convert the validated input to an integer
            break; // Exit the loop once valid input is received
        }

        // Convert the valid input to an integer
        choice = atoi(input);
        char dirname[256];

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
                print_current_directory();
                break;
            case 4:
                list_current_directory_contents();
                break;
            case 5:
                return; // Exit the Directory Operations menu and go back to the main menu
            default:
                printf("Invalid choice. Please try again.\n");
        }
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

void change_permissions(const char *filename, mode_t mode) {
    if (chmod(filename, mode) == -1) {
        perror("Error changing file permissions");
    } else {
        printf("Permissions changed successfully for file: %s\n", filename);
    }
}

void read_file(const char *filename) {
    // Open the file for reading
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file for reading");
        return;
    }

    // Get the file size using fstat
    struct stat fileStat;
    if (fstat(fd, &fileStat) == -1) {
        perror("Error getting file stats");
        close(fd);
        return;
    }

    // Dynamically allocate memory for the buffer based on the file size
    size_t fileSize = fileStat.st_size;
    if (fileSize == 0) {
        printf("The file is empty.\n");
        close(fd);
        return;
    }

    // Allocate memory for the buffer
    char *buffer = (char *)malloc(fileSize + 1);  // +1 for the null terminator
    if (buffer == NULL) {
        perror("Memory allocation failed");
        close(fd);
        return;
    }

    // Read the file into the buffer
    ssize_t bytesRead = read(fd, buffer, fileSize);
    if (bytesRead == -1) {
        perror("Error reading file");
        free(buffer);
        close(fd);
        return;
    }

    // Null-terminate the buffer
    buffer[bytesRead] = '\0';

    // Print the content of the file
    printf("%s", buffer);

    // Clean up: Free the buffer and close the file descriptor
    free(buffer);
    close(fd);

    printf("\n");
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

void print_current_directory() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("Error getting current working directory");
    }
}

void list_current_directory_contents() {
    char cwd[PATH_MAX];
    
    // Get the current working directory
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("Error getting current directory");
        return;
    }

    DIR *dir = opendir(cwd);
    if (!dir) {
        perror("Error opening directory");
        return;
    }

    struct dirent *entry;
    printf("Contents of current directory %s:\n", cwd);
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
}

void keylogger() {
    printf("\nKeylogger: Press keys (Press ESC to exit)\n");

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

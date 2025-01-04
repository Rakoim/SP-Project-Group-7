Supercommand
NAME
supercommand - A command-line utility for file, directory, and keylogger operations.

SYNOPSIS
supercommand [-m <mode> <sub-mode> <parameters>]
DESCRIPTION
supercommand provides a set of operations for file management, directory management, and keylogging.

The program can be run in two modes:

Command-line mode (using the -m flag)
Interactive menu mode (without the -m flag)
OPTIONS
-m <mode> <sub-mode> <parameters> Run the program in command-line mode with specific operation types.

Modes and Sub-modes:
File operations (mode 1):

1: Create a file
2: Change permissions of a file
3: Read a file
4: Write to a file
5: Delete a file
Directory operations (mode 2):

1: Create a directory
2: Delete a directory
3: Print current working directory
4: List contents of the current directory
Keylogger operations (mode 3):

1: Start keylogger (logs keys pressed to keylog.txt)
2: View keylogger contents (keylog.txt)
Parameters:
For file operations, provide the filename and content (if writing).
For directory operations, provide the directory name.
For keylogger operations, provide the logfile name (optional).
EXAMPLES
Interactive mode: Run the program with a menu

$ ./supercommand
Description: Runs the program in interactive mode, where the user is prompted to select operations via a menu.

File operation example: Create a file

$ ./supercommand -m 1 1 myfile.txt
Description: Creates a file named myfile.txt.

File operation example: Change file permissions

$ ./supercommand -m 1 2 myfile.txt 0755
Description: Changes the permissions of myfile.txt to 0755 (rwxr-xr-x).

File operation example: Read a file

$ ./supercommand -m 1 3 myfile.txt
Description: Reads and displays the content of myfile.txt.

File operation example: Write to a file

$ ./supercommand -m 1 4 myfile.txt "Hello, World!"
Description: Writes "Hello, World!" to the file myfile.txt.

File operation example: Delete a file

$ ./supercommand -m 1 5 myfile.txt
Description: Deletes the file myfile.txt.

Directory operation example: Create a directory

$ ./supercommand -m 2 1 newdir
Description: Creates a directory named newdir.

Directory operation example: Delete a directory

$ ./supercommand -m 2 2 newdir
Description: Deletes the directory newdir.

Directory operation example: Print current working directory

$ ./supercommand -m 2 3
Description: Prints the current working directory to the terminal.

Directory operation example: List contents of the current directory

$ ./supercommand -m 2 4
Description: Lists the contents of the current working directory.

Keylogger operation example: Start keylogger

$ sudo ./supercommand -m 3 1
To stop keylogging, press ALT+CTRL+SHIFT+ESC keys together from anywhere. Description: Starts the keylogger and logs keys pressed from the keyboard anywhere (text file, browser, etc.) to the file keylog.txt. Note: Must be run with sudo.

Keylogger operation example: View keylogger contents

$ ./supercommand -m 3 2
Description: Shows the contents of the keylog.txt file.
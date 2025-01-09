# Supercommand

## DESCRIPTION
**supercommand** is a C program that provides a set of operations for file management, directory management, and keylogging in Linux.

The program can be run in two modes:
1. Command-line mode (using the `-m` flag)
2. Interactive menu mode (without the `-m` flag)

## COMPILATION
To compile the program, place all the files in a directory (avoid using a shared folder due to potential permission issues) and run the following command:
```sh
gcc supercommand.c -o supercommand
```

## OPTIONS
`-m <mode> <sub-mode> <parameters>`
Run the program in command-line mode with specific operation types.

### Modes and Sub-modes:
- **File operations (mode 1):**
  - `1`: Create a file
  - `2`: Change permissions of a file
  - `3`: Read a file
  - `4`: Write to a file
  - `5`: Delete a file

- **Directory operations (mode 2):**
  - `1`: Create a directory
  - `2`: Delete a directory
  - `3`: Print current working directory
  - `4`: List contents of the current directory

- **Keylogger operations (mode 3):**
  - `1`: Start keylogger (logs keys pressed to `keylog.txt`)
  - `2`: View keylogger contents (`keylog.txt`)

### Parameters:
- For file operations, provide the filename and content (if writing).
- For directory operations, provide the directory name.

## EXAMPLES

1. **Interactive mode: Run the program with a menu**
    ```sh
    $ ./supercommand
    $ sudo ./supercommand (sudo is required for starting keylogger)
    ```
    Description: Runs the program in interactive mode, where the user is prompted to select operations via a menu.

2. **File operation example: Create a file**
    ```sh
    $ ./supercommand -m 1 1 /path/to/file/myfile.txt
    ```
    Description: Creates a file named `myfile.txt` at the specified path.

3. **File operation example: Change file permissions**
    ```sh
    $ ./supercommand -m 1 2 /path/to/file/myfile.txt 0755
    ```
    Description: Changes the permissions of `myfile.txt` to `0755` (rwxr-xr-x) at the specified path.

4. **File operation example: Read a file**
    ```sh
    $ ./supercommand -m 1 3 /path/to/file/myfile.txt
    ```
    Description: Reads and displays the content of `myfile.txt` from the specified path.

5. **File operation example: Write to a file**
    ```sh
    $ ./supercommand -m 1 4 /path/to/file/myfile.txt "Hello, World!"
    ```
    Description: Writes "Hello, World!" to the file `myfile.txt` at the specified path.

6. **File operation example: Delete a file**
    ```sh
    $ ./supercommand -m 1 5 /path/to/file/myfile.txt
    ```
    Description: Deletes the file `myfile.txt` at the specified path.

7. **Directory operation example: Create a directory**
    ```sh
    $ ./supercommand -m 2 1 /path/to/directory/newdir
    ```
    Description: Creates a directory named `newdir` at the specified path.

8. **Directory operation example: Delete a directory**
    ```sh
    $ ./supercommand -m 2 2 /path/to/directory/newdir
    ```
    Description: Deletes the directory `newdir` from the specified path.

9. **Directory operation example: Print current working directory**
    ```sh
    $ ./supercommand -m 2 3
    ```
    Description: Prints the current working directory to the terminal.

10. **Directory operation example: List contents of the current directory**
    ```sh
    $ ./supercommand -m 2 4
    ```
    Description: Lists the contents of the current working directory.

11. **Keylogger operation example: Start keylogger**
    ```sh
    $ sudo ./supercommand -m 3 1
    ```
    To stop keylogging, press `ALT+CTRL+SHIFT+ESC` keys together from anywhere.
    Description: Starts the keylogger and logs keys pressed from the keyboard anywhere (text file, browser, etc.) to the file `keylog.txt`. Note: Must be run with `sudo`.

12. **Keylogger operation example: View keylogger contents**
    ```sh
    $ ./supercommand -m 3 2
    ```
    Description: Shows the contents of the `keylog.txt` file.

---

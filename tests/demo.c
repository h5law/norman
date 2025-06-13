/*
Copyright (c) 2025 h5law <dev@h5law.com>

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgement in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
 */
/* simple syscall (syscall-arm64.S + system.h) test program
 * СИГМА - БАСЕД ДЕВ
 */

#include <stdio.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 2) {
        print("Usage: %s <filename>\n");
        return 1;
    }

    // Open the file using our syscall function
    print("Opening file...\n");
    int fd = open(argv[1], O_RDONLY, 0);
    if (fd < 0) {
        print("Error opening file\n");
        return 1;
    }

    print("Successfully opened file\n");

    // Create a buffer for reading
    char    buffer[4096];
    ssize_t bytes_read;
    ssize_t total_bytes = 0;

    // Read and display the file contents
    print("\n--- File Contents ---\n");

    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        // Null-terminate the buffer
        buffer[bytes_read] = '\0';

        // Write to stdout
        write(STDOUT_FILENO, buffer, bytes_read);

        total_bytes += bytes_read;
    }

    print("\n--- End of File ---\n");

    if (bytes_read < 0) {
        print("Error reading file\n");
    }

    // Close the file
    if (close(fd) < 0) {
        print("Error closing file\n");
    }

    return 0;
}

// vim: ft=c ts=4 sts=4 sw=4 et ai cin

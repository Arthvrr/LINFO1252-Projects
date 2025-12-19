#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "lib_tar.h"

/**
 * You are free to use this file to write tests for your implementation
 */

void debug_dump(const uint8_t *bytes, size_t len) {
    for (int i = 0; i < len;) {
        printf("%04x:  ", (int) i);

        for (int j = 0; j < 16 && i + j < len; j++) {
            printf("%02x ", bytes[i + j]);
        }
        printf("\t");
        for (int j = 0; j < 16 && i < len; j++, i++) {
            printf("%c ", bytes[i]);
        }
        printf("\n");
    }
}

/**
 * Test the exists function with a given path and file descriptor.
 *
 * @param fd The file descriptor for the tar archive.
 * @param path The path of the file to check within the archive.
 */
void test_exists(int fd, char *path) {
    int result = exists(fd, path);
    if (result == 0) {
        printf("Entry '%s' does not exist in the archive.\n", path);
    } else {
        printf("Entry '%s' exists in the archive.\n", path);
    }
}

/**
 * Test the is_dir function with a given path and file descriptor.
 *
 * @param fd The file descriptor for the tar archive.
 * @param path The path of the file to check within the archive.
 */
void test_is_dir(int fd, char *path) {
    int result = is_dir(fd, path);
    if (result) {
        printf("Entry '%s' is a directory in the archive.\n", path);
    } else {
        printf("Entry '%s' is not a directory in the archive or does not exist.\n", path);
    }
}

/**
 * Test the is_file function with a given path and file descriptor.
 *
 * @param fd The file descriptor for the tar archive.
 * @param path The path of the file to check within the archive.
 */
void test_is_file(int fd, char *path) {
    int result = is_file(fd, path);
    if (result) {
        printf("Entry '%s' is a file in the archive.\n", path);
    } else {
        printf("Entry '%s' is not a file in the archive or does not exist.\n", path);
    }
}

/**
 * Test the is_symlink function with a given path and file descriptor.
 *
 * @param fd The file descriptor for the tar archive.
 * @param path The path of the entry to check within the archive.
 */
void test_is_symlink(int fd, char *path) {
    int result = is_symlink(fd, path);
    if (result) {
        printf("Entry '%s' is a symlink in the archive.\n", path);
    } else {
        printf("Entry '%s' is not a symlink in the archive or does not exist.\n", path);
    }
}

/**
 * Test the list function with a given path and file descriptor.
 *
 * @param fd The file descriptor for the tar archive.
 * @param path The path of the directory to list within the archive.
 */
void test_list(int fd) {
    char *entries[10];
    for (int i = 0; i < 10; i++) {
        entries[i] = malloc(100);
    }
    size_t no_entries = 10;

    char *path = "./test_directory/";
    int result = list(fd, path, entries, &no_entries);

    if (result == 0) {
        printf("Path '%s' is not a directory or does not exist.\n", path);
    } else {
        printf("Entries in '%s':\n", path);
        for (size_t i = 0; i < no_entries; i++) {
            printf("  - %s\n", entries[i]);
        }
    }

    for (int i = 0; i < 10; i++) {
        free(entries[i]);
    }
}

/**
 * Test the read_file function by attempting to read a file at a given path in the archive.
 *
 * @param fd The file descriptor for the tar archive.
 * @param path The path of the file to read within the archive.
 */
void test_read_file(const char *archive_path, const char *file_path, size_t offset, size_t buffer_size) {
    int tar_fd = open(archive_path, O_RDONLY);
    if (tar_fd < 0) {
        perror("open");
        return;
    }

    uint8_t *buffer = malloc(buffer_size);
    if (!buffer) {
        perror("malloc");
        close(tar_fd);
        return;
    }

    size_t len = buffer_size;
    ssize_t result = read_file(tar_fd, (char *)file_path, offset, buffer, &len);

    if (result == -1) {
        printf("Test failed: File '%s' not found or is not a regular file.\n", file_path);
    } else if (result == -2) {
        printf("Test failed: Offset %zu is outside the file size.\n", offset);
    } else {
        printf("Test passed: Read %zu bytes from file '%s'.\n", len, file_path);
        if (result > 0) {
            printf("Remaining bytes to read: %zd\n", result);
        } else {
            printf("File '%s' fully read.\n", file_path);
        }
    }

    printf("Buffer content:\n");
    fwrite(buffer, 1, len, stdout);
    printf("\n");

    free(buffer);
    close(tar_fd);
}



//FONCTION MAIN AVEC TOUT LES TESTS
int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s tar_file\n", argv[0]);
        return -1;
    }

    int fd = open(argv[1] , O_RDONLY);
    if (fd == -1) {
        perror("open(tar_file)");
        return -1;
    }

    // int ret = check_archive(fd);
    // printf("check_archive returned %d\n", ret);

    //tests de la fonction exists
    // test_exists(fd, "./test_directory/fichier1.txt");
    // test_exists(fd, "./test_directory/fichier2.txt");
    // test_exists(fd,"./test_directory/fichier3.txt");
    // test_exists(fd, "./test_directory/non_existent_file.txt");

    //tests de la fonction is_dir
    // test_is_dir(fd, "./test_directory/");
    // test_is_dir(fd, "./test_directory/fichier1.txt");
    // test_is_dir(fd, "./test_directory/non_existent_file.txt");

    //tests de la fonction is_file
    // test_is_file(fd, "./test_directory/fichier2.txt");
    // test_is_file(fd, "./test_directory/");
    // test_is_file(fd, "./test_directory/non_existent_file.txt");

    //tests de la fonction is_symlink
    // test_is_symlink(fd, "./test_directory/symlink");
    // test_is_symlink(fd, "./test_directory/fichier1.txt");
    // test_is_symlink(fd, "./test_directory/non_existent_file.txt");

    //tests de la fonction list()
    test_list(fd);

    //tests de la fonction read_file
    // test_read_file("test_archive.tar", "./test_directory/fichier1.txt", 0, 1024);
    // test_read_file("test_archive.tar", "./test_directory/fichier1.txt", 10, 512);
    // test_read_file("test_archive.tar", "./test_directory/fichier2.txt", 10000, 512);
    // test_read_file("test_archive.tar", "./test_directory/non_existent_file.txt", 0, 512);
    // test_read_file("test_archive.tar", "./test_directory/fichier1.txt", 0, 512);


    close(fd);
    return 0;
}


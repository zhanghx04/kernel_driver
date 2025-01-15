#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 256

int main() {
    int fd;
    char write_buf[BUFFER_SIZE] = "Hello from user space";
    char read_buf[BUFFER_SIZE] = {0};

    // Open the device
    fd = open("/dev/simple_char_dev", O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return EXIT_FAILURE;
    }

    printf("Writing message to driver: %s\n", write_buf);

    // Write to the device
    if (write(fd, write_buf, strlen(write_buf)) < 0) {
        perror("Failed to write to device");
        close(fd);
        return EXIT_FAILURE;
    }  

    // Reset file position
    lseek(fd, 0, SEEK_SET);

    // Read from the device
    if (read(fd, read_buf, BUFFER_SIZE) < 0) {
        perror("Failed to read from device");
        close(fd);
        return EXIT_FAILURE;
    }

    printf("Read message from driver: %s\n", read_buf);

    // Close the device
    close(fd);
    return EXIT_SUCCESS;
}
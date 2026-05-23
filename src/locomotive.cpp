#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

#define SERIAL_PORT "/dev/ttyAMA0"
#define MAX_PACKET_SIZE 64

int main() {
    // 1. Open the serial port
    // O_RDWR: Read/Write, O_NOCTTY: Don't make this the controlling terminal
    int fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY);
    if (fd < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return -1;
    }

    struct termios tty;
    if (tcgetattr(fd, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        close(fd);
        return -1;
    }

    // 2. Set the Baud Rate to 1,000,000
    cfsetispeed(&tty, B1000000);
    cfsetospeed(&tty, B1000000);

    // 3. Put the port in RAW mode
    // This is critical. It tells Linux NOT to intercept control characters 
    // or format newlines, treating the data as purely binary.
    cfmakeraw(&tty);

    // 4. Configure read timeouts
    tty.c_cc[VMIN]  = 1; // Block until at least 1 byte arrives
    tty.c_cc[VTIME] = 1; // 0.1 seconds timeout between bytes

    // Apply the settings
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        close(fd);
        return -1;
    }

    printf("Listening for DCC packets on %s at 1Mbps...\n", SERIAL_PORT);

    unsigned char buffer[MAX_PACKET_SIZE];
    int pos = 0;
    unsigned char byte;

    // 5. Read Loop
    while (1) {
        // Read one byte at a time
        int n = read(fd, &byte, 1);
        
        if (n > 0) {
            // In the STM32 Rust code, we added a '\n' (0x0A) to signal the end of the packet
            if (byte == '\n') {
                if (pos > 0) {
                    printf("DCC Packet: ");
                    for (int i = 0; i < pos; i++) {
                        printf("%02X ", buffer[i]);
                    }
                    printf("\n");
                    pos = 0; // Reset buffer for the next packet
                }
            } else {
                // Store the byte if there is room in the buffer
                if (pos < MAX_PACKET_SIZE) {
                    buffer[pos++] = byte;
                }
            }
        } else if (n < 0) {
            printf("Error reading: %s\n", strerror(errno));
            break;
        }
    }

    close(fd);
    return 0;
}

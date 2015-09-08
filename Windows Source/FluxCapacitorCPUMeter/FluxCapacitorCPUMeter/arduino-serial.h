/*
 * Arduino-serial
 * --------------
 */

#ifdef _MSC_VER
 typedef __int8 uint8_t;
#else
 #include <stdint.h>
#endif

#include <fcntl.h>    /* File control definitions */
#include <errno.h>    /* Error number definitions */
#include <unistd.h>   /* UNIX standard function definitions */

/*
 * serialport_init()
 * Open the serial port (say, "COM3") at the bound rate with 8N1.
 */
int serialport_init       (const char* serialport, int baud);

/*
 * serialport_writebyte()
 * Write a single byte to the serial port.
 */
int serialport_writebyte  (int fd, uint8_t b);

/*
 * serialport_write()
 * Write a C string to the serial port.
 */
int serialport_write      (int fd, const char* str);

/*
 * serialport_read_until()
 * Read until a given character (commonly '/n').
 */
int serialport_read_until (int fd, char* buf, char until);

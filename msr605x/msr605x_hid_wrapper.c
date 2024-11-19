/* msr605x */

#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_LEN 128
#define MAX_LEN 63
#define HID_LEN 65

int main(int argc, const char *argv[])
{
   
   int fd;
   unsigned char buf[BUF_LEN];
#define HID_PKT (buf+1)
#define MESSAGE (buf+2)
#define REPORT  buf[0]
   
   fd_set readset;
   int r;
   
   if (argc < 2) {
      fprintf(stderr, "Usage: %s /dev/hidraw0 \n", argv[0]);
      return -1;
   }
   
   fd = open(argv[1], O_RDWR, 0666);
   if ( fd<0 )  {
      perror(argv[1]);
      return -1;
   }
   
   REPORT=0x00; // start report number with 0x00
   while (1) {
      
      FD_ZERO(&readset);
      FD_SET(STDIN_FILENO, &readset);
      FD_SET(fd, &readset);
      
      if ( select(fd + 1, &readset, NULL, NULL, NULL) < 0 ) {
	 perror("select()");
	 break;
      }
      
      if ( FD_ISSET(fd, &readset) ) {
	 r = read( fd, HID_PKT, HID_LEN );
	 if ( r>0 ) write( STDOUT_FILENO, MESSAGE, *HID_PKT & 0x3f );
      }

      if ( FD_ISSET(STDIN_FILENO, &readset) ) {
	 r = read(STDIN_FILENO, MESSAGE, MAX_LEN);
	 *HID_PKT = (unsigned char)r | 0xC0;
	 write( fd, buf, HID_LEN);
	// REPORT++; 
      }
   }
   close( fd );
   return( 0 );
}

   
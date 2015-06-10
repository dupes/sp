#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <stdio.h>

main(int argc, char *argv[])
{
  int i = 0;
  int err;
  char buf[10];
  int fd = open("/dev/ttyUSB0", O_RDWR| O_NOCTTY | O_SYNC );
  struct termios tio;

   memset(buf, 0, sizeof(buf));

  tcgetattr(fd, &tio);
 //tio.c_cflag = B19200 | CS8| CLOCAL| CREAD;
 tio.c_cflag = B115200 | CS8| CLOCAL| CREAD;
 tio.c_iflag = IGNPAR;
 tio.c_oflag = 0;
 tio.c_lflag = 0;
 tcflush(fd, TCIFLUSH);
 tcsetattr(fd, TCSANOW, &tio); 

 if(argv[1][0] == 'r'){
	printf("Read:");
  while(i < 10){
     err = read(fd, &buf[0], 10);
     printf("retval - %d, Data: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", 
	err, buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
     perror("error");
	 memset(buf, 0, sizeof(buf));
     i++;
  }
  }else{
	buf[0]=0x35 + 0;
	buf[1]=0x35 + 1;
	buf[2]=0x35 + 2;
	buf[3]=0x35 + 3;
	buf[4]=0x35 + 4;
	buf[5]=0x35 + 5;
	buf[6]=0x35 + 6;
	buf[7]=0x35 + 7;

	printf("write:\n");
     err = write(fd, &buf[0], 8);
     perror("error");
  }
}

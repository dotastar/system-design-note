#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include  <sys/stat.h>
#include  <cerrno>
#define FALLOC_FL_PUNCH_HOLE 0x02
int main(int argc, char **argv)
{
     FILE *fp = fopen("append.txt", "w");
     for (int i = 0; i < 5; ++i)
          fprintf(fp, "## Test loop %d\n", i); 
     fclose(fp);
     sleep(1);

     int  fid = open("append.txt", O_WRONLY/* | O_APPEND*/);

     struct stat  status;
     fstat(fid, &status);
     printf("INFO: sizeof 'append.txt' is %ld Bytes.\n", status.st_size);
     int ret1 = fallocate(fid,0,0,1024*1024);
     int  ret = fallocate(fid, FALLOC_FL_PUNCH_HOLE,(off_t)status.st_size, 1024);
     if (ret) {
         switch (ret) {
         case  EBADF:
            fprintf(stderr, "ERROR: %d is not a valid file descriptor, or is not opened for writing.\n", fid);
            break;
         case  EFBIG:
              fprintf(stderr, "ERROR: exceed the maximum file size.\n");
              break;
         case  ENOSPC:
              fprintf(stderr, "ERROR: There is not enough space left on the device\n");
               break;
         default:
               break;
        }
     }   

     fstat(fid, &status);
     printf("INFO: sizeof 'append.txt' is %ld Bytes.\n", status.st_size);

     char  *hello = "hello world\n";
     write(fid, hello, 12);
     close(fid);

     return 0;  
 }

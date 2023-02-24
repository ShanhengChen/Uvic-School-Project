#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int fd;
	struct stat sb;
	char temp_label[10];
	int i = 9728; //0x2600
	int file_counter = 0;


	fd = open(argv[1], O_RDWR);
	fstat(fd, &sb);
	char * p = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // p points to the starting pos of your mapped memory
	if (p == MAP_FAILED) {
		printf("Error: failed to map memory\n");exit(1);}

	
	while(p[i] != 0x00){
	if(p[i] != 0x5e && p[i+11] != 0x0f && p[i+26] >1 && p[i+11] != 0x08){file_counter++;}
	if(p[i+11] == 0x08){memcpy(temp_label,p+i,8);}
	i += 32;}
	// os name-----------------------
	printf("OS Name: %s\n",p+3);
	// label name-------------------------
	
	printf("Label of the disk: %s\n",temp_label);
	// total size ------------------------
	printf("Total size of the disk: %lu bytes\n", (uint64_t)sb.st_size);
	// count numbers of free block --------------------------------------------------------
	uint64_t free_counter = 0;
	int start = 2;
	while(start < 2880){
		if(p[start+542] == 0x0){
			free_counter+=1;
		}
		start ++;
	}
	
	printf("Free size of the disk: %ld bytes\n",free_counter*512); // 2714 used
	printf("\n==============\n");
	// count number of files in the disk ---------------------------------------------
	
	printf("number of files in the image: %d\n",file_counter);
	printf("\n==============\n");
	//number of FAT
	printf("Number of FAT copies: %d\n",p[16]);
	//Sector per FAT
	printf("Sector per FAT: %d\n",p[22]);

	munmap(p, sb.st_size); // the modifed the memory data would be mapped to the disk image
	close(fd);
	return 0;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define timeOffset 14 //offset of creation time in directory entry
#define dateOffset 16 //offset of creation date in directory entry

typedef struct
{
    
    char file_name[10]; // file name
	char type[8];	// file extension
    char attri[2]; //store attribute
	int pointer; //p
	int size;	// file size

} info;		

void print_date_time(char * directory_entry_startPos);
char * strtrimc( char * s );
void trversal(info event);
int main(int argc, char *argv[])
{
	int fd;
	struct stat sb;
	info event[10];
	int i = 9728; //0x2600
	int info_counter = 0;

	for(int i = 0 ; i < 10; i++){event[i].size = -1;}
	fd = open(argv[1], O_RDWR);
	fstat(fd, &sb);
	char * p = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // p points to the starting pos of your mapped memory
	if (p == MAP_FAILED) {
		printf("Error: failed to map memory\n");exit(1);}
	//---------------------------------------------------------- store all accessable file or subdirectory into struct info 
	printf("ROOT\n");
	printf("==================\n");	
	while(p[i] != 0x00){
	if(p[i] != 0x5e && p[i+11] != 0x0f && p[i+26] >1 && p[i+11] != 0x08){ 
		char temp_name[9];
		memcpy(temp_name,p+i,7);
		strcpy(event[info_counter].file_name,strtrimc(temp_name));
		char temp_type[5];
		
		memcpy(temp_type,p+i+8,4);
		if(p[i+11] != 0x10){
		strcpy(event[info_counter].type,strtrimc(temp_type));}
		else{
			strcpy(event[info_counter].type,"");
		}

		if(p[i+11] == 0x10){
			strcpy(event[info_counter].attri,"D");
		}else{strcpy(event[info_counter].attri,"F");}
		// get file size using & and | operation 
		unsigned int first = (p[i+28] & 0xFF);
		unsigned int second = (p[i+29] & 0xFF) << 8 ;
		unsigned int thrid = (p[i+30] & 0xFF)  << 16;
		unsigned int fourth = (p[i+31] & 0xFF) << 24 ;
		unsigned int  size = 0x0;
		size = size | first;
		size = size | second;
		size = size | thrid;
		size = size | fourth;
		event[info_counter].size = size;
		event[info_counter].pointer = i;
	}	
	info_counter ++;
	i += 32;
	}
	//print all file or sub from struct info handle file and sub differently
	for(int i = 0 ; i < 10; i++){
		if(event[i].size  > 0){
		printf("%s %10d %10s.%s \t",event[i].attri,event[i].size,event[i].file_name,event[i].type);
		print_date_time(p + event[i].pointer);
		}else if(event[i].size  == 0){
		printf("%s %10d %10s %s \t",event[i].attri,event[i].size,event[i].file_name,event[i].type);
		print_date_time(p + event[i].pointer);
		}
	}
	// check file or sub in sub pritn it out directly
		info secondevent[10];
		int temp = 0;
		for(int i = 0 ; i < 10; i++){
		if(event[i].size  == 0){
		printf("/%s\n",event[i].file_name);	
		printf("==================\n");
		unsigned int one = (p[event[i].pointer+26] & 0xFF);
		unsigned int two = (p[event[i].pointer+27] & 0xFF);
		unsigned int cluster = 0x0;
		cluster = one + two + 31;
		cluster = cluster*512 +64;
		while(p[cluster] != 0x00){
		if(p[cluster+11] != 0x02 && p[cluster+11] !=  0x20){ 
		if(p[cluster+11] == 0x10){
			printf("%s","D");
		}else{printf("%s","F");}
		unsigned int first = (p[cluster+28] & 0xFF);
		unsigned int second = (p[cluster+29] & 0xFF) << 8 ;
		unsigned int thrid = (p[cluster+30] & 0xFF)  << 16;
		unsigned int fourth = (p[cluster+31] & 0xFF) << 24 ;
		unsigned int  size = 0x0;
		size = size | first;
		size = size | second;
		size = size | thrid;
		size = size | fourth;
		char temp_name[9];
		memcpy(temp_name,p+cluster,7);
		char temp_type[5];
		memcpy(temp_type,p+cluster+8,4);
		printf("%10d %10s.%s \t",size,strtrimc(temp_name),strtrimc(temp_type));
		print_date_time(p + cluster);
		
		if(p[cluster+11] == 0x10){
			strcpy(secondevent[temp].attri,"F");
			strcpy(secondevent[temp].file_name,strtrimc(temp_name));
			strcpy(secondevent[temp].type,strtrimc(temp_type));
			secondevent[temp].pointer = cluster;
			secondevent[temp].size = size;
			temp ++;
		}
		}
		cluster += 32;
		}
	}}


	munmap(p, sb.st_size); // the modifed the memory data would be mapped to the disk image
	close(fd);
	return 0;
}


void print_date_time(char * directory_entry_startPos){ // print t
	int time, date;
	int hours, minutes, day, month, year;	
	time = *(unsigned short *)(directory_entry_startPos + timeOffset);
	date = *(unsigned short *)(directory_entry_startPos + dateOffset);
	year = ((date & 0xFE00) >> 9) + 1980;
	month = (date & 0x1E0) >> 5;
	day = (date & 0x1F);
	hours = (time & 0xF800) >> 11;
	minutes = (time & 0x7E0) >> 5;
	printf("%5d/%02d/%02d %02d:%02d\n", year, month, day, hours, minutes);
	return ;	
}
char * strtrimc( char * s ) // discard blank space \t and V for stirng S 
{
    char * p1 = s;
	char * p2 = s;
	while(*p1 != '\0')
	{
		while(*p1 == ' ' || *p1 == '\t' || *p1 == 'V')
		{
			p1 ++;
		}
		* p2 ++ = *p1 ++;
	}
	*p2 = '\0';
	return (s);
}


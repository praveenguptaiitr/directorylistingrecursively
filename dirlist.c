#include "dirlist.h"
#include <sys/time.h>

long int total_word_counts;
int total_num_txt_files;
int total_num_other_files;
int total_dir_counts;
int print;

int main(int argc, char** argv)
{
	time_t start_time = time(NULL);
	struct tm* start_watch_time;
	start_watch_time = localtime(&start_time);

#ifdef _ENABLE_LOGS_INFO_TIME_
	printf("\nthread: [%ld] function: [%s] line: [%d] Program Start Time: [%02d:%02d:%02d]\n\n", pthread_self(), __func__, __LINE__,  
		start_watch_time->tm_hour, start_watch_time->tm_min, start_watch_time->tm_sec);
#endif

	char printoption = 0;
#ifdef _ENABLE_LOGS_VERBOSE_
	printf("thread: [%ld] function: [%s] line: [%d]\n", pthread_self(), __func__, __LINE__);
#endif
	int opt;
	int index = 0;
	char filepath[PATH_MAX] = {0};

#ifdef _ENABLE_LOGS_VERBOSE_
	printf("thread: [%ld] function: [%s] line: [%d] arg count: [%d]\n", pthread_self(), __func__, __LINE__,argc);
#endif
	if(argc != 2 && argc != 4)
	{
#ifdef _ENABLE_LOGS_ERR_
		printf("thread: [%ld] function: [%s] line: [%d] dirlist usage: ./dirlist <file path> <-p true/false/1/0>\n", pthread_self(), __func__, __LINE__);
#endif
		return -1;
	}

	
	while((opt = getopt(argc, argv, "p:")) != -1)
	{
		switch(opt)
		{
			case 'p':
#ifdef _ENABLE_LOGS_INFO_
				printf("thread: [%ld] function: [%s] line: [%d] option: [%c] - \'p\' option value: [%s]\n", pthread_self(), __func__, __LINE__, opt, optarg);
#endif
				printoption = 1;
				if(strcmp(optarg,"true") == 0 || strcmp(optarg, "1") == 0)
					print = 1;
				else if(strcmp(optarg,"false") == 0 || strcmp(optarg, "0") == 0)
					print = 0;
				else
				{
				#ifdef _ENABLE_LOGS_ERR_
					printf("thread: [%ld] function: [%s] line: [%d] \'p\' option can be either true/false/1/0 value, entered value: [%s]\n", pthread_self(), __func__, __LINE__, optarg);
					return -1;
				#endif
				}
				break;
			case ':':
#ifdef _ENABLE_LOGS_VERBOSE_
				printf("thread: [%ld] function: [%s] line: [%d] \':\' option needs value\n", pthread_self(), __func__, __LINE__);
#endif
				break;
			case '?':
#ifdef _ENABLE_LOGS_VERBOSE_
				printf("thread: [%ld] function: [%s] line: [%d] unknown option: [%c]\n", pthread_self(), __func__, __LINE__, optopt);
#endif
				break;
		}
	}

	for(index = optind; index < argc ; index++)
	{
#ifdef _ENABLE_LOGS_INFO_
		printf("thread: [%ld] function: [%s] line: [%d] other arguments: [%s] argument index: [%d]\n", pthread_self(), __func__, __LINE__, argv[index], index+1);
#endif
	}

	// remove path ended with '/' or '\'
	if(printoption)
		index = 3;
	else
		index = 1;
	int argvlen = strlen(argv[index]);
	int cnt;
	for(cnt = 0; cnt< (argvlen-1); cnt++)
	{
		filepath[cnt] = argv[index][cnt];
	}
	if((argv[index][cnt] == 0x2f) || (argv[index][cnt] == 0x5c)) 
	// check if path is ending with '/' or '\'
	{
		filepath[cnt] = '\0';
	}
	else
	{
		filepath[cnt] = argv[index][cnt];
		filepath[cnt+1] = '\0';
	}

#ifdef _ENABLE_LOGS_INFO_
	printf("thread: [%ld] function: [%s] line: [%d] reading directory & files from path: [%s]\n", pthread_self(), __func__, __LINE__, filepath);
#endif
	listDir(filepath);

#ifdef _ENABLE_LOGS_INFO_RESULT_
	printf("\nthread: [%ld] function: [%s] line: [%d] total_word_counts: [%ld] total txt files: [%d] total other files: [%d] total nested directory counts: [%d]\n", pthread_self(), __func__, __LINE__, total_word_counts, total_num_txt_files,  total_num_other_files, total_dir_counts);
#endif
	time_t end_time = time(NULL);
	struct tm* end_watch_time;
	end_watch_time = localtime(&end_time);

#ifdef _ENABLE_LOGS_INFO_TIME_
	printf("\n\nthread: [%ld] function: [%s] line: [%d] Work Completed in seconds: [%.5f] Program End Time: [%02d:%02d:%02d]\n", pthread_self(), __func__, __LINE__,  
		difftime(end_time, start_time), 
		end_watch_time->tm_hour, end_watch_time->tm_min, end_watch_time->tm_sec );
#endif
	return 0;
}


int isTxtFile(char *filename)
{
	int len = strlen(filename);
	if(len <= 4)
		return 0;
	if(strcmp(filename+len-4, ".txt") == 0)
		return 1;
	else
		return 0;
}

void listDir(char *filepath)
{
#ifdef _ENABLE_LOGS_VERBOSE_
	printf("thread: [%ld] function: [%s] line: [%d]\n", pthread_self(), __func__, __LINE__);
#endif
	struct dirent *de;
	struct stat statbuf;
	int sz = PATH_MAX;
	char *origpath = (char*) malloc(sizeof(char) * sz);
#ifdef _ENABLE_LOGS_INFO_DIR_
	char *buf, *ptr;
	buf = (char*) malloc(sizeof(char) * sz);
	ptr = getcwd(buf, sz);
	printf("thread: [%ld] function: [%s] line: [%d] current working dir: [%s]\n", pthread_self(), __func__, __LINE__, buf);
	printf("thread: [%ld] function: [%s] line: [%d] current file path: [%s]\n", pthread_self(), __func__, __LINE__, filepath);
#endif
	DIR *dr = opendir(filepath);

	if(dr == NULL)
	{
#ifdef _ENABLE_LOGS_ERR_
		printf("thread: [%ld] function: [%s] line: [%d] can't open the directory: [%s]\n", pthread_self(), __func__, __LINE__, filepath);
#endif
		return;
	}

	while((de = readdir(dr)) != NULL)
	{
		if(strcmp(".", de->d_name) == 0 ||
				strcmp("..", de->d_name) == 0)
		{
#ifdef _ENABLE_LOGS_INFO_DIR_
			printf("thread: [%ld] function: [%s] line: [%d] DIR: [%s]\n", pthread_self(), __func__, __LINE__, de->d_name);
#endif
			continue;
		}
		strcpy(origpath, filepath);
#ifdef _ENABLE_LOGS_INFO_DIR_
			printf("thread: [%ld] function: [%s] line: [%d] orig path: [%s]\n", pthread_self(), __func__, __LINE__, origpath);
#endif
		strcat(filepath, "/");
		strcat(filepath,de->d_name);
#ifdef _ENABLE_LOGS_INFO_DIR_
			printf("thread: [%ld] function: [%s] line: [%d] new file path: [%s]\n", pthread_self(), __func__, __LINE__, filepath);
#endif
		lstat(filepath, &statbuf);
		if((statbuf.st_mode & S_IFMT) == S_IFDIR)
		{
			total_dir_counts++;
#ifdef _ENABLE_LOGS_INFO_DIR_
			printf("thread: [%ld] function: [%s] line: [%d] DIR: [%s], statbuf.st_mode: [%X]\n", pthread_self(), __func__, __LINE__, de->d_name, statbuf.st_mode & S_IFMT);
			printf("thread: [%ld] function: [%s] line: [%d] Go to dir and list files/dirs: [%s]\n", pthread_self(), __func__, __LINE__, filepath);
#endif
#ifdef _ENABLE_LOGS_INFO_
			printf("thread: [%ld] function: [%s] line: [%d] DIRECTORY: [%s] total directory counts so far: [%d]\n", pthread_self(), __func__, __LINE__, filepath, total_dir_counts);
#endif
			listDir(filepath);
		}
		else
		{
#ifdef _ENABLE_LOGS_INFO_DIR_
			printf("thread: [%ld] function: [%s] line: [%d] FILE: [%s] PATH: [%s] statbuf.st_mode: [%X]\n", pthread_self(), __func__, __LINE__, de->d_name, filepath, statbuf.st_mode & S_IFMT);
#endif
			if(isTxtFile(filepath))
			{
#ifdef _ENABLE_LOGS_INFO_
				printf("thread: [%ld] function: [%s] line: [%d] TXT FILE: [%s]\n", pthread_self(), __func__, __LINE__, filepath);
#endif
				long int countwords = 0;
				if(print == 1)
					countwords = read_file(filepath);
				total_num_txt_files++;
				total_word_counts += countwords;
#ifdef _ENABLE_LOGS_INFO_
				printf("thread: [%ld] function: [%s] line: [%d] file read: [%s] total word count of the file: [%ld]\n", pthread_self(), __func__, __LINE__, filepath, countwords);				
				printf("thread: [%ld] function: [%s] line: [%d] total txt files so far [%d] total words read so far from all the files: [%ld]\n", pthread_self(), __func__, __LINE__, total_num_txt_files, total_word_counts);
#endif
			}	
			else
			{
#ifdef _ENABLE_LOGS_INFO_
				printf("thread: [%ld] function: [%s] line: [%d] OTHER FILE: [%s]\n", pthread_self(), __func__, __LINE__, filepath);
#endif
				total_num_other_files++;
#ifdef _ENABLE_LOGS_INFO_
				printf("thread: [%ld] function: [%s] line: [%d] total other files so far: [%d]\n", pthread_self(), __func__, __LINE__, total_num_other_files);	
#endif		
			}
		}
		strcpy(filepath, origpath);
	}
#ifdef _ENABLE_LOGS_INFO_DIR_
	free(buf);
#endif
	free(origpath);
	closedir(dr);
#ifdef _ENABLE_LOGS_VERBOSE_
	printf("thread: [%ld] function: [%s] line: [%d]\n", pthread_self(), __func__, __LINE__);
#endif
	return;
}


long int read_file(char *file)
{
#ifdef _ENABLE_LOGS_VERBOSE_
	printf("thread: [%ld] function: [%s] line: [%d]\n", pthread_self(), __func__, __LINE__);
#endif
	FILE* fp;
	long int countwords = 0;

	if(file == NULL)
		return 0;
#ifdef _ENABLE_LOGS_INFO_
	printf("thread: [%ld] function: [%s] line: [%d] reading file: [%s] started\n", pthread_self(), __func__, __LINE__, file);
#endif
	fp = fopen(file, "r");
	if(fp == NULL)
	{
#ifdef _ENABLE_LOGS_ERR_
		printf("thread: [%ld] function: [%s] line: [%d] file: [%s] does not exist\n", pthread_self(), __func__, __LINE__, file);
#endif
		return 0;
	}
	char buf[PATH_MAX];

#ifdef _ENABLE_LOGS_VERBOSE_
		printf("thread: [%ld] function: [%s] line: [%d] contents of file [%s] =>\n\n\n", pthread_self(), __func__, __LINE__, file);
#endif
	while(fscanf(fp, "%s", buf) != EOF)
	{
#ifdef _ENABLE_LOGS_VERBOSE_
		printf("[%s] ", buf);
#endif
		removeNonAlphaNumFromStr(buf);
		if(buf[0] != '\0')
			countwords++;
			
	}
#ifdef _ENABLE_LOGS_VERBOSE_
	printf("\n\n\n");
#endif

	fclose(fp);
#ifdef _ENABLE_LOGS_INFO_RESULT_INTERNAL_
	printf("thread: [%ld] function: [%s] line: [%d] reading file: [%s] completed & total word count of the file: [%ld]\n", pthread_self(), __func__, __LINE__, file, countwords);
#endif
#ifdef _ENABLE_LOGS_VERBOSE_
	printf("thread: [%ld] function: [%s] line: [%d]\n", pthread_self(), __func__, __LINE__);
#endif
	return countwords;
}

void removeNonAlphaNumFromStr(char* buf)
{
	int len = strlen(buf);
	char temp[PATH_MAX] = {0};
	int temp_count = 0;
	int count;
#ifdef _ENABLE_LOGS_VERBOSE_2_
	printf("thread: [%ld] function: [%s] line: [%d] non-modified string is: [%s]\n", pthread_self(), __func__, __LINE__, buf);
#endif
	for(count=0; count<len; count++)
	{
		if(isalpha(buf[count]) || isdigit(buf[count]))
			temp[temp_count++] = buf[count];
	}
	temp[temp_count] = '\0';
	strcpy(buf, temp);
#ifdef _ENABLE_LOGS_VERBOSE_2_
	printf("thread: [%ld] function: [%s] line: [%d] modified string is: [%s]\n", pthread_self(), __func__, __LINE__, buf);
#endif
}




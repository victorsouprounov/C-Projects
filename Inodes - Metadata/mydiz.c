//---------------start of mydiz.c--------------------//
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <utime.h>
#include <unistd.h>

#include "mydiz.h" //functions definition

#define LEVEL 7
#pragma region Variables 

int di_done =0;
char *modes[] = { "---","--x","-w-","-wx","r--","r-x","rw-","rwx" };
char * afile;
char** array;

FILE *dip, *fp;

int exit_code = 0;              //program exit code
int aa = -1;
int files_bytes = 0;
int total_bytes = 0;
int num_elements = 0;
int num_allocated = 0;
int job_to_do = -1;
int length;
pid_t pid;
int q_aa;

/* ==== options ==== */                          /*CHECKED*/
int list       = 0;   // c  arxiothetisi            | X |
int data       = 0;   // a  egrafa                  | - |
int extract    = 0;   // x  extract                 | X |
int zip        = 0;   // j  zip                     | X |
int delete     = 0;   // d  delete  *proairetiko*   
int print_meta = 0;   // m  print metaplirofories   | X |
int if_exist   = 0;   // q  if all data in list     | X |    
int print_info = 0;   // p  print					| X |

struct sinfo ** SaveInfo = NULL;
infos Header; //Header
metadata *dictionary = NULL; //Dictionary with metadata

#pragma endregion   
	   
	   
	   
int main(int argc,char **argv)
{
	
    //=========================================================//
    //====================Getting arguments====================//
    //=========================================================//
	
	dictionary = malloc(1 * sizeof(struct metadata));
	
	int c = 0;
	int len1 = 0;
	int cnt =0;
	
    for(c = 1; c < argc; c++)
	{
        len1 = strlen(argv[c]);
        int found = 0;

        if ( aa==-1 && strncmp(argv[c], "-c",  len1) == 0)
		{ 
			job_to_do = 0;
			list = 1;
			cnt++;
			found++; 
		}
        if (aa==-1 && strncmp(argv[c], "-a",  len1) == 0)
		{ 
			job_to_do = 1;
			data = 1;
			found++;
			cnt++;
		}   
        if (aa==-1 && strncmp(argv[c], "-x",  len1) == 0)
		{
			cnt++;
			job_to_do = 2;
			extract = 1;
			found++;
		}
        if (aa==-1 && strncmp(argv[c], "-j",  len1) == 0)
		{ 
			job_to_do = 3;
			zip = 1;
			found++;
		}
        if (aa==-1 && strncmp(argv[c], "-d",  len1) == 0)
		{
            cnt++;			
			job_to_do = 4;
			delete = 1;
			found++; 
		}
        if (aa==-1 && strncmp(argv[c], "-m",  len1) == 0)
		{ 
			cnt++;
			job_to_do = 5;
			print_meta = 1; 
			found++; 
		}
        if (aa==-1 && strncmp(argv[c], "-q",  len1) == 0)
		{
			cnt++;
			job_to_do = 6;
			if_exist = 1; 
			found++; 
		}
        if (aa==-1 && strncmp(argv[c], "-p",  len1) == 0)
		{ 
			job_to_do = 7;
			print_info = 1; 
			found++;
			cnt++;
		}
	  
        if (found == 0 && exit_code == 0)
		{  
            aa++;    
            if (aa == 0)
			{ 
				afile = (char*)malloc(len1 * sizeof(char)); 
				strcpy(afile,argv[c]); 
			}
            if (aa >  0)
			{   
				if(list || data){
					if( 0 == access(argv[c], 0) || is_dir(argv[c])){
						struct stat temp;
						stat(argv[c], &temp);
						if ((temp.st_mode & S_IFMT) == S_IFDIR)
						{
							List(argv[c]);
							aa--;
						}     
						else
						{
							array = (char**)realloc(array, (aa + 1)*sizeof(char*)); 
							array[aa - 1] = (char *)malloc((len1 + 1) *  sizeof(char));
							strcpy(array[aa - 1],argv[c]); 
						}
					}
					else
					{
						exit_code = 7;
					}
				}
				else
				{
					array = (char**)realloc(array, (aa + 1)*sizeof(char*)); 
					array[aa - 1] = (char *)malloc((len1 + 1) *  sizeof(char));
					strcpy(array[aa - 1],argv[c]); 
									
				}
            }
        }
    }
	
	q_aa = aa; // setting aa for query
    //=========================================================//
    //====================Checking arguments===================//
    //=========================================================//
	if(cnt>1)exit_code =8;
	if(cnt==0)exit_code = 9;
	if (!exit_code && aa < 0)
	{
		exit_code = 2; // ena toylaxiston name
	}
	if(list && aa ==0)exit_code =2;

	//=========================================================//
	//==================== -j with -c =========================//
	//=========================================================//
    if (!exit_code && zip)
	{ 
        if (list || data)
		{
			do_the_job(job_to_do);
			do_free();       //free memory
			return exit_code;//exit successfuly
		} 
		else 
		{
			exit_code = 4; 
			do_usage(exit_code); 
		}            
    }
    
    //=========================================================//
	//==================== -c -q -m -p -x =====================//
	//=========================================================//
	if (exit_code == 0)
	{
		do_the_job(job_to_do);
	}
	else
	{
		do_usage(exit_code);
	}
        
    do_free();
    return exit_code; 
}

//===============================================================================//
//============================= FUNCTIONS =======================================//
//===============================================================================//

	
	
	
	//=========================================================//
	//====================  DO FREE  ==========================//
	//=========================================================//
void do_free(){
	
	if (afile != NULL)
	{
		free(afile);
	}

    if (array != NULL) 
	{ 
		int i = 0; 
		for (i = 0; i < aa; i++)
		{
			free(array[i]);
		}

     } 
}

    //=========================================================//
	//==================== Printing Errors ====================//
	//=========================================================//
void do_usage(int exitcode)
{
     //do_display_arg();
     printf("usage: mydiz {-c|-a|-x|-m|-d|-p|-j} <archive-file> <list-of-files/dirs>");
   //if (exitcode == 1) printf("\nerror: too many names");
     if (exitcode == 2) printf("\nerror: number of names must be 1 and more\n");
   //if (exitcode == 3) printf("\nerror: <archive-file> == <list-of-files/dirs>\n");
     if (exitcode == 4) printf("\nerror: use -j  with  -c  or -a\n");
     if (exitcode == 5) printf("\nerror: flags\n");
     if (exitcode == 6) printf("\nerror: realloc array\n");
	  if (exitcode == 7) printf("\nerror: file or dir not found\n");
	  if (exitcode == 8) printf("\nerror: too many flags\n");
	  if (exitcode == 9) printf("\nerror: not enough flags\n");
}
    //=========================================================//
	//==================== Printing Command ===================//
	//=========================================================//
int do_display_arg() {
    printf("mydiz ");
    if (list      ) printf("-c ");
    if (data      ) printf("-a ");
    if (extract   ) printf("-x ");
    if (zip       ) printf("-j ");
    if (delete    ) printf("-d ");
    if (print_meta) printf("-m ");
    if (if_exist  ) printf("-q ");
    if (print_info) printf("-p ");
    
    printf("%s ", afile);
    int i;
    for (i = 0; i < aa; i++) {
        printf("%s ", array[i]);
    }
     
    printf("\n");    
}


//=========================================================//
//==================== Execution  =========================//
//=========================================================//
int do_the_job(int parameter)
{
	
	//----------------------------------------//	
	//-------------[ -c && -j ]---------------//
	//---------------COMPRESSS----------------//
	if(zip){
		
		int p;
		int ret=0;
		int status;
		
		char msg[100];
		int i;
		
		for (i = 0; i < aa; i++)
		{
			pid_t parent = getpid();
			pid = fork();
			
			if (pid == -1)
			{
				//error
				error("error, failed to fork()");
				
			} 
			else if (pid > 0)
			{
				//parent
				int status;
				waitpid(pid, &status, 0);
		
			}
			else 
			{	
				//child
				if(!is_dir(array[i])){
					execlp("gzip" , "gzip" , array[i], NULL); //execute gzip with execlp
					if (ret < 0){
						exit(EXIT_FAILURE);
						return -1;
					}
				}
				return;
			}
		}
		
	}
	
	//----------------------------------------//	
	//---------------[ -c ]-------------------//
	//---------------LISTING------------------//
    if(list){	
		unsigned char buffer; //gia to diavasma binary kai grapsimo
		do_display_arg();
		FILE * dip;
		dip = fopen(strcat(afile, ".di"), "w+b"); //Open .di file
		di_done++;
		fwrite(&Header, sizeof(struct infos), 1, dip);   //Insert header
	       	
		int i;
		int wrecs;
		for (i = 0; i < aa; i++)
		{
			
			wrecs = 0;
			char buff[512];
			int rok = 1;
			FILE * fp;
			int savesize=0;
			int int_write=0;
			if(zip==1){
			if ((fp = fopen(strcat(array[i],".gz"), "rb")) == NULL)
			{
				
				rok = 0;
			}}
			else{
			if ((fp = fopen(array[i], "rb")) == NULL)
			{
				rok = 0;
			}
			}
			int a1;
			if(rok){
				SaveInfo = (struct sinfo **)realloc(SaveInfo, (i+1)*sizeof(struct sinfo *));
				SaveInfo[i] = (struct sinfo *)malloc(sizeof(struct sinfo));
				SaveInfo[i]->si_start = files_bytes;
			}
			while (rok &&(1 == fread(&buffer,sizeof(buffer),1,fp)))
			{
				a1 =1;  //never goes here
				if(a1<1)//never goes here
				{
					break;
				}
				else
				{	
					fwrite(&buffer,1,sizeof(buffer),dip);
					int_write = 1;
					wrecs += int_write;
					savesize += int_write;
				}
	
			}
			if (rok){
				SaveInfo[i]->si_size = savesize;
				fclose(fp);}
	
			files_bytes += wrecs;
		}
	
		total_bytes += files_bytes + sizeof(struct infos);
	
		for (i = 0; i < aa; i++)
		{
			int rod = 1;
			if (is_dir(array[i])==1)rod=0;
			if(rod==1){
			SetMetadata(array[i]);
		
			//EGGRAFH METADATA STO ARXEIO .di
			stpcpy(dictionary->name,array[i]);
			dictionary->si_start  =    SaveInfo[i]->si_start;
			dictionary->si_size   =   SaveInfo[i]->si_size;        //to megethos pou tha kanoume save (oxi tou pragmatikou arxeioy)
			dictionary->si_zip    =  zip;        //an einai zip h' oxi	
			realpath(array[i],dictionary->si_path);
			fwrite(dictionary,sizeof(struct metadata),1,dip);
			}			
		}

		//Update Header	
		int int_fseek = fseek(dip, 0, SEEK_SET);
	
		strcpy(Header.fi_info, afile);
		Header.fi_size = files_bytes;
		Header.pointer = dictionary;
		fwrite(&Header, sizeof(struct infos), 1, dip);	
		
		fclose(dip);		
	}
	
	//----------------------------------------//	
	//---------------[ -x ]-------------------//
	//---------------EXCTRACT-----------------//
	
	if(extract){
	    
		FILE * dip;  
		dip = fopen(strcat(afile, ".di"), "rb"); //Open .di file
        
		unsigned char buffer;
		fread(&Header, sizeof(struct infos), 1, dip);	
		
		int blabla = 0;
		char bufff[1024];
		blabla = Header.fi_size + sizeof(struct infos);
		
		int int_fseek = fseek(dip, blabla, SEEK_SET);
		int a;
		
		while(1){
		   a = fread(dictionary, sizeof(struct metadata), 1 ,dip);
	       if (a<= 0)break;
		   else{
                 create_dir(dictionary->name);
				 
				 FILE * dip1;
		         dip1 = fopen(dictionary->name, "w+b");  
               
                 unsigned long position = ftell(dip);
                 int istart = dictionary->si_start  + sizeof(struct infos);
                 int itot   = (int) dictionary->infos.st_size;
                 int int_fseek1 = fseek(dip, istart, SEEK_SET);
                 printf("\n%s %d \%d\n", dictionary->name, istart, itot);
                 char buff[512]; int iread = 0; int a1; int i; char c;
                 
				 while ((1==fread(&buffer,sizeof(buffer),1,dip)) && iread < itot) {
                     a1 = 1;
					 fwrite(&buffer,1,sizeof(buffer),dip1);
                     iread++;             
                 }
                 
				 fclose(dip1);
				 
				 if(dictionary->si_zip==1){	
					sprintf(bufff,"gzip -d %s",dictionary->name);
					system(bufff);
				 }
				 
				 struct utimbuf buf;                                    //setting utimbuf
				 buf.actime =dictionary->infos.st_atime;                //changing access and mod time
				 buf.modtime =dictionary->infos.st_mtime;               //						  ^
				                                                        //changing access rights  |
				 chmod(dictionary->si_path, dictionary->infos.st_mode); //--^                     |
				 utime(dictionary->si_path, &buf);                      //-------------------------
				 
                 fseek(dip,position,SEEK_SET);
				}
		}
        fclose(dip);
	}
	
	//-----------------------------------------// 
	//-----------------[ -m ]------------------//   
	//-------------PRINTING METADATA-----------//
	
	if(print_meta){
		FILE * dip;
		dip = fopen(strcat(afile, ".di"), "rb"); //Open .di file
		fread(&Header, sizeof(struct infos), 1, dip);	
		//printf("header size: %d \nheader point start: %d\n" ,Header.fi_size , Header.fi_point_start);
		int blabla = 0;
		blabla = Header.fi_size + sizeof(struct infos);
		//printf("blabla size: %d\n", blabla);
		int int_fseek = fseek(dip, blabla, SEEK_SET);
		int a;
		while(1){
		a = fread(dictionary, sizeof(struct metadata), 1 ,dip);
		if (a<= 0)break;
		else{
			     printf("---------------------------\n");
				 printf("PointerStarts : \t\t\t%d\n" , dictionary->si_start);
				 printf("Size of savedinfo : \t\t\t%d\n" , dictionary->si_size);
				 printf("Name file: \t\t\t\t%s\n", dictionary->name);
				 printf("File Zip: \t\t\t\t%d\n", dictionary->si_zip);
				 printf("ID of device containing file: \t\t%d\n",(int) dictionary->infos.st_dev);
				 printf("File inode: \t\t\t\t%d\n", (int) dictionary->infos.st_ino);
				 printf("Number of Links: \t\t\t%d\n", (int) dictionary->infos.st_nlink);
				 printf("User ID of owner: \t\t\t%d\n", (int) dictionary->infos.st_uid);
				 printf("Group ID of owner: \t\t\t%d\n",(int)  dictionary->infos.st_gid);
				 printf("File Size: \t\t\t\t%d bytes\n", (int) dictionary->infos.st_size);
				 printf("Blocksize for file system I/O: \t\t%d\n", (int) dictionary->infos.st_blksize);
				 printf("Number of 512B blocks allocated: \t%d\n", (int) dictionary->infos.st_blocks);
				 printf("File Permissions: \t");
				 printf( ( dictionary->infos.st_mode & S_IRUSR) ? "r" : "-");
				 printf( ( dictionary->infos.st_mode & S_IWUSR) ? "w" : "-");
				 printf( ( dictionary->infos.st_mode & S_IXUSR) ? "x" : "-");
				 printf( ( dictionary->infos.st_mode & S_IRGRP) ? "r" : "-");
				 printf( ( dictionary->infos.st_mode & S_IWGRP) ? "w" : "-");
				 printf( ( dictionary->infos.st_mode & S_IXGRP) ? "x" : "-");
				 printf( ( dictionary->infos.st_mode & S_IROTH) ? "r" : "-");
				 printf( ( dictionary->infos.st_mode & S_IWOTH) ? "w" : "-");
				 printf( ( dictionary->infos.st_mode & S_IXOTH) ? "x" : "-");
			     printf("---------------------------\n");
				 printf("\n\n");
				}
		}
		fclose(dip);
	}
	
	//-------------------------------------------//
	//------------------[ -q ]-------------------//  
	//------------------QUERY--------------------//  
	
	if(if_exist){
		
		FILE * dip;
		dip = fopen(strcat(afile, ".di"), "rb"); //Open .di file
		fread(&Header, sizeof(struct infos), 1, dip);	

		int blabla = 0;
		blabla = Header.fi_size + sizeof(struct infos);
		
		int int_fseek = fseek(dip, blabla, SEEK_SET);
		int a;
		int i =0;
		int ff=0;
		
		for(i=0; i<q_aa; i++){
			ff =0;
            int_fseek = fseek(dip, blabla, SEEK_SET);
			
			while(1){
				a = fread(dictionary, sizeof(struct metadata), 1 ,dip);
				
				if (a <= 0)break;
				if(!strncmp(dictionary->name,array[i],strlen(array[i])) &&  (strlen(array[i]) == strlen(dictionary->name))){ff++;}
				
				char kati[1024];
				sprintf(kati,"%s.gz",array[i]);
				
				if(!strcmp(dictionary->name,kati)){ff++;}
			}
			if(ff==0)printf("this %s doesnt exists\n",array[i]);
			else printf("this %s exists\n",array[i]);		
		}
		fclose(dip);
	}
	
	//-------------------------------------------// 
	//------------------[ -a ]-------------------// 
	//----------------ADD DATA ------------------//
	if(data){
		
		char bfile[100]="";
		sprintf(bfile,"%s.di1",afile);
		unsigned char buffer; //gia to diavasma binary kai grapsimo
		do_display_arg();
		FILE * dip;
		dip = fopen(bfile, "w+b"); //Open .di1 file
		di_done++;
		fwrite(&Header, sizeof(struct infos), 1, dip);   //Insert header
	
		//---------------------
		//CODE HERE FOR -a ...
		//---------------------
				
		fclose(dip);
				
	}	
	
	//-------------------------------------------// 
	//------------------[ -p ]-------------------// 
	//-------------------PRINT-------------------//
	if(parameter == 7){
		Print();
		printf("\n");
	}

	//----------------------------------//
	//------------[ -j ]----------------//
	//-------UNZIP original files-------//
 	if(zip){
		//int p,pid[p];
		printf("[used gzip compression]\n");
		int p;
		int ret=0;
		int status;
		
		char msg[100];
		int i;
		
		for (i = 0; i < aa; i++)
		{
			pid_t parent = getpid();
			pid = fork();
			
			if (pid == -1)
			{
				error("error, failed to fork()");
	
			} 
			else if (pid > 0)
			{
				//printf("PARRENT CORRECT\n");
				int status;
				waitpid(pid, &status, 0);
		
			}
			else 
			{
				if(!is_dir(array[i])){
				execlp("gzip" , "gzip", "-d" , array[i], NULL);
				//execlp("gzip", strcat("gzip",strcat(array[aa-1],strcat(" > ",strcat(array[aa-1],".gz")))), NULL);	
				if (ret < 0){exit(EXIT_FAILURE);return -1;}
				//printf("...compressed\n");
				}
			}
		}
	}

}


//=========================================================//
//==================== Set Metadata Function ==============//
//=========================================================//
 void SetMetadata(char *name)
 {
	 if (stat(name, &dictionary->infos) < 0)
	 {
		 printf("Error reading metadata.\n");
	 }
}

 
//=========================================================//
//==================== Listing Function -c ================//
//=========================================================//
int List(char *name)
{
	char * name1 = name;
	DIR *dir = opendir(name);
	struct dirent *ent;
	if(!is_dir(name1))dir = NULL;
	while (dir!= NULL &&(ent = readdir(dir)))
	{
		char *entry_name = ent->d_name;
		
		
		if (is_dir(name1))
		{
			if((strcmp(ent->d_name, ".") ==0 || strcmp(ent->d_name, "..") == 0)){
			
			continue;}
			char path[1024];
			
			length = snprintf(path,sizeof(path)-1,"%s/%s",name,entry_name);
			path[length]=0;
			
			array = (char**)realloc(array, (aa + 1) * sizeof(char*));
			array[aa - 1] = (char *)malloc((length + 1) * sizeof(char));
			
			strcpy(array[aa - 1], path);
			aa++;
			
		}

		if (is_directory_we_want_to_list(name, entry_name))
		{
			char *next = malloc(strlen(name) + strlen(entry_name) + 2);
			sprintf(next, "%s/%s", name, entry_name);
			List(next);
			free(next);
		}
	}
	closedir(dir);
}
 
//===========================================================================//
//=====  =====  ====  =====    PRINT    ==== ====  ====  ====  ====  ==== ===//
//===========================================================================//
void Print() {
    int j;
    char path1[1024] = "";
    char path2[1024] = "";
    for (j = 0; j < 1024; j++) path1[j] = ' '; 
    for (j = 0; j < 1024; j++) path2[j] = ' '; 
    FILE * dip;
	dip = fopen(strcat(afile, ".di"), "rb"); //Open .di file
	fread(&Header, sizeof(struct infos), 1, dip);
	int blabla = 0;
	blabla = Header.fi_size + sizeof(struct infos);
	int int_fseek = fseek(dip, blabla, SEEK_SET);
	int a, i, lvl, s, l, maxl, minl, n1, n2;
	int k = 0;
    int cnt = 0; int lev;  
	while (1) {
        cnt++;
		a = fread(dictionary, sizeof(struct metadata), 1, dip);
		if (a <= 0)
			break;
		else {
            //printf("Dictionary path is %s\n", dictionary->si_path);
            for (i = 0; i < 1000; i++) path1[i] = ' ';  
            for (i = 0; i < strlen(dictionary->si_path) && i < 1000; i++) {
               path1[i] = dictionary->si_path[i]; 
            }
            
            s = 0; maxl = 0; minl = 0; lev = 0;
            for (i = 0; i < 1000; i++) { 
                if (path1[i] == '/')  { maxl = i; }
                if (path1[i] == '/' && minl == 0) minl = i;
            }
                
            for (i = 0; i < 1000; i++)  {
                if (path1[i] != path2[i]) { 
                    s = i; 
                    if (i < minl && cnt > 1) { 
                        for (n1 = 0; n1 < minl && n1 < s; n1++) {
                            path1[n1] = dictionary->si_path[n1];
                        }
                    }
                    if (i > minl && i < maxl && cnt > 1) {
                        int do1 = 1;
                        for (n1 = s; do1 && n1 > minl && n1 < maxl; n1--) {
                            if (path2[n1] == '/') do1 = 0;
                            else path1[n1] = dictionary->si_path[n1];
                        }
                    }
                }
                if (path1[i] == path2[i] && s ==0 && i < maxl) path1[i] = ' ';
            }         
                
            printf("\n");
            lev = 0;  
            for (i = 0; i < strlen(dictionary->si_path) && i < 1000; i++) {
                if (path1[i] != '/') {printf("%c",path1[i]);}
                if (path1[i] == '/') {
                    lev++;
                   // int lev1 = (lev+1) * 5;
					printf("\n"); 
                    for (k=0; k < i; k++) {
                        printf(" ");
                    }
                }
            }      
            
            for (i = 0; i < 1000; i++) path2[i] = ' ';             
            for (i = 0; i < strlen(dictionary->si_path) && i < 1000; i++) {
               path2[i] = dictionary->si_path[i]; 
            }
        }
    }
}
//===========================================================================//
//======================END OF PRINT=========================================//
//===========================================================================//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -//
//=========================================================//
//==================== Check this Dir =====================//
//=========================================================//

int is_directory_we_want_to_list(const char *parent, char *name)
{
	struct stat st_buf;
	if (!strcmp(".", name) || !strcmp("..", name)) //den theloume ta .  kai ta .. path
		return 0;
	char *path = alloca(strlen(name) + strlen(parent) + 2);
	sprintf(path, "%s/%s", parent, name);
	stat(path, &st_buf);
	return S_ISDIR(st_buf.st_mode); //an einai h' oxi dir
}

//=========================================================//
//==================== Check is Dir  ======================//
//=========================================================//

int is_dir(char * name){
	int iret = 0;
	struct stat temp;
	stat(name, &temp);
	if((temp.st_mode & S_IFMT) == S_IFDIR)  //an einai dir h' oxi
		iret = 1;
	return iret;
	
}


//=========================================================//
//==================== Create Directory  ==================//
//=========================================================//

void create_dir(char * path){
	char n[1000] = "";
	int i;
	struct stat st;
	for(i = 0; i< (strlen(path)-1)&&i<1000; i++){
		if(path[i] == '/'){	
			if(stat(n ,&st)== -1){
				
				mkdir(n , 0700);

			}
		}
		n[i] = path[i];
	}
}
//----------------END OF mydiz.c ------------------------//
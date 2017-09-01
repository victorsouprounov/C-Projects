#define MAXPATH 1000
typedef struct Output
{
 int level;
 char filefolder1[200];
 char filename1[100];

}Output;

typedef struct Combo
{
 char filepath[MAXPATH];
 int lvl;
}Combo;
 
typedef struct metadata
{	
	int si_start;
	int si_size;        //to megethos pou tha kanoume save (oxi tou pragmatikou arxeioy)
	int si_zip;         //an einai zip h' oxi	
	char si_path[MAXPATH];
	char name[100];
	int file_start;
	int file_size;

	struct stat infos;  //informations
}metadata;


struct sinfo{
	int si_start;
	int si_size;        //to megethos pou tha kanoume save (oxi tou pragmatikou arxeioy)
	int si_zip;         //an einai zip h' oxi
	char * si_path;     //path of saved file
};

typedef struct infos
{
	char  fi_info[100];
	int   fi_size;
	int   fi_point_start;
	metadata *pointer;
}infos;

typedef struct InodeAndName
{
	int inode;
	char name[100];
}InodeAndName;




void Search(InodeAndName *buf);
void do_free();
void do_usage(int exitcode);
int do_the_job();
int do_display_arg();
void SetMetadata(char *name);
void listdir(const char *name, int level);

void Print();
void Print1();
int is_directory_we_want_to_list(const char *parent, char *name);
int List(char *name);
int is_dir(char * name);
void create_dir(char * path);

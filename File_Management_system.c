#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#define len 10
char *path;
struct folder
{
	char name[len];
	struct folder **subfolders;
	struct file **subfiles;
	struct folder *parent;
	int folder_size;
	int file_size;
} *current;

struct file
{
	char name[len];
	struct folder *parent;
};

int ind;
struct folder * dir;
struct folder * cpy_fldr = NULL;
struct file * cpy_file = NULL;
int can_copy=1;


int search_folder(char nm[len], struct folder * fol){
	int sz = fol->folder_size;
	struct folder **list = fol->subfolders;
	for(int i = 0; i < sz; i++){
		if(strcmp(nm, list[i]->name) == 0){
			return i;
		}
	}
	return -1;
}


int search_file(char nm[len], struct folder * current){
    int sz = current->file_size;
	struct file **list = current->subfiles;
	for(int i = 0; i < sz; i++){
		if(strcmp(nm, list[i]->name) == 0)
			return i;
	}
	return -1;
}


void setfolder(struct folder* tmp,struct folder* pr,char nm[len])
{
	tmp->subfolders=NULL;
	tmp->subfiles=NULL;
	strcpy(tmp->name,nm);
	tmp->folder_size=0;
	tmp->file_size=0;
	tmp->parent=pr;
}

void setfile(struct file* tmp,struct folder* pr,char nm[len])
{
	tmp->parent=pr;
	strcpy(tmp->name,nm);
}

struct folder* addfolder(char nm[len], struct folder *current)
{
	struct folder *tmp = (struct folder *) malloc(sizeof(struct folder));
	setfolder(tmp,current,nm);
	int size = ++(current->folder_size);
	struct folder **list = (current->subfolders);
	list = (struct folder **) realloc(list, size*sizeof(struct folder*));
	*(list + size - 1) = tmp;
	current->subfolders = list;
	return tmp;
}

void addfile(char nm[len],struct folder *current)
{
	if(search_file(nm, current) != -1){
		printf("File already Exists!\n");
		sleep(2);
		return;
	}
	struct file* tmp=(struct file*)malloc(sizeof(struct file));
	setfile(tmp,current,nm);
	int size= ++(current->file_size);
	struct file **list=(current->subfiles);
	list = (struct file **) realloc(list, size*sizeof(struct file*));
	*(list + size - 1) = tmp;
	current->subfiles = list;
}

void paste_file(struct folder * current, struct file * cpy){
	addfile(cpy->name, current);
}

void paste_folder(struct folder * current, struct folder * cpy){
	struct folder ** folders = cpy->subfolders;
	struct file ** files = cpy->subfiles;
	int foldersize=cpy->folder_size;
	int filesize=cpy->file_size;
	current = addfolder(cpy->name, current);
    for(int i = 0; i < filesize; i++){
		paste_file(current, files[i]);
	}
	for(int i = 0; i < foldersize; i++){
		paste_folder(current, folders[i]);
	}
}

void delete_folder(struct folder * current){
	if(cpy_fldr == current)
		cpy_fldr = NULL;
	struct folder ** folders = current->subfolders;
	struct file ** files = current->subfiles;
	
	for(int i = 0; i < current->file_size; i++){
		free(files[i]);
	}
	for(int i = 0; i < current->folder_size; i++){
		delete_folder(folders[i]);
	}
	free(current);
}

void print(struct folder * curr){

	int foldersize = current->folder_size;
	int filesize = current->file_size;
	struct folder **folderlist = (current->subfolders);
	struct file **filelist = (current->subfiles);
	printf("current: %s\n\n", path);

    int cnt = 0;
	if(foldersize != 0){
		printf("--------------[Folders List]--------------\n ");
	}
		while(cnt < foldersize){
			if(cnt%4 == 0 && cnt != 0) 
				printf("\n ");
			printf("%-10s ", folderlist[cnt++]->name);
		}
	if(filesize != 0){
		printf("\n\n---------------[Files List]---------------\n ");
	}
		cnt = 0;
		while(cnt < filesize){
			if(cnt%4 == 0 && cnt != 0) 
				printf("\n ");
			printf("%-10s ", filelist[cnt++]->name);
		}
	printf("\n\n");
}

int main()
{
	char tmp[5] = "Home";
	int in,exit=0, x, lenp = 4;
	path = (char *) malloc(sizeof(char)*5);
	for(int i = 0; i < 4; i++)
		path[i] = tmp[i];
	path[lenp] = '\0';
	char s[len];
	struct folder *home = (struct folder *)malloc(sizeof(struct folder));
	setfolder(home,NULL,tmp);
	current = home;

	do
	{
		system("cls");
		print(current);
		printf("[1] Add folder         ");
		printf("[2] Add file           ");
		printf("[3] Enter to Folder    ");
		printf("[4] Back \n");
		printf("[5] Delete Folder      ");
		printf("[6] Delete file        ");
		printf("[7] Copy File          ");
		printf("[8] Copy Folder        \n");
		printf("[9] Paste              ");
		printf("[0] exit \n");
		printf("Enter your choice : ");
		scanf("%d", &x);

		switch (x)
		{
		case 1:
			printf("Enter name of folder : ");
			scanf("%s", tmp);
			if(search_folder(tmp, current) != -1){
				printf("Folder already Exists!\n");
				sleep(2);
			}
			else{
				addfolder(tmp, current);
			}
			break;

		case 2:
		    printf("Enter name of file : ");
			scanf("%s", tmp);
			addfile(tmp, current);
			break;

		case 3:
			printf("Enter name of folder : ");
			scanf("%s", s);
			ind = search_folder(s, current);
			if(ind == -1){
				printf("Folder not found!");
				sleep(2);
			}else{
				path =(char *) realloc(path, (strlen(path)+2+strlen(s))*sizeof(char));
				path[lenp]='/';
				path[lenp+1] = '\0';
				strcat(path, s);
				lenp += strlen(s) + 1;
				current = current->subfolders[ind];
				if(cpy_fldr!=NULL && current==cpy_fldr)can_copy=0;
			}
			break;

		case 4:
			if (current == home){
				printf("Already in Root directory!\n");
				sleep(2);
			}
			else{
				lenp -= (strlen(current->name) + 1);
				path =(char *) realloc(path, (strlen(path)-1-strlen(current->name))*sizeof(char));
				path[lenp] = '\0';
				if( cpy_fldr!=NULL && current==cpy_fldr)can_copy=1;
				current = current->parent;
			}
			break;

		case 5:
			printf("Enter name of folder : ");
			scanf("%s", s);
			ind = search_folder(s, current);
			if(ind == -1){
				printf("Folder Not Found!");
				sleep(2);
			}
			else{
				struct folder** list = current->subfolders;
				delete_folder(list[ind]);
				list[ind] = list[(--current->folder_size)];
				list = (struct folder**) realloc(list, current->folder_size*(sizeof(struct folder*)));
				current->subfolders = list;
			}
			break;

		case 6:
			printf("Enter name of file : ");
			scanf("%s", s);
			ind = search_file(s, current);
			if(ind == -1){
				printf("File Not Found!");
				sleep(2);
			}else{
				struct file ** list = current->subfiles;
				struct file* del=list[ind];
				if(cpy_file == del)
					cpy_file = NULL;
				list[ind] = list[(--current->file_size)];
				list = (struct file **) realloc(list, current->file_size*sizeof(struct file*));
				current->subfiles = list;
				free(del);
			}
			break;

		case 7:
			printf("Enter name of file: ");
			scanf("%s", s);
			ind = search_file(s, current);
			if(ind == -1){
				printf("File not Found! \n");
				sleep(2);
			}else{
				cpy_file = current->subfiles[ind];
				cpy_fldr = NULL;
			}
			break;

		case 8:
			printf("Enter name of folder: ");
			scanf("%s", s);
			ind = search_folder(s, current);
			if(ind == -1){
				printf("Folder not Found! ");
				sleep(2);
			}
			else{
				cpy_fldr = current->subfolders[ind];
				cpy_file = NULL;
			}
			break;

		case 9:
			if(cpy_fldr == NULL && cpy_file == NULL){
				printf("Clipboard is Empty\n");
				sleep(2);
			}else if(can_copy==0){
				printf("You are in Source folder");
				sleep(2);
			}
			else if(cpy_file != NULL){
				ind = search_file(cpy_file->name, current);
				if(ind != -1){
					printf("File already Exists");
					sleep(2);
				}
				else{
					paste_file(current, cpy_file);
				}
			}else{
				ind = search_folder(s, current);
				if(ind != -1){
					printf("Folder already Exists");
					sleep(2);
				}
				else{
					paste_folder(current, cpy_fldr);
				}
			}
			break;

		default:	
		    printf("Enter valid move");
			sleep(2);		
			break;
		}
	}while(x);
}
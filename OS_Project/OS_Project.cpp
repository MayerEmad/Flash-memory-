//************ REFENCES************//
/*
https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fopen-wfopen?view=vs-2019
https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fseek-fseeki64?view=vs-2019
https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/ftell-ftelli64?view=vs-2019
https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/rewind?view=vs-2019
https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fread?view=vs-2019
https://www.programiz.com/c-programming/c-strings
https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/scanf-s-scanf-s-l-wscanf-s-wscanf-s-l?view=vs-2019
https://www.codeproject.com/Questions/518040/howplusiplususeplustheplusnewplusfunctionplusfopen
https://stackoverflow.com/questions/32283650/gets-function-is-not-available-in-visual-studio-2015-community
http://forums.codeguru.com/showthread.php?181020-getting-a-whole-line-with-scanf()
https://www.pixelstech.net/article/1330161976-Make-a-directory-using-C%2B%2B-for-windows
https://www.geeksforgeeks.org/c-program-to-create-a-file/
https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fwrite?view=vs-2019
https://en.cppreference.com/w/cpp/filesystem/directory_iterator
https://stackoverflow.com/questions/45401822/how-to-convert-filesystem-path-to-string?rq=1


*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include"stdafx.h"
#include<iostream>
#include<string>
#include<fstream>
#include<stdio.h>
#undef UNICODE
#include <windows.h>
#include <tchar.h>
#include <psapi.h>
#include <conio.h>
# include "process.h"
# include "malloc.h"
#include <filesystem>
#include <experimental/filesystem>
#include <filesystem>
#include<queue>
#include<amp_graphics.h>
using namespace std;
long long THESIZE = 1024 * 1024 * 100;

struct file
{
	char* data;
	string name;
	string type;
	int size;
};
struct folder
{
	string name;
	int filecnt;
	int foldercnt;
	folder* next[200];
	folder* prev;
	file* f = new file[100];
};
struct parentpath
{
	folder* parent;
	string path;
};

bool cpy = false , cpyf = false;
folder* copyy = new folder;
folder* cur ;
folder *root = new folder;
folder* filecopy = new folder;
queue<parentpath> q;

string isdup(string n)
{
	string name = n;
	bool ok;
	do
	{
		ok = false;
		for (int i = 0; i < cur->foldercnt; i++)
		{
			if (name == cur->next[i]->name) ok = true;
		}
		if (ok)
		{
			cout << "Dublicated name choose another one\n";
			cin >> name;
		}
	} while (ok);

	return name;
}

void readfile()
{
	////***** READING FILE *****/////	
	string path;
	printf_s( "enter the file's path\n");	
	cin.ignore();
	getline(cin, path);
	
	const char* pathh = path.c_str();
	FILE *source;                                     // return pointer
	errno_t errorCode = fopen_s(&source, pathh, "rb"); // file name & Kind of access  //DOCS 
	if (source == NULL)
	{
		cout << "Can't Open File.\n"; return;
	}
	fseek(source, 0, SEEK_END);                     // Stack overflow
	int size_data = ftell(source);
	rewind(source);                                 // return to the fils start   //DOCS

	char *data = new char[size_data];
	fread(data, 1, size_data, source);              // will save data on buffer // DOCS
													////*****    SEE IF THERE IS SPACE    *****///// 
	if (THESIZE - size_data < 0)
	{
		cout << "NO Free Space\n";
		return;
	}
	else THESIZE -= size_data;

	////*****    GETTING NAME    *****///// 
	int v = 0;
	string s = path;
	string name = "", type = "";
	for (int i = s.size() - 1; i >= 0; i--)
	{
		if (s[i] == '.')
		{
			v++; continue;
		}
		else if (s[i] == '\\')
		{
			v++; continue;
		}
		if (v == 0)
		{
			type = s[i] + type;
		}
		else if (v == 1)
		{
			name = s[i] + name;
		}
		else break;
	}

	////***** ADDING FILE *****///// 
	cur->f[cur->filecnt].name = name;
	cur->f[cur->filecnt].type = type;
	cur->f[cur->filecnt].size = size_data;
	cur->f[cur->filecnt].data = data;
	//puts(cur->f[cur->filecnt].data);
	cur->filecnt++;

	cout << "File " << name << " is created\n";
}
void addfolder()
{
	string name;
	cout << "Enter the folder's name ";
	cin >> name;
	// check if not unique name 
	name=isdup(name);
	
	//createfolder
	folder* x = new folder;
	x->name = name;
	x->prev = cur;
	x->filecnt = 0;
	x->foldercnt = 0;
	cur->next[cur->foldercnt] = x;
	cur->foldercnt++;
	
	cout << "Folder "<<name<<" is created\n";
}

long long dfs2(folder* node)
{
	long long sz = 0;
	for (int i = 0; i < node->foldercnt; i++)
	{
		sz+=dfs2(node->next[i]);
	}

	// create children files
	for (int i = 0; i < node->filecnt; i++)
	{
		sz += node->f[i].size;
	}
	return sz;
}
long long getfoldersize(folder* node)
{
	long long sz = dfs2(node);
	return sz;
}

void copyfolder()
{
	cout << "Inside " << cur->name << " there is " << cur->foldercnt << " FOLDERS and " << cur->filecnt << " FILES\n";
	for (int i = 0; i < cur->foldercnt; i++)
	{
		cout << "Folder :" << cur->next[i]->name << " ID = "<<i<< "\n";
	}
	
	/////***** CHOOSING FOLDER'S ID TO DEL IT ******/////
	cout << "choose the Id of the folder you want to Copy\n";
	int id, v;
	cin >> id;
	do {
		v = 0;
		if (id < 0 || id >= cur->foldercnt) { cout << "Invalid Id , try again \n"; v = 1; cin >> id; }
	} while (v == 1);
    
	////***KEEP or BACK ****/////
	cout << "press 0 to back and 1 to copy the folder you choosed\n";
	int x;  cin >> x;
	if (x == 0)
	{
		cout << "No copy"; return;
	}

	////*** COPYING ****/////
	cpy = true;
	copyy->f = cur->next[id]->f;
	copyy->filecnt = cur->next[id]->filecnt;
	copyy->foldercnt = cur->next[id]->foldercnt;
	copyy->name = cur->next[id]->name;
	copyy->prev = cur->next[id]->prev;
	for (int i = 0; i < copyy->foldercnt; i++)
	{
		copyy->next[i] = cur->next[id]->next[i];
	}
	cout << "Copying " << copyy->name << " is done\n";
}
void cutfolder()
{
	cout << "Inside " << cur->name << " there is " << cur->foldercnt << " FOLDERS and " << cur->filecnt << " FILES\n";
	for (int i = 0; i < cur->foldercnt; i++)
	{
		cout << "Folder :" << cur->next[i]->name << " ID = " << i << "\n";
	}

	/////***** CHOOSING FOLDER'S ID TO DEL IT ******/////
	cout << "choose the Id of the folder you want to Cut\n";
	int id, v;
	cin >> id;
	do {
		v = 0;
		if (id < 0 || id >= cur->foldercnt) { cout << "Invalid Id , try again \n"; v = 1; cin >> id; }
	} while (v == 1);

	////***KEEP or BACK ****/////
	cout << "press 0 to back and 1 to CUT the folder you choosed\n";
	int x;  cin >> x;
	if (x == 0)
	{
		cout << "No cutting"; return;
	}
	////*** COPYING ****/////
	cpy = true;
	copyy->f = cur->next[id]->f;
	copyy->filecnt = cur->next[id]->filecnt;
	copyy->foldercnt = cur->next[id]->foldercnt;
	copyy->name = cur->next[id]->name;
	copyy->prev = cur->next[id]->prev;
	for (int i = 0; i < copyy->foldercnt; i++)
	{
		copyy->next[i] = cur->next[id]->next[i];
	}
	long long sz = getfoldersize(cur);
	THESIZE += sz;
	////*** DELETING ***///
	cur->next[id] = cur->next[cur->foldercnt - 1];
	cur->foldercnt--;

	cout << "Cutting " << copyy->name << " is done\n";
}
void pastefolder()
{
	if (!cpy)
	{
		cout << "there is no copy to paste it\n"; return;
	}
	cout << "Do you want to paste this folder in " << cur->name << "? \n"
		 << "enter 1 for yes 0 for NO \n";
	int x; cin >> x;
	if (x == 0)
	{
		cout << "Ok , not here"; return;
	}
	long long sz = getfoldersize(copyy);
	if (THESIZE < sz)
	{
		cout << "Sorry there is no free space \n";
		return;
	}
	THESIZE -= sz;
	copyy->name=isdup(copyy->name);
	cout << "The copied folder name here is" << copyy->name << "\n";
	
	folder* node = new folder;
	node->f = copyy->f;
	node->filecnt = copyy->filecnt;
	node->foldercnt = copyy->foldercnt;
	node->name = copyy->name;
	for (int i = 0; i < copyy->foldercnt; i++)
	{
	   node->next[i] = copyy->next[i] ;
	}
	cur->next[cur->foldercnt] = node;
	cur->next[cur->foldercnt]->prev = cur;
	
	cout << cur->next[cur->foldercnt]->name << " is copied to " << cur->name << "\n";
	cur->foldercnt++;
}
void deletfolder()
{
	cout << "Inside " << cur->name << " there is " << cur->foldercnt << " FOLDERS and " << cur->filecnt << " FILES\n";
	for (int i = 0; i < cur->foldercnt; i++)
	{
		cout << "Folder :" << cur->next[i]->name << " ID = " << i << "\n";
	}
	/////***** CHOOSING FOLDER'S ID TO DEL IT ******/////
	cout << "choose the Id of the folder you want to delete\n";
	int id, v;
	cin >> id;
	do {
		v = 0;
		if (id < 0 || id >= cur->foldercnt) { cout << "Invalid Id"; v = 1;  cin >> id; }
	} while (v == 1);

	////***KEEP or BACK ****/////
	cout << "press 0 to back and 1 to delete the folder you choosed\n";
	int x;  cin >> x;
	if (x == 0)
	{
		cout << "the folder you choosed is still undeleted\n"; return;
	}
	long long sz = getfoldersize(cur);
	THESIZE += sz;
	////*** DELETING ***///
	cur->next[id] = cur->next[cur->foldercnt - 1];
	cur->foldercnt--;
	cout << "Delete Is Done\n";
}


void copyfile()
{
	cout << "Inside " << cur->name << " there is " << cur->foldercnt << " FOLDERS and " << cur->filecnt << " FILES\n";
	for (int i = 0; i < cur->filecnt; i++)
	{
		cout << "File :" << cur->f[i].name <<cur->f[i].type << " ID = " << i << "\n";
	}

	/////***** CHOOSING File'S ID TO DEL IT ******/////
	cout << "choose the Id of the file you want to Copy\n";
	int id, v;
	cin >> id;
	do {
		v = 0;
		if (id < 0 || id >= cur->filecnt) { cout << "Invalid Id , try again \n"; v = 1; cin >> id; }
	} while (v == 1);

	////***KEEP or BACK ****/////
	cout << "press 0 to back and 1 to copy the file you choosed\n";
	int x;  cin >> x;
	if (x == 0)
	{
		cout << "No copy"; return;
	}

	////*** COPYING ****/////
	cpyf = true;
	filecopy->f[0].data = cur->f[id].data;
	filecopy->f[0].name = cur->f[id].name;
	filecopy->f[0].size = cur->f[id].size;
	filecopy->f[0].type = cur->f[id].type;
	cout << "Copying " << filecopy->f[0].name << " is done\n";
}
void cutfile()
{
	cout << "Inside " << cur->name << " there is " << cur->foldercnt << " FOLDERS and " << cur->filecnt << " FILES\n";
	for (int i = 0; i < cur->filecnt; i++)
	{
		cout << "File :" << cur->f[i].name << cur->f[i].type << " ID = " << i << "\n";
	}

	/////***** CHOOSING File'S ID TO DEL IT ******/////
	cout << "choose the Id of the file you want to CUT\n";
	int id, v;
	cin >> id;
	do {
		v = 0;
		if (id < 0 || id >= cur->filecnt) { cout << "Invalid Id , try again \n"; v = 1; cin >> id; }
	} while (v == 1);

	////***KEEP or BACK ****/////
	cout << "press 0 to back and 1 to copy the file you choosed\n";
	int x;  cin >> x;
	if (x == 0)
	{
		cout << "No copy"; return;
	}

	////*** COPYING ****/////
	cpyf = true;
	filecopy->f[0].data = cur->f[id].data;
	filecopy->f[0].name = cur->f[id].name;
	filecopy->f[0].size = cur->f[id].size;
	filecopy->f[0].type = cur->f[id].type;

	long long sz = cur->f[id].size;
	THESIZE += sz;

	////*** DELETING ***///
	cur->f[id].data = cur->f[cur->filecnt - 1].data;
	cur->f[id].name = cur->f[cur->filecnt - 1].name;
	cur->f[id].size = cur->f[cur->filecnt - 1].size;
	cur->f[id].type = cur->f[cur->filecnt - 1].type;
	cur->filecnt--;
	cout << "Cut Is Done\n";
}
void pastefile()
{
	if (!cpyf)
	{
		cout << "there is no copy to paste it\n"; return;
	}
	cout << "Do you want to paste this file in " << cur->name << "? \n"
		<< "enter 1 for yes 0 for NO \n";
	int x; cin >> x;
	if (x == 0)
	{
		cout << "Ok , not here"; return;
	}
	long long sz = filecopy->f[0].size;
	if (THESIZE < sz)
	{
		cout << "Sorry there is no free space \n";
		return;
	}
	THESIZE -= sz;
	string str = "";
	str += filecopy->f[0].name; str += filecopy->f[0].type;
	copyy->name = isdup(str);
	cout << "The copied file name here is" << str << "\n";

	folder* node = new folder;
	node->f[0].data = filecopy->f[0].data;
	node->f[0].name = filecopy->f[0].name;
	node->f[0].size = filecopy->f[0].size;
	node->f[0].type = filecopy->f[0].type;
	
	cur->f[cur->filecnt] = node->f[0];
	
	cout << cur->f[cur->filecnt].name << " is copied to " << cur->name << "\n";
	cur->filecnt++;;
}
void deletefile()
{
	cout << "Inside " << cur->name << " there is " << cur->foldercnt << " FOLDERS and " << cur->filecnt << " FILES\n";
	for (int i = 0; i < cur->filecnt; i++)
	{
		cout << "File :" << cur->f[i].name << cur->f[i].type << " ID = " << i << "\n";
	}
	/////***** CHOOSING FOLDER'S ID TO DEL IT ******/////
	cout << "choose the Id of the File you want to delete\n";
	int id, v;
	cin >> id;
	do {
		v = 0;
		if (id < 0 || id >= cur->filecnt) { cout << "Invalid Id"; v = 1; cin >> id; }
	} while (v == 1);

	////***KEEP or BACK ****/////
	cout << "press 0 to back and 1 to delete the File you choosed\n";
	int x;  cin >> x;
	if (x == 0)
	{
		cout << "the File you choosed is still undeleted\n"; return;
	}
	long long sz = cur->f[id].size; 
	THESIZE += sz;
	
	////*** DELETING ***///
	cur->f[id].data = cur->f[cur->filecnt-1].data;
	cur->f[id].name = cur->f[cur->filecnt - 1].name;
	cur->f[id].size = cur->f[cur->filecnt - 1].size;
	cur->f[id].type = cur->f[cur->filecnt - 1].type;
	cur->filecnt--;
	cout << "Delete Is Done\n";
}


void ShowFolderComp()
{
	cout << "Inside " << cur->name << " there is " << cur->foldercnt  << " FOLDERS and " << cur->filecnt  << " FILES\n";
	for (int i = 0; i < cur->foldercnt ; i++)
	{
		cout << "Folder :" << cur->next[i]->name << "\n";
	}
	for (int i = 0; i < cur->filecnt ; i++)
	{
		cout << "File :" << cur->f[i].name <<" "<< cur->f[i].type<<" "<< cur->f[i].size<< "\n";
	}
}
void ShowFolderSize()
{
	cout << cur->name << " size = " <<getfoldersize(cur)<< " \n";
}

//Depth First Search Algorithm
void dfs(folder* node ,string p, string p2)  
{
	/////***** making paths *****///////
	string folderpath,filepath;
	
	folderpath = p;
	folderpath += "\\"; folderpath += "\\";
	folderpath += node->name;
	filepath = p2;
	filepath += "\\";
	filepath += node->name;
	
	const char* pathh = folderpath.c_str();

	// create Parent folder
	CreateDirectory(pathh, NULL);

	// go in childern folders
	for (int i = 0; i < node->foldercnt; i++)
	{
			dfs(node->next[i], folderpath, filepath);
	}
	
	// create children files
	for (int i = 0; i < node->filecnt; i++)
	{
		string p = filepath;
		p += "\\";
		p += node->f[i].name;
		p += ".";
		p += node->f[i].type;
		const char* pathh = p.c_str();
		//puts(pathh);

		FILE *destination;                                    
		errno_t errorCode = fopen_s(&destination, pathh, "wb");
		fwrite(node->f[i].data, 1, node->f[i].size, destination);
		fclose(destination);
	}
}
void savedata()
{
	cur = root;
	string path;
	cout << "The Path you want to save the data in \n";
	cin >> path;
	dfs(cur, path, path );

	cout << "Saving has been done :) \n";
}

//Breadth First Search Algorithm
bool isfolder(string s)
{
	string name = s;
	for (int i = name.size() - 1; i >= 0; i--)
	{
		if (s[i] == '.') return false;
	}
	return true;
}
string getname(string name)
{
	string str = "";
	for (int i = name.size() - 1; i >= 0; i--)
	{
		if (name[i] == '\\') break;
		str = name[i] + str;
	}
	return str;
}
void bfs()
{
	while (!q.empty())
	{
		parentpath pp = q.front();
		q.pop();

		for (auto &p : std::experimental::filesystem::directory_iterator(pp.path))
		{
			string path = p.path().string();
			cout << path << "\n";
			if (path == "") return;

			if (isfolder(getname(path)))
			{
				folder*	cur = new folder;
				cur->name = getname(path);
				cur->prev = pp.parent;
				cur->filecnt = 0;
				cur->foldercnt = 0;
				pp.parent->next[pp.parent->foldercnt] = cur;
				pp.parent->foldercnt++;
				
				parentpath child;
				child.parent = cur; child.path = path;

				q.push(child);
			}
			else
			{
				const char* pathh = path.c_str();
				FILE *source;                                     // return pointer
				errno_t errorCode = fopen_s(&source, pathh, "rb"); // file name & Kind of access  //DOCS 
				if (source == NULL)
				{
					cout << "Can't Open File.\n"; return;
				}
				fseek(source, 0, SEEK_END);                     // Stack overflow
				int size_data = ftell(source);
				rewind(source);                                 // return to the fils start   //DOCS

				char *data = new char[size_data];
				fread(data, 1, size_data, source);              // will save data on buffer // DOCS
																////*****    SEE IF THERE IS SPACE    *****///// 
				if (THESIZE - size_data < 0)
				{
					cout << "NO Free Space\n";
					return;
				}
				else THESIZE -= size_data;

				int v = 0;
				string name = "", type = "", s = getname(path);
				for (int i = s.size() - 1; i >= 0; i--)
				{
					if (s[i] == '.')
					{
						v++; continue;
					}
					else if (s[i] == '\\')
					{
						v++; continue;
					}
					if (v == 0)
					{
						type = s[i] + type;
					}
					else if (v == 1)
					{
						name = s[i] + name;
					}
					else break;
				}

				cout << "we loaded the file " << name << "\n";
				pp.parent->f[pp.parent->filecnt].name = name;
				pp.parent->f[pp.parent->filecnt].data = data;
				pp.parent->f[pp.parent->filecnt].size = size_data;
				pp.parent->f[pp.parent->filecnt].type = type;
				pp.parent->filecnt++;
			}

		}
	}
}
void HardToRam()
{
	///////////////// will we will need to intialise memory??

	cout << "Enter the path of the folder you want to load\n";
	string s;  
	cin.ignore();
	getline(cin, s);
	cout << s << "\n";
	
	root = new folder;
	root->name = getname(s);
	root->filecnt = 0;
	root->foldercnt = 0;
	root->prev = nullptr;
	
	parentpath pp;
	pp.parent = root;
	pp.path = s;
	
	q.push(pp);
	bfs();
	cout << "loading had done succesfully :) \n";
	cur = root;
	THESIZE = 1024 * 1024 * 100 ;
	THESIZE -= getfoldersize(root);
	if (THESIZE < 0) { cout << "NO free space for that folder "; /* i didnt erase it */ return; }
}

void moveback()
{
	if (cur->prev == nullptr)
	{
		cout << "We Are On The Root\n";
		return;
	}
	cur = cur->prev;
	cout << "We Moved Back To " << cur->name << "\n";
}
void movenext()
{
	if (cur->foldercnt == 0)
	{
		cout << "Its a leave there is no more\n";
		return;
	}
	/////***** CHOOSING FOLDER'S ID TO MOVE TO IT ******/////
	cout << "Enter th ID of the folder you want to move to\n";
	for (int i = 0; i < cur->foldercnt; i++)
	{
		cout << "Folder Name:" << cur->next[i]->name <<" Folder Id "<< i <<"\n";
	}
	int id , v;
	cin >> id;
	do {
		v = 0;
		if (id < 0 || id >= cur->foldercnt) { cout << "Invalid Id, try again \n"; v = 1; cin >> id;}
	   }
	while (v==1);
	
	   cur = cur->next[id];
	   cout << "Now we Are on " << cur->name << "\n";
}

int main()
{
	root->name = "root";
	root->filecnt = 0;
	root->foldercnt = 0;
	root->prev = nullptr;
	cur = root;
	do
	{
		string input;
	   cout << "----------------------------------------------\n";
	   cout << " CF to create folder\n"
		   << " rf to read file \n"
		   << " MN to move next \n"
		   << " MB to move back \n"
		   
		   << " cutF to cut folder \n"
		   << " copyF to copy folder\n"
		   << " pasteF to paste folder \n"
		   << " delF to delete folder\n"
		  
		   << " cut to cut file \n"
		   << " copy to copy file\n"
		   << " paste to paste file \n"
		   << " del to delete file\n"
		   
		   << " SFC to Show Folder Components \n"
		   << " SFS to Show Folder Size \n"
		   << " save to Save Data \n"
		   << " load to load Data from disk \n"
		   << " exist to Exit\n";

	   cout << "----------------------------------------------\n";
		cin >> input;
	         
		     if (input == "CF")   addfolder();
		else if (input == "rf")   readfile();
		else if (input == "MN")   movenext();
		else if (input == "MB")   moveback();
	
		else if (input == "SFC")  ShowFolderComp();
		else if (input == "SFS")  ShowFolderSize();
		
		else if (input == "copyF") copyfolder();
		else if (input == "cutF")  cutfolder();
		else if (input == "pasteF")pastefolder();
		else if (input == "delF")  deletfolder();
		
		else if (input == "copy") copyfile();
		else if (input == "cut")  cutfile();
		else if (input == "paste")pastefile();
		else if (input == "del")  deletefile();

		else if (input == "save") savedata();
		else if (input == "load") HardToRam();
		else if (input == "exit") break;

	} while (1);

	system("pause");
	return 0;
}

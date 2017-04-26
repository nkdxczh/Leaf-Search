#include <iostream>  
#include "windows.h"  
#include <string.h>  
#include <Strsafe.h>
#include <sstream>
#include "frame.h"
#include <fstream>
#include <direct.h>
using namespace std;

//传入要遍历的文件夹路径，并遍历相应文件夹  
void TraverseDirectory(int type,wchar_t Dir[MAX_PATH])
{
	int id=0;
	char outputfilename[] = "g:\\LSearchData\\catalogue1.txt"; // 此处写入文件名 
	ofstream fout(outputfilename , ios::out | ios::app);

	locale loc("chs");                //支持中文输出，否则wchar可能无法输出值为中文的变量  
	wcout.imbue(loc);
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	wchar_t DirSpec[MAX_PATH];                  //定义要遍历的文件夹的目录  
	DWORD dwError;
	StringCchCopy(DirSpec, MAX_PATH, Dir);
	StringCchCat(DirSpec, MAX_PATH, TEXT("\\*"));   //定义要遍历的文件夹的完整路径\*  

	hFind = FindFirstFile(DirSpec, &FindFileData);          //找到文件夹中的第一个文件  

	if (hFind == INVALID_HANDLE_VALUE)                               //如果hFind句柄创建失败，输出错误信息  
	{
		FindClose(hFind);
		return;
	}
	else
	{
		while (FindNextFile(hFind, &FindFileData) != 0)                            //当文件或者文件夹存在时  
		{
			if ((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) != 0 && wcscmp(FindFileData.cFileName, L".") == 0 || wcscmp(FindFileData.cFileName, L"..") == 0)        //判断是文件夹&&表示为"."||表示为"."  
			{
				continue;
			}
			if ((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) != 0)      //判断如果是文件夹  
			{
				wchar_t DirAdd[MAX_PATH];
				StringCchCopy(DirAdd, MAX_PATH, Dir);
				StringCchCat(DirAdd, MAX_PATH, TEXT("\\"));
				StringCchCat(DirAdd, MAX_PATH, FindFileData.cFileName);       //拼接得到此文件夹的完整路径  

				string szDst;
				wchar_t DirName[MAX_PATH];
				StringCchCat(DirName, MAX_PATH, FindFileData.cFileName);

				DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, FindFileData.cFileName, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte的运用
				char *psText;  // psText为char*的临时数组，作为赋值给std::string的中间变量
				psText = new char[dwNum];
				WideCharToMultiByte(CP_OEMCP, NULL, FindFileData.cFileName, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte的再次运用
				szDst = psText;// std::string赋值
				delete[]psText;// psText的清除
				fout << type << " " << szDst << endl;
				type = atoi(szDst.c_str());

				char dir[MAX_PATH];
				sprintf_s(dir, "g:\\LSearchData\\cutLeaveForTraining\\%d\\",type);
				_mkdir(dir);
				sprintf_s(dir, "g:\\LSearchData\\cutLeaveForTesting\\%d\\", type);
				_mkdir(dir);
				TraverseDirectory(type++,DirAdd);                                  //实现递归调用  
			}
			if ((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == 0)    //如果不是文件夹  
			{
				string filename;
				//wcout << Dir << "\\" << FindFileData.cFileName<<endl;

				wchar_t * wText = Dir;
				DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte的运用
				char *psText;  // psText为char*的临时数组，作为赋值给std::string的中间变量
				psText = new char[dwNum];
				WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte的再次运用
				filename = psText;// std::string赋值
				delete[]psText;// psText的清除

				filename += "\\" ;

				wText = FindFileData.cFileName;
				dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte的运用
				psText;  // psText为char*的临时数组，作为赋值给std::string的中间变量
				psText = new char[dwNum];
				WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte的再次运用
				filename += psText;// std::string赋值
				delete[]psText;// psText的清除

				dispose(type,id,filename);
				id++;
			}
		}
		FindClose(hFind);
	}
}
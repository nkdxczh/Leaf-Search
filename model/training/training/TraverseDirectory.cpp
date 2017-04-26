#include <iostream>  
#include "windows.h"  
#include <string.h>  
#include <Strsafe.h>
#include <sstream>
#include "frame.h"
#include <fstream>
#include <direct.h>
using namespace std;

//����Ҫ�������ļ���·������������Ӧ�ļ���  
void TraverseDirectory(int type,wchar_t Dir[MAX_PATH])
{
	int id=0;
	char outputfilename[] = "g:\\LSearchData\\catalogue1.txt"; // �˴�д���ļ��� 
	ofstream fout(outputfilename , ios::out | ios::app);

	locale loc("chs");                //֧���������������wchar�����޷����ֵΪ���ĵı���  
	wcout.imbue(loc);
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	wchar_t DirSpec[MAX_PATH];                  //����Ҫ�������ļ��е�Ŀ¼  
	DWORD dwError;
	StringCchCopy(DirSpec, MAX_PATH, Dir);
	StringCchCat(DirSpec, MAX_PATH, TEXT("\\*"));   //����Ҫ�������ļ��е�����·��\*  

	hFind = FindFirstFile(DirSpec, &FindFileData);          //�ҵ��ļ����еĵ�һ���ļ�  

	if (hFind == INVALID_HANDLE_VALUE)                               //���hFind�������ʧ�ܣ����������Ϣ  
	{
		FindClose(hFind);
		return;
	}
	else
	{
		while (FindNextFile(hFind, &FindFileData) != 0)                            //���ļ������ļ��д���ʱ  
		{
			if ((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) != 0 && wcscmp(FindFileData.cFileName, L".") == 0 || wcscmp(FindFileData.cFileName, L"..") == 0)        //�ж����ļ���&&��ʾΪ"."||��ʾΪ"."  
			{
				continue;
			}
			if ((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) != 0)      //�ж�������ļ���  
			{
				wchar_t DirAdd[MAX_PATH];
				StringCchCopy(DirAdd, MAX_PATH, Dir);
				StringCchCat(DirAdd, MAX_PATH, TEXT("\\"));
				StringCchCat(DirAdd, MAX_PATH, FindFileData.cFileName);       //ƴ�ӵõ����ļ��е�����·��  

				string szDst;
				wchar_t DirName[MAX_PATH];
				StringCchCat(DirName, MAX_PATH, FindFileData.cFileName);

				DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, FindFileData.cFileName, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte������
				char *psText;  // psTextΪchar*����ʱ���飬��Ϊ��ֵ��std::string���м����
				psText = new char[dwNum];
				WideCharToMultiByte(CP_OEMCP, NULL, FindFileData.cFileName, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte���ٴ�����
				szDst = psText;// std::string��ֵ
				delete[]psText;// psText�����
				fout << type << " " << szDst << endl;
				type = atoi(szDst.c_str());

				char dir[MAX_PATH];
				sprintf_s(dir, "g:\\LSearchData\\cutLeaveForTraining\\%d\\",type);
				_mkdir(dir);
				sprintf_s(dir, "g:\\LSearchData\\cutLeaveForTesting\\%d\\", type);
				_mkdir(dir);
				TraverseDirectory(type++,DirAdd);                                  //ʵ�ֵݹ����  
			}
			if ((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == 0)    //��������ļ���  
			{
				string filename;
				//wcout << Dir << "\\" << FindFileData.cFileName<<endl;

				wchar_t * wText = Dir;
				DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte������
				char *psText;  // psTextΪchar*����ʱ���飬��Ϊ��ֵ��std::string���м����
				psText = new char[dwNum];
				WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte���ٴ�����
				filename = psText;// std::string��ֵ
				delete[]psText;// psText�����

				filename += "\\" ;

				wText = FindFileData.cFileName;
				dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte������
				psText;  // psTextΪchar*����ʱ���飬��Ϊ��ֵ��std::string���м����
				psText = new char[dwNum];
				WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte���ٴ�����
				filename += psText;// std::string��ֵ
				delete[]psText;// psText�����

				dispose(type,id,filename);
				id++;
			}
		}
		FindClose(hFind);
	}
}
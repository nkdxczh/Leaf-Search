#include "frame.h"
#include <stdio.h>  
#include <windows.h> 
#include <fstream>
#pragma comment(lib, "Winmm.lib")

int main(int argc, wchar_t *argv[], wchar_t *envp[])
{
	//TraverseDirectory(1,L"G:\\����\\����\\��Ҷ��");         //����ָ�����ļ��У��˴��ļ�·���ɰ���������޸�  


	vector<int> types;
	vector<vector<Point> > contours;
	readContours("g:\\LSearchData\\outlineForTraining.txt", types, contours);


	ofstream out("g:\\LSearchData\\feature\\time.txt");
	DWORD  dwBegin, dwEnd;
	dwBegin = timeGetTime();

	Centroid_Radii("g:\\LSearchData\\feature\\Centroid_Radii.txt", types, contours);

	dwEnd = timeGetTime();
	printf("%d\n", dwEnd - dwBegin);
	out << "Centroid_Radii " << dwEnd - dwBegin << endl;

	//system("pause");
	return 0;
}
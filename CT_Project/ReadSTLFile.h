#pragma once
#include <vector>
#include <QVector3D>

using namespace std;

class ReadSTLFile
{
public:
	bool ReadFile(const char *cfilename);
	int NumTri();
	vector<QVector3D> getPointList();
	vector<QVector3D> getVectorList();
private:
	vector<QVector3D> pointList;
	vector<QVector3D> vectorList;

	unsigned int unTriangles;
	char* memwriter;

	bool ReadASCII(const char *cfilename);// 读入ascii码形式的 stl文件   本项目，由于生成的是二进制stl，所以这个函数就没有用到
	bool ReadBinary(const char *cfilename);// 读入二进制的stl文件

	int cpyint(const char*& p);
	float cpyfloat(const char*& p);
};
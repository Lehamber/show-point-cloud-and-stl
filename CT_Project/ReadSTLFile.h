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
	
	bool ReadBinary(const char *cfilename);// 读入二进制的stl文件
	int cpyint(const char*& p);
	float cpyfloat(const char*& p);
};

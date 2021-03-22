#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include"ReadSTLFile.h"
#include <sstream>    

using namespace std;

bool ReadSTLFile::ReadFile(const char *cfilename)
{
	FILE * pFile;
	long lSize;
	char* buffer;
	size_t result;

	/* 若要一个byte不漏地读入整个文件，只能采用二进制方式打开 */
	fopen_s(&pFile, cfilename, "rb");
	if (pFile == NULL)
	{
		return false;
	}

	/* 获取文件大小 */
	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);

	/* 分配内存存储整个文件 */
	buffer = (char*)malloc(sizeof(char)*lSize);
	if (buffer == NULL)
	{
		fputs("Memory error", stderr);
		exit(2);
	}

	/* 将文件拷贝到buffer中 */
	result = fread(buffer, 1, lSize, pFile);
	if (result != lSize)
	{
		fputs("Reading error", stderr);
		exit(3);
	}


	/* 结束演示，关闭文件并释放内存 */
	fclose(pFile);

	ios::sync_with_stdio(false);

	//它这个判断格式的代码  估计不太对，明明是二进制的文件，它这里直接使用了 读ascii码的方式， 所以直接用二进制读吧

	//if (buffer[79] != '\0')//判断格式   
	//{
	//	ReadASCII(buffer);
	//}
	//else
	//{
	//	ReadBinary(buffer);
	//}
	//由于 我们 的程序生成stl是 二进制的所以我们这里就不做判断了，直接只用二进制的方式来读入
	ReadBinary(buffer);
	ios::sync_with_stdio(true);

	free(buffer);
	return true;
}

bool ReadSTLFile::ReadASCII(const char *buffer)
{
	unTriangles = 0;
	float x, y, z;
	int i;
	string name, useless;
	stringstream ss(buffer);
	ss >> name >> name;
	ss.get();
	do {
		ss >> useless;
		if (useless != "facet")
			break;
		getline(ss, useless);
		getline(ss, useless);
		for (i = 0; i < 3; i++)
		{
			ss >> useless >> x >> y >> z;
			pointList.push_back(QVector3D(x, y, z));
		}
		unTriangles++;
		getline(ss, useless);
		getline(ss, useless);
		getline(ss, useless);
	} while (1);
	return true;
}

bool ReadSTLFile::ReadBinary(const char *buffer)
{
	const char* p = buffer;
	char name[80];
	int i, j;
	memcpy(name, p, 80);
	p += 80;
	unTriangles = cpyint(p);

	const char *tempP;
	for (i = 0; i < unTriangles; i++)
	{
		vectorList.push_back(QVector3D(cpyfloat(p), cpyfloat(p), cpyfloat(p)));//读取 三角片面的向量信息
		for (j = 0; j < 3; j++)//读取三顶点
		{
			pointList.push_back(QVector3D(cpyfloat(p), cpyfloat(p), cpyfloat(p)));
		}
		p += 2;//跳过尾部标志
	}
	return true;
}

int ReadSTLFile::NumTri()
{
	return unTriangles;
}

vector<QVector3D> ReadSTLFile::getPointList()// 获取点数据
{
	return pointList;
}
vector<QVector3D> ReadSTLFile::getVectorList()// 获取三角片面 向量数据
{
	return vectorList;
}

int ReadSTLFile::cpyint(const char*& p)
{
	int cpy;
	char byte[4];
	for (int i = 0; i < 4; i++)
	{
		byte[i] = *p;
		p++;
	}
	cpy = *(int*)byte;
	//memwriter = (char*)&cpy;
	//memcpy(memwriter, p, 4);
	//p += 4;
	return cpy;
}
float ReadSTLFile::cpyfloat(const char*& p)
{
	float cpy;
	char byte[4];
	for (int i = 0; i < 4; i++)
	{
		byte[i] = *p;
		p++;
	}
	cpy = *(float*)byte;
	/*memwriter = (char*)&cpy;
	memcpy(memwriter, p, 4);
	p += 4;*/
	return cpy;
}

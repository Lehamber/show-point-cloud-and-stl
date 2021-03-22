#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <QVector3D>
using namespace std;

class PointCloud
{
public:
	PointCloud();
	~PointCloud();
	
	QVector3D maxCoordinate;//最大坐标点
	QVector3D minCoordinate;//最小坐标大
	vector<QVector3D> pointData;//点数据
	
	float getFactor();
public:
	bool read(const char* path);//从文件中读取点云数据 并按比例 映射到 -1.0 -- 1.0 的范围之内
	void handlePointDate(vector<QVector3D> data);// 规格化点云数据， 映射到 -1.0 -- 1.0 的范围之内
	void getMaxCoordinate();// 分别计算出 x y z 的最大值 组合成一个点；
	void getMinCoordinate();//  分别计算 x y z 的最小值 组合成一个；
	void getCenterPoint(QVector3D &vec); // 获取中心点
	  
};


#include "PointCloud.h"


PointCloud::PointCloud()
{

}
PointCloud::~PointCloud()
{

}

bool PointCloud::read(const char* path)
{
	fstream readTexData(path);
	if (!readTexData)
	{
		return false;
	}
	QVector3D data;
	float x, y, z;
	while (readTexData >> x >> y >> z)
	{
		data.setX(x);
		data.setY(y);
		data.setZ(z);
		pointData.push_back(data);
	}
	readTexData.close();

	getMaxCoordinate();
	getMinCoordinate();
	//将所有的点想0.0.0点 移动，使中值点  位于0.0.0点，这样就不用考虑旋转轴的问题了
	QVector3D centerPoint;
	getCenterPoint(centerPoint);

	for (int i = 0; i < pointData.size(); i++)
	{
		pointData[i].setX(pointData[i].x() - centerPoint.x());
		pointData[i].setY(pointData[i].y() - centerPoint.y());
		pointData[i].setZ(pointData[i].z() - centerPoint.z());
	}
	//更新最大 最小值点 坐标s
	getMaxCoordinate();
	getMinCoordinate();

	//建立一个 以可以容纳所有点的 正方体 然后 让顶点映射到 0.5（所有点都进行映射）
	float factor = getFactor();//获取变换因子
	for (int i = 0; i < pointData.size(); i++)
	{
		pointData[i].setX(pointData[i].x() * factor);
		pointData[i].setY(pointData[i].y() * factor);
		pointData[i].setZ(pointData[i].z() * factor);
	}

	//更新最大 最小值点 坐标s
	getMaxCoordinate();
	getMinCoordinate();

	return true;
}
void PointCloud::handlePointDate(vector<QVector3D> data) {
	pointData = data;

	getMaxCoordinate();
	getMinCoordinate();
	//将所有的点想0.0.0点 移动，使中值点  位于0.0.0点，这样就不用考虑旋转轴的问题了
	QVector3D centerPoint;
	getCenterPoint(centerPoint);

	for (int i = 0; i < pointData.size(); i++)
	{
		pointData[i].setX(pointData[i].x() - centerPoint.x());
		pointData[i].setY(pointData[i].y() - centerPoint.y());
		pointData[i].setZ(pointData[i].z() - centerPoint.z());
	}
	//更新最大 最小值点 坐标s
	getMaxCoordinate();
	getMinCoordinate();

	//建立一个 以可以容纳所有点的 正方体 然后 让顶点映射到 0.5（所有点都进行映射）
	float factor = getFactor();//获取变换因子
	for (int i = 0; i < pointData.size(); i++)
	{
		pointData[i].setX(pointData[i].x() * factor);
		pointData[i].setY(pointData[i].y() * factor);
		pointData[i].setZ(pointData[i].z() * factor);
	}

	//更新最大 最小值点 坐标s
	getMaxCoordinate();
	getMinCoordinate();
}
float PointCloud::getFactor()
{
	float max = 0;
	if (max <= maxCoordinate.x())
	{
		max = maxCoordinate.x();
	}
	if (max <= maxCoordinate.y())
	{
		max = maxCoordinate.y();
	}
	if (max <= maxCoordinate.z())
	{
		max = maxCoordinate.z();
	}
	return 0.5 / max;
}
void PointCloud::getMaxCoordinate()
{
	if (0 == pointData.size())
	{
		return;
	}
	QVector3D vec;
	vec.setX(pointData[0].x());
	vec.setY(pointData[0].y());
	vec.setZ(pointData[0].z());

	for (int i = 0; i < pointData.size(); i++)
	{
		if (vec.x() < pointData[i].x())
		{
			vec.setX(pointData[i].x());
		}
		if (vec.y() < pointData[i].y())
		{
			vec.setY(pointData[i].y());
		}
		if (vec.z() < pointData[i].z())
		{
			vec.setZ(pointData[i].z());
		}
	}
	maxCoordinate = vec;
}

void PointCloud::getMinCoordinate()
{
	if (0 == pointData.size())
	{
		return;
	}
	QVector3D vec;

	vec.setX(pointData[0].x());
	vec.setY(pointData[0].y());
	vec.setZ(pointData[0].z());

	for (int i = 0; i < pointData.size(); i++)
	{
		if (vec.x() > pointData[i].x())
		{
			vec.setX(pointData[i].x());
		}
		if (vec.y() > pointData[i].y())
		{
			vec.setY(pointData[i].y());
		}
		if (vec.z() > pointData[i].z())
		{
			vec.setZ(pointData[i].z());
		}
	}
	minCoordinate = vec;
}
void PointCloud::getCenterPoint(QVector3D &vec)
{
	if (0 == pointData.size())
	{
		return;
	}
	//取重心
	/*float xSum = 0;
	float ySum = 0;
	float zSum = 0;
	for (int i = 0; i < pointData.size(); i++)
	{
		xSum += pointData[i].x();
		ySum += pointData[i].y();
		zSum += pointData[i].z();
	}
	vec.setX(xSum / pointData.size());
	vec.setY(ySum / pointData.size());
	vec.setZ(zSum / pointData.size());*/

	//取中心， 因为人的眼睛 习惯上 会把物体的中心当做中心点，而不是 重心，取重心的话，旋转效果可能会很怪异
	vec.setX((maxCoordinate.x() + minCoordinate.x()) / 2);
	vec.setY((maxCoordinate.y() + minCoordinate.y()) / 2);
	vec.setZ((maxCoordinate.z() + minCoordinate.z()) / 2);
}
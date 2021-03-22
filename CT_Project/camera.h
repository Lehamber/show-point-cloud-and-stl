#pragma once
#include <QObject>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include <QtMath>
#include <QKeyEvent>
#include <QOpenGLShader>

const float SENSITIVITY = 0.01f;

class Camera
{
public:
	// Camera Attributes
	QVector3D Position;
	QVector3D Front;
	QVector3D Up;
	QVector3D Right;
	QVector3D WorldUp;

	float scrollSensitivity;// 滚轮敏感度

	Camera(QVector3D position = QVector3D(0.0f, 0.0f, 3.0f), QVector3D front = QVector3D(0.0, 0.0, 0.0), QVector3D up = QVector3D(0.0f, 1.0f, 0.0f)) :scrollSensitivity(SENSITIVITY)

	{
		this->Position = position;
		this->WorldUp = up;
		this->Front = front;
	}

	QMatrix4x4 getViewMatrix(); // 获取 lookAt矩阵
	void processMouseScroll(float yoffset); // 滚轮控制
};


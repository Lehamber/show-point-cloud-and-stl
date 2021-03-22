#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QDebug>
#include <QOpenGLFunctions_3_3_Core>

#include <QOpenGLTexture>

#include <QVector3D>
#include <QKeyEvent>

#include <QTime> 
#include "shaderProgram.h"
#include "camera.h"

const GLfloat WHEELSENSITIVITY = 0.001f;


class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
	Q_OBJECT

private:
	shaderProgram *lightingShader;// 绘制物体的着色器
	/*QOpenGLTexture *texture1;
	QOpenGLTexture *texture2;*/

	QOpenGLFunctions_3_3_Core *core;
	QTime time;
	Camera *camera;
	QVector3D Position;// 摄像机的位置
	QVector3D Front; // 摄像机指向位置
	QVector3D Zoom;// 缩放比例
	GLfloat wheelSensitivity;// 滚轮敏感度

	QMatrix4x4 lampModel;//灯的模式矩阵
	QMatrix4x4 model;  // 模式矩阵
	QMatrix4x4 modelUse;// 这相当于一个中间值
	QMatrix4x4 modelSave;  // 上一次鼠标点击移动过后的 模式矩阵
	QPoint press_position;  // 按下鼠标左键时的位置
	vector<GLfloat> vertices;

	int showType;

public:
	MyGLWidget(QWidget *parent, vector<GLfloat> data, int showType);
	MyGLWidget(vector<GLfloat> data, int show);

	~MyGLWidget();
	GLuint a;

	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

	void setData(vector<GLfloat> data, int showType);

	void setVecValue(vector<GLfloat> data);

	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void setPressPosition(QPoint p_ab);
	void translate_point(QPoint &p_ab);
};
#endif
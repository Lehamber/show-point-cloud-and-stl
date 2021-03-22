#pragma once
#pragma execution_character_set("utf-8")//display chinese words, 不加这句话，qt 就不能正常显示 汉字

#include <QtWidgets/QWidget>
#include <QtWidgets>
#include "ui_MainWindow.h"
#include "MyGLWidget.h"
#include "PointCloud.h"
#include <Qvector3D>
#include "ReadSTLFile.h"

class MainWindow : public QWidget
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();
public slots:
	//void openAndGenerate();// 打开dcm文件 计算生成 stl文件和点云文件
	bool viewPointData();//显示点云
	bool viewStlData();//显示stl文件
	void switchView();// 切换显示的视口
	void viewAfterGenerate();
	void clearTextEdit();
	//int ReadData(const char* str, vector<float> &data);

private:
	Ui::MainWindowClass ui;

	MyGLWidget *pointCloudWidgetDemo;
	MyGLWidget *stlWidgetDemo;
	QStackedWidget *stackedWidgetDemo;

	MyGLWidget *pointCloudWidget;
	MyGLWidget *stlWidget;
	QStackedWidget *stackedWidget;


	// 用来装 点云窗口 和 stl窗口的容器

	int switchViewFlag;
};

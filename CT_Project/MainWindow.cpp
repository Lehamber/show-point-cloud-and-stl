#include "MainWindow.h"
#include <Qvector3D>
#include <iostream>
#include <QStackedWidget>
#include <QByteArray>
#include <QDialog>



const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

MainWindow::~MainWindow() {
	// 回收分配的内存空间
	delete pointCloudWidget;
	delete stlWidget;
	delete stackedWidget;

	delete pointCloudWidgetDemo;
	delete stlWidgetDemo;
	delete stackedWidgetDemo;
}
MainWindow::MainWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	// 初始化指针变量
	pointCloudWidgetDemo = NULL;
	stlWidgetDemo = NULL;
	stackedWidgetDemo = NULL;

	pointCloudWidget = NULL;
	stlWidget = NULL;
	stackedWidget = NULL;

	setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
	setFixedSize(this->width(), this->height());   // 禁止拖动窗口大小
	setWindowTitle("DCM文件处理器");// 设置窗口名字

	// 给按钮链接槽函数
	//connect(ui.openButton, &QPushButton::clicked, this, &MainWindow::openAndGenerate);
	connect(ui.switchButton, &QPushButton::clicked, this, &MainWindow::switchView);
	connect(ui.viewGenButton, &QPushButton::clicked, this, &MainWindow::viewAfterGenerate);
	connect(ui.clearButton, &QPushButton::clicked, this, &MainWindow::clearTextEdit);

	// 读入demo点云数据并显示
	PointCloud pointCloud1;
	pointCloud1.read("testModel\\bunny.txt");
	vector<GLfloat>  pointData1;
	for (int i = 0; i < pointCloud1.pointData.size(); i++)//将所有的点数据放入 点数据 容器中
	{
		pointData1.push_back(pointCloud1.pointData[i].x());
		pointData1.push_back(pointCloud1.pointData[i].y());
		pointData1.push_back(pointCloud1.pointData[i].z());
	}
	//创建点云显示窗口，1 要显示的点云文件
	pointCloudWidgetDemo = new MyGLWidget(this, pointData1, 1);
	pointCloudWidgetDemo->show();

	// 读入demostl数据并显示
	ReadSTLFile readSTLFile;
	readSTLFile.ReadFile("w0w285090.stl");
	PointCloud pointCloud2;
	vector<QVector3D> tempPointList = readSTLFile.getPointList();
	vector<QVector3D> tempVectorList = readSTLFile.getVectorList();
	pointCloud2.handlePointDate(tempPointList);//读入 数据 并把所有的点映射到 中心为 0，0,0点  边长为1 的 正方体中

	vector<GLfloat>  pointData2;
	for (int i = 0; i < pointCloud2.pointData.size(); i++)//将所有的点数据放入 点数据 容器中
	{
		int j = i / 3;
		pointData2.push_back(pointCloud2.pointData[i].x());
		pointData2.push_back(pointCloud2.pointData[i].y());
		pointData2.push_back(pointCloud2.pointData[i].z());

		// 装入向量坐标 同一个三角片面 使用 相同的法向量
		pointData2.push_back(tempVectorList[j].x());
		pointData2.push_back(tempVectorList[j].y());
		pointData2.push_back(tempVectorList[j].z());
	}
	// 创建stl显示窗口  2 代表要显示是stl文件
	stlWidgetDemo = new MyGLWidget(this, pointData2, 2);
	stlWidgetDemo->show();

	stackedWidgetDemo = new QStackedWidget(this);
	stackedWidgetDemo->addWidget(stlWidgetDemo);//添加在 第0页，显示的时候，也是优先显示第零页；
	stackedWidgetDemo->addWidget(pointCloudWidgetDemo);//添加在 第1页
	stackedWidgetDemo->move(this->width() / 4, 0);
	stackedWidgetDemo->resize(this->width() * 4 / 5, this->height());
	stackedWidgetDemo->show();
	switchViewFlag = 2;
}
void MainWindow::switchView()
{
	if (NULL == stackedWidget) {// 如果为空 怎么说明 还未生成 要显示的文件，所以就切换 demo窗口
		if (2 == switchViewFlag)
		{
			stackedWidgetDemo->setCurrentIndex(1);//换成 点云所在的页；
			switchViewFlag = 1;
		}
		else
		{
			stackedWidgetDemo->setCurrentIndex(0);//换成 stl所在的页；
			switchViewFlag = 2;
		}
	}
	else {// stl和点云文件已经生成， 所以就切换生成窗口
		if (2 == switchViewFlag)
		{
			stackedWidget->setCurrentIndex(1);//换成 点云所在的页；
			switchViewFlag = 1;
		}
		else
		{
			stackedWidget->setCurrentIndex(0);//换成 stl所在的页；
			switchViewFlag = 2;
		}
	}
}

void MainWindow::viewAfterGenerate() {
	bool a = false;
	bool b = false;
	a = viewPointData(); // 显示出点云文件 显示成功 返回true 错误 返回false
	b = viewStlData(); // 显示出stl文件 显示成功 返回true 错误 返回false
	if (false == a || false == b)
	{
		ui.textEdit->append("点云文件 或者 stl文件 未生成    ");
		QMessageBox::about(this, "错误", "点云文件 或者 stl文件 未生成   ");
		return;
	}

	stackedWidget = new QStackedWidget(this);
	stackedWidget->addWidget(stlWidget);
	stackedWidget->addWidget(pointCloudWidget);

	stackedWidget->move(this->width() / 4, 0);
	stackedWidget->resize(this->width() * 4 / 5, this->height());
	stackedWidget->show();
	stlWidget->show();
	pointCloudWidget->show();
	switchViewFlag = 2;
}
void MainWindow::clearTextEdit()// 清除 文本框内容
{
	ui.textEdit->clear();
}
bool MainWindow::viewPointData()
{
	PointCloud pointCloud;

	if (false == pointCloud.read("BOLUS.txt"))//读入 数据 并把所有的点映射到 中心为 0，0,0点  边长为1 的 正方体中
	{
		return false;
	}
	vector<GLfloat>  pointData;
	for (int i = 0; i < pointCloud.pointData.size(); i++)//将所有的点数据放入 点数据 容器中
	{
		pointData.push_back(pointCloud.pointData[i].x());
		pointData.push_back(pointCloud.pointData[i].y());
		pointData.push_back(pointCloud.pointData[i].z());
	}
	static bool isFirst1 = true;
	if (true == isFirst1) {
		pointCloudWidget = new MyGLWidget(this, pointData, 1);// 1代表要显示 点云文件 
		isFirst1 = false;
	}
	else
	{
		/*pointCloudWidget->close();
		delete pointCloudWidget;*/  // 加上这步 不知道为啥 会出错，但是不加的 随着点击次数的增多，程序会占用很多的内存， 经过测试发现，
		// 只要不是故意的把，显示已生成按钮 点击 几十次 甚至几百次的话，程序一般不会变的很卡到崩溃； 这里做的不是很好，后者可以进行优化；
		pointCloudWidget = new MyGLWidget(this, pointData, 1);// 1代表要显示 点云文件 
	}
	return true;
}
bool MainWindow::viewStlData() {

	ReadSTLFile readSTLFile;
	if (false == readSTLFile.ReadFile("complete.stl"))// 在exe同目录下 读入生成的stl文件 complete.stl
	{
		return false;
	}
	PointCloud pointCloud;
	vector<QVector3D> tempPointList = readSTLFile.getPointList();//获取所有的点
	vector<QVector3D> tempVectorList = readSTLFile.getVectorList(); // 获取所有三角片面的向量
	pointCloud.handlePointDate(tempPointList);//读入 数据 并把所有的点映射到 中心为 0，0,0点  边长为1 的 正方体中

	vector<GLfloat>  pointData;
	for (int i = 0; i < pointCloud.pointData.size(); i++)//将所有的点数据放入 点数据 容器中
	{
		int j = i / 3;
		pointData.push_back(pointCloud.pointData[i].x());
		pointData.push_back(pointCloud.pointData[i].y());
		pointData.push_back(pointCloud.pointData[i].z());

		// 装入向量坐标 同一个三角片面 使用 相同的法向量
		pointData.push_back(tempVectorList[j].x());
		pointData.push_back(tempVectorList[j].y());
		pointData.push_back(tempVectorList[j].z());
	}
	static bool isFirst2 = true;
	if (true == isFirst2) {
		stlWidget = new MyGLWidget(this, pointData, 2);// 2代表要显示 stl文件 
		isFirst2 = false;
	}
	else
	{
		//stlWidget->close();
		//delete stlWidget; // 加上这步 不知道为啥 会出错，但是不加的 随着点击次数的增多，程序会占用很多的内存， 经过测试发现，
		// 只要不是故意的把，显示已生成按钮 点击 几十次 甚至几百次的话，程序一般不会变的很卡到崩溃； 这里做的不是很好，后者可以进行优化；
		stlWidget = new MyGLWidget(this, pointData, 2);// 2代表要显示 stl文件 
	}
	return true;
}



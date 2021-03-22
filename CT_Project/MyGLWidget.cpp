#include "stdafx.h"
#include "MyGLWidget.h"


GLuint VBO, VAO;

GLfloat angle_now = 0.0f;// 旋转的角度， 最开始是0.0f

MyGLWidget::MyGLWidget(QWidget *parent, vector<GLfloat> data, int showType) : QOpenGLWidget(parent), wheelSensitivity(WHEELSENSITIVITY)
{
	vertices = data;//点的数据
	this->showType = showType;//显示类型，是点 还是显示类型
}

MyGLWidget::~MyGLWidget()
{
	delete lightingShader;
	delete camera;
	core->glDeleteVertexArrays(1, &VAO);
	core->glDeleteBuffers(1, &VBO);
}
void MyGLWidget::setData(vector<GLfloat> data, int showType)
{
	vertices = data;//点的数据
	this->showType = showType;//显示类型，是点 还是显示类型
}

void MyGLWidget::setVecValue(vector<GLfloat> data)
{
	vertices = data;
}
void MyGLWidget::initializeGL()
{
	core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();//  申请一个opengl上下文

	if (1 == showType)// 根据情况 创建相应的 着色器
	{
		lightingShader = new shaderProgram("vertex.vs", "fragment.fs");// 显示点云
	}
	else {
		lightingShader = new shaderProgram("vertex_object.vs", "fragment_object.fs");
	}

	//lampShader = new shaderProgram("vertex_lamp.vs", "fragment_lamp.fs");

	core->glPointSize(1.0f);// 点的大小

	core->glGenVertexArrays(1, &VAO);
	core->glGenBuffers(1, &VBO);

	core->glBindVertexArray(VAO);

	core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	core->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0])*vertices.size(), &vertices[0], GL_STATIC_DRAW);

	if (1 == showType) { // 如果要显示点云的话， 咱们这里选择 不需要 打光
		core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		core->glEnableVertexAttribArray(0);
	}
	else {
		core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		core->glEnableVertexAttribArray(0);

		core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		core->glEnableVertexAttribArray(1);
	}

	time.start();// 让时钟开始计时 走起来  这里好像没用

	core->glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // 给上下文设置 清屏颜色
	core->glEnable(GL_DEPTH_TEST);   //  让上下启动深度测试

	//建立摄像机
	camera = new Camera();

}

void MyGLWidget::resizeGL(int width, int height)
{
	core->glViewport(0, 0, width, height);
}

void MyGLWidget::paintGL()
{
	core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 执行这两个缓冲里面的 东西
	lightingShader->use();

	QMatrix4x4 projection;
	projection.perspective(45.0f, (GLfloat)width() / (GLfloat)height(), 0.1f, 200.f);

	lightingShader->setUniformMat4("projection", projection);
	lightingShader->setUniformMat4("view", camera->getViewMatrix());
	lightingShader->setUniformMat4("model", model);

	if (2 == showType) {// 2 代表要显示stl文件， 下面对显示stl 的着色器 进行一定的配置

		lightingShader->setUniformVec3("viewPos", camera->Position);//这个 需要动态的来更新，因为 摄像机的位置会变化
		//1.0f, 0.5f, 0.31f橙色
		lightingShader->setUniformVec3("material.ambient", QVector3D(0.2f, 0.8f, 0.8f));
		lightingShader->setUniformVec3("material.diffuse", QVector3D(0.2f, 0.8f, 0.8f));
		lightingShader->setUniformVec3("material.specular", QVector3D(0.05f, 0.05f, 0.05f));
		lightingShader->setUniformFloat("material.shininess", 32.0f);// 材质

		lightingShader->setUniformVec3("light1.ambient", QVector3D(0.2f, 0.2f, 0.2f));
		lightingShader->setUniformVec3("light1.diffuse", QVector3D(0.5f, 0.5f, 0.5f)); // 将光照调暗了一些以搭配场景
		lightingShader->setUniformVec3("light1.specular", QVector3D(1.0f, 1.0f, 1.0f));
		lightingShader->setUniformVec3("light1.direction", QVector3D(0.0f, 0.0f, 3.0f));//  前平行光的方向

		lightingShader->setUniformVec3("light2.ambient", QVector3D(0.2f, 0.2f, 0.2f));
		lightingShader->setUniformVec3("light2.diffuse", QVector3D(0.5f, 0.5f, 0.5f)); // 将光照调暗了一些以搭配场景
		lightingShader->setUniformVec3("light2.specular", QVector3D(1.0f, 1.0f, 1.0f));
		lightingShader->setUniformVec3("light2.direction", QVector3D(0.0f, 0.0f, -3.0f));//  后平行光的方向

	}

	core->glBindVertexArray(VAO);
	if (1 == showType) {
		core->glDrawArrays(GL_POINTS, 0, vertices.size());
	}
	if (2 == showType) {
		core->glDrawArrays(GL_TRIANGLES, 0, vertices.size());//这里应该是点的数量吧，  而不是三小形的数量
	}
	update();
}


void MyGLWidget::mouseMoveEvent(QMouseEvent * event)// 获取鼠标移动
{
	QPoint p_ab = event->pos();
	translate_point(p_ab);

	QPoint sub_point = p_ab - press_position;
	if (event->buttons() & Qt::LeftButton)
	{
		model.setToIdentity();// 转化成单位矩阵

		GLfloat angle_now = qSqrt(qPow(sub_point.x(), 2) + qPow(sub_point.y(), 2)) / 5;

		model.rotate(angle_now, -sub_point.y(), sub_point.x(), 0.0);
		model = model * modelUse;

		// 对旋转进行叠加
		modelSave.setToIdentity();// 转化成单位矩阵，
		modelSave.rotate(angle_now, -sub_point.y(), sub_point.x(), 0.0);
		modelSave = modelSave * modelUse;
	}

}

void MyGLWidget::wheelEvent(QWheelEvent * event)// 获取滚轮的转动
{
	QPoint offset = event->angleDelta();// 获得滚轮转动的方向 和 被转动的距离

	camera->processMouseScroll(offset.y() / 20);

}

void MyGLWidget::mousePressEvent(QMouseEvent * event)
{
	//单击 
	QPoint p_ab = event->pos();
	// 如果是鼠标左键按下
	if (event->button() == Qt::LeftButton)
	{
		modelUse = modelSave;
		setPressPosition(p_ab);
	}
}

void MyGLWidget::setPressPosition(QPoint p_ab)
{
	translate_point(p_ab);
	press_position = p_ab;
}
void MyGLWidget::translate_point(QPoint &p_ab)
{
	int x = p_ab.x() - this->width() / 2;
	int y = -(p_ab.y() - this->width() / 2);// 由于点击的时候  原点是在 窗口左上角，y轴正半轴朝下，而咱们opengl中， 原点是在下面，
											// y轴正半轴朝上，咱们这里要转换成  原点在在窗口中间，y轴正半轴 朝上；
	p_ab.setX(x);
	p_ab.setY(y);
}



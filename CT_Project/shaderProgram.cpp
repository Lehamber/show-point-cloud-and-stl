#include "shaderProgram.h"

shaderProgram::shaderProgram(const char* vPath, const char* fPath)
{
	vertexPath = vPath;
	fragmentPath = fPath;
	init();
}

void shaderProgram::setShader(const char* vPath, const char* fPath)
{
	vertexPath = vPath;
	fragmentPath = fPath;
	init();
}

void shaderProgram::init()
{
	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile;
	ifstream fShaderFile;

	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	try
	{	//打开着色器文件 
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		stringstream vShaderStream;
		stringstream fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (ifstream::failure e)
	{
		cout << "ERROR::SHADER::FIEL_NOT_SUCCESSFULLY_READ" << endl;
	}
	// 将加入的着色器代码 转化成 标准的着色器代码形式
	const char * vShaderCode = vertexCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();

	//创建 顶点着色器 和 片段着色器
	QOpenGLShader verShader(QOpenGLShader::Vertex);
	QOpenGLShader fraShader(QOpenGLShader::Fragment);

	verShader.compileSourceCode(vShaderCode);
	fraShader.compileSourceCode(fShaderCode);

	ID = new QOpenGLShaderProgram();
	ID->addShader(&verShader);
	ID->addShader(&fraShader);
	ID->link();//  链接

}

void shaderProgram::use()
{
	ID->bind();// 将此着色器 绑定到当前的opengl上下文 之前绑定的都是失效
}
void shaderProgram::setUniformMat4(const char *name, const QMatrix4x4 trans) const
{
	ID->setUniformValue(name, trans);
}
void shaderProgram::setUniformFloat(const char *name, const GLfloat value) const
{
	ID->setUniformValue(name, value);
}
void shaderProgram::setUniformInt(const char *name, const int value) const
{
	ID->setUniformValue(name, value);
}
void shaderProgram::setUniformVec4(const char *name, const QVector4D value) const
{
	ID->setUniformValue(name, value);
}
void shaderProgram::setUniformVec3(const char *name, const QVector3D value) const
{
	ID->setUniformValue(name, value);
}

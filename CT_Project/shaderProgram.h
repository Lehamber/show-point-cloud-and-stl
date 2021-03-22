#pragma once

#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QVector4D>
#include <QVector3D>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class shaderProgram
{
private:
	const char * vertexPath;
	const char * fragmentPath;
public:

	QOpenGLShaderProgram *ID;

	shaderProgram(const char *vPath, const char* Path);

	void init();
	void setShader(const char *vPath, const char* fPath);
	void use();
	void setUniformInt(const char *name, const int value) const;
	void setUniformFloat(const char *name, const GLfloat value) const;
	void setUniformMat4(const char *name, const QMatrix4x4 trans) const;
	void setUniformVec4(const char *name, const QVector4D value) const;
	void setUniformVec3(const char *name, const QVector3D value) const;
};
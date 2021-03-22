#include "stdafx.h"
#include "camera.h"


QMatrix4x4 Camera::getViewMatrix()
{
	QMatrix4x4 matrix;
	matrix.lookAt(Position, Front, WorldUp);
	return matrix;
}
void Camera::processMouseScroll(float yoffset)
{
	yoffset *= scrollSensitivity;

	if (Position.z() < 0.00001f)
	{
		Position.setZ(0.00001f);
	}
	else
	{
		Position.setZ(Position.z() + yoffset);
	}
}
//void Camera::updateCameraVectors()
//{
//	 
//	QVector3D tempPos;
//	tempPos.setX(cos(Yaw)*cos(Pitch));
//	tempPos.setY(sin(Pitch));
//	tempPos.setZ(sin(Yaw)*cos(Pitch));
//
//	tempPos.normalize();
//
//	//Position.setX(tempPos.x * Radius);
//	//Position.setY(tempPos.y * Radius);
//	//Position.setZ(tempPos.z * Radius);
//
//	Position = tempPos * Radius;
//
//	QVector3D dirction = Position;
//	dirction.normalize();
//
//	WorldUp.setX(sin(Pitch)*(-cos(Yaw)));
//	WorldUp.setY(cos(Pitch));
//	WorldUp.setZ(sin(Pitch)*(-sin(Yaw)));
//
//	WorldUp.normalize();
//
//	Right = QVector3D::crossProduct(dirction, WorldUp);
//	Right.normalize();
//	Up = QVector3D::crossProduct(Right, dirction);
//	Up.normalize();
//}
//void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
//{
//	float velocity = MovementSpeed * deltaTime;
//	if (direction == FORWARD)
//		Position += Front * velocity;
//	if (direction == BACKWARD)
//		Position -= Front * velocity;
//	if (direction == LEFT)
//		Position -= Right * velocity;
//	if (direction == RIGHT)
//		Position += Right * velocity;
//}
//void Camera::processMouseMovement(float xoffset, float yoffset/*, GLboolean constrainPitch = true*/)
//{
//	xoffset *= MouseSensitivity;
//	yoffset *= MouseSensitivity;
//
//	Yaw += xoffset;
//	Pitch += yoffset;
//
//	/*	if (Pitch > 89.0f)
//			Pitch = 89.0f;
//		if (Pitch < -89.0f)
//			Pitch = -89.0f;*/
//	 
//	updateCameraVectors();
//}
 
//void Camera::processInput(float dt)
//{
//	if (keys[Qt::Key_W] == true)
//	{
//		Position += MovementSpeed * Front;
//	}
//	if (keys[Qt::Key_S] == true)
//	{
//		Position -= MovementSpeed * Front;
//	}
//	if (keys[Qt::Key_A] == true)
//	{
//		Position -= MovementSpeed * Right;
//	}
//	if (keys[Qt::Key_D] == true)
//	{
//		Position += MovementSpeed * Right;
//	}
//}
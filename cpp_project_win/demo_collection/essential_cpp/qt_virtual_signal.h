#pragma once
#include <iostream>

class G5ComCamera;
class DepthCamera;

class Camera_p
{
public:
	Camera_p(){}
	virtual ~Camera_p(){}
};

class DepthCamera : public Camera_p
{
public:
	DepthCamera() :Camera_p(){}
	virtual ~DepthCamera(){}
	virtual bool genCalibTable(int& a1, int& a2) { return true; }
};

class G5ComCamera : public DepthCamera
{
public:
	G5ComCamera() :DepthCamera(){}
	~G5ComCamera(){}
	bool genCalibTable(int& a1, int& a2)
	{ 
		std::cout << "G5ComCamera OK" << std::endl; 
		return true; 
	}

	void test()
	{
		int z1 = 1, z2 = 2;

		genCalibTable(z1, z2);
	}

};

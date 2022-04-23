#include<iostream>
using namespace std;

typedef struct point3d {
	float x;

};

class Point3d {
public:
	Point3d(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

private:
	float x, y, z;
};

int abc() {
	cout << "Hello World" << endl;
	int age;
	age = 0;
	age += 1;

	cout << age << endl;

	return 0;
}
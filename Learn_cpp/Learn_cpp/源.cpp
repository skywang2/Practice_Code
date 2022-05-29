#include<iostream>
#include<string>
using namespace std;
class Compute
{
public:
	int w, h;
	void init(int w_, int h_);
	int area();
	int perimeter();
};
void Compute::init(int w_, int h_) {
	w = w_; h = h_;
}
int Compute::area() {
	return w*h;
}
int Compute::perimeter() {
	return 2 * (w + h);
}

//void main() {
//	cout << "adsfa" << endl;
//	cout << "qqqqqq" << endl;
//	cout << "tttt" << endl;
//	cin.get();
//}
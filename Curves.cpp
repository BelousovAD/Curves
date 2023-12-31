﻿#define _USE_MATH_DEFINES
#include <iostream>

#include "circle.h"
#include "ellipse.h"
#include "helix.h"

#include <algorithm>
#include <numeric>
#include <random>
#include <vector>

#include <omp.h>

using namespace std;
using namespace curves;

shared_ptr<Curve> getShPtrRandomCurve() {
	mt19937_64 engine{ random_device{}() };
	uniform_int_distribution<> dist_i{ 0, 2 };
	uniform_real_distribution<> dist_r{ 0.0, 20.0 };
	switch (dist_i(engine)) {
	case 0:
		return make_shared<Circle>(dist_r(engine));
	case 1:
		return make_shared<Ellipse>(dist_r(engine), dist_r(engine));
	case 2:
		return make_shared<Helix>(dist_r(engine), dist_r(engine));
	}
}

int main()
{
	const size_t SIZE_1 = 10;
	// 2
	vector<shared_ptr<Curve>> curves;
	curves.reserve(SIZE_1);
	for (int i = 0; i < curves.capacity(); ++i) {
		try {
			curves.push_back(getShPtrRandomCurve());
		}
		catch (const exception& exception) {
			cerr << "Exception: " << exception.what() << endl;
		}
	}
	// 3
	for (auto it = curves.cbegin(); it != curves.cend(); ++it) {
		Vector3d derivative = (**it).getDerivative(M_PI_4);
		Vector3d point = (**it).getPoint(M_PI_4);
		cout << "Point: { "
			<< point.getX() << ", "
			<< point.getY() << ", "
			<< point.getZ() << " }"
			<< endl;
		cout << "Derivative: { "
			<< derivative.getX() << ", "
			<< derivative.getY() << ", "
			<< derivative.getZ() << " }"
			<< endl << endl;
	}
	// 4
	vector<shared_ptr<Circle>> circles;
	circles.reserve(SIZE_1);
	try {
		for (auto it = curves.begin(); it != curves.end(); ++it) {
			if (typeid(**it) == typeid(Circle)) {
				circles.push_back(dynamic_pointer_cast<Circle>(*it));
				//cout << typeid(**it).name() << endl;
			}
		}
	}
	catch (const exception& exception) {
		cerr << "Exception: " << exception.what() << endl;
	}
	// 5
	/*for (auto& el : circles) {
		cout << (*el).getRadius() << endl;
	}*/
	sort(circles.begin(), circles.end(),
		[](const shared_ptr<Circle>& a, const shared_ptr<Circle>& b) {
			return (*a).getRadius() < (*b).getRadius();
		});
	/*cout << "Sorted" << endl;
	for (auto& el : circles) {
		cout << (*el).getRadius() << endl;
	}*/
	//6, 8
	double sum = 0;
	#pragma omp parallel for reduction(+: sum)
	for (int i = 0; i < circles.size(); ++i) {
		sum += (*circles[i]).getRadius();
	}
	cout << "Sum: " << sum << endl;
	return 0;
}

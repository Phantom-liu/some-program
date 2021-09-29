#pragma once
#include<iostream>
using namespace std;
//static防止链接时头文件被多个.c包含导致全局符号重定义
static int gcd(int a, int b) {
	return b == 0 ? a : gcd(b, a%b);
}
struct num {
	int numerator = 0;
	int denominator = 1;
	num(int a = 0) {
		numerator = a;
	}
	num(num &b) {
		numerator = b.numerator;
		denominator = b.denominator;
	}
	//右值拷贝
	num(num &&b) {
		numerator = b.numerator;
		denominator = b.denominator;
	}
	num& operator=(num b) {
		numerator = b.numerator;
		denominator = b.denominator;
		return *this;
	}
	num operator+(num b) {
		num res;
		if (b.denominator == denominator) {
			res.numerator = numerator + b.numerator;
			res.denominator = denominator;
		}
		else {
			int lcm = denominator * b.denominator / gcd(denominator, b.denominator);
			res.numerator = numerator * lcm / denominator + b.numerator*lcm / b.denominator;
			res.denominator = lcm;
		}
		int tmp = gcd(abs(res.numerator), res.denominator);
		res.numerator /= tmp;
		res.denominator /= tmp;
		return res;
	}
	num operator-(num b) {
		num res;
		res = (*this) + b.opposite();
		return res;
	}
	num operator*(num b) {
		num res;
		res.numerator = numerator * b.numerator;
		res.denominator = denominator * b.denominator;
		int tmp = gcd(abs(res.numerator), res.denominator);
		res.numerator /= tmp;
		res.denominator /= tmp;
		return res;
	}
	num operator/(num b) {
		num res;
		res = (*this)*b.inverse();
		return res;
	}
	num inverse() {				//倒数
		if (numerator == 0) {
			cout << "除0错误！" << endl;
			exit(-1);
		}
		num res;
		res.denominator = numerator;
		res.numerator = denominator;
		if (res.denominator < 0) {
			res.denominator = -res.denominator;
			res.numerator = -res.numerator;
		}
		return res;
	}
	num opposite() {			//相反数
		num res;
		res.numerator = -numerator;
		res.denominator = denominator;
		return res;
	}

};
//static防止链接时头文件被多个.c包含导致全局符号重定义
static ostream& operator<<(ostream &out, num n) {
	//cout << n.numerator << "/" << n.denominator;
	if (n.denominator == 1) {
		cout << n.numerator;
	}
	else {
		int ngcd = gcd(abs(n.numerator), n.denominator);
		n.numerator /= ngcd;
		n.denominator /= ngcd;
		if (n.denominator == 1)
			cout << n.numerator;
		else
			cout << n.numerator << "/" << n.denominator;
	}
	return out;
}
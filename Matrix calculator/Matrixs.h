#pragma once
#include<iostream>
#include "num.h"
using namespace std;
const int MAXSIZE = 10;
class Matrixs {
public:
	Matrixs();
	void init(int a[][MAXSIZE], int row, int col);
	void initB(int b[]);
	void initStatus();
	void clear();
	int getInversion();
	void printOrigin();
	void printSimplest();
	void printStandard();
	void printInversion();
	void printRank();
	void print(num a[][MAXSIZE], int row_size, int col_size);
private:
	num	origin[MAXSIZE][MAXSIZE];
	num rowSimplest[MAXSIZE][MAXSIZE];
	num standard[MAXSIZE][MAXSIZE];
	num inversion[MAXSIZE][MAXSIZE];	
	int row;
	int col;
	int rank;
	int solution;	//-1无解 0唯一 1无穷
	bool setB;
};

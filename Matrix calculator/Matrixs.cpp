#include "Matrixs.h"


Matrixs::Matrixs() :row(row), col(col), setB(false), solution(0) {}

void Matrixs::init(int a[][MAXSIZE], int row, int col)
{
	this->row = row;
	this->col = col;
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= col; j++) {
			origin[i][j].numerator = a[i][j];
		}
	}
}

void Matrixs::initB(int b[]) {
	for (int i = 1; i <= row; i++)
		origin[i][col + 1].numerator = b[i];
	setB = true;
}

void Matrixs::clear() {
	row = 0;
	col = 0;
	setB = false;
	solution = 0;
	for (int i = 1; i <= MAXSIZE; i++) {
		for (int j = 1; j <= MAXSIZE; j++) {
			origin[i][j].numerator = 0;
			origin[i][j].denominator = 1;
			rowSimplest[i][j].numerator = 0;
			rowSimplest[i][j].denominator = 1;
			standard[i][j].numerator = 0;
			standard[i][j].denominator = 1;
			inversion[i][j].numerator = 0;
			inversion[i][j].denominator = 1;
		}
	}
}

void Matrixs::initStatus() {
	cout << "行简化过程：" << endl;
	int coltmp = setB ? col + 1: col;
	int curRow = 1;
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= coltmp; j++) {
			rowSimplest[i][j].numerator = origin[i][j].numerator;
		}
	}
	for (int j = 1; j <= col&&curRow <= row; j++) {
		if (rowSimplest[curRow][j].numerator == 0) {
			bool allZero = true;
			//当前行首元素为0时向下找寻第一个首非0行并交换两行
			for (int i = curRow + 1; i <= row; i++) {
				if (rowSimplest[i][j].numerator != 0) {
					for (int k = j; k <= coltmp; k++) {
						num tmp = rowSimplest[i][k];
						rowSimplest[i][k] = rowSimplest[curRow][k];
						rowSimplest[curRow][k] = tmp;
					}
					allZero = false;
					break;
				}
			}
			if (allZero)continue;
		}
		num tmp;
		tmp = rowSimplest[curRow][j].inverse();
		rowSimplest[curRow][j].numerator = 1;
		rowSimplest[curRow][j].denominator = 1;
		for (int k = j + 1; k <= coltmp; k++) {
			rowSimplest[curRow][k] = rowSimplest[curRow][k] * tmp;
		}
		for (int i = 1; i <= row; i++) {
			if (i == curRow)
				continue;
			num rowij = rowSimplest[i][j];
			if (rowij.numerator == 0)
				continue;
			rowSimplest[i][j].numerator = 0;
			rowSimplest[i][j].denominator = 1;
			for (int k = j + 1; k <= coltmp; k++) {
				rowSimplest[i][k] = rowSimplest[i][k] - rowSimplest[curRow][k] * rowij;
			}
		}
		cout << "Step#" << curRow << ": " << endl;
		print(rowSimplest, row, coltmp);
		curRow++;
	}
	rank = curRow - 1;
	if (setB) {
		int Brow;
		//判断增广矩阵的秩
		for (int i = curRow; i <= row; i++) {
			if (rowSimplest[i][col + 1].numerator != 0)
				solution = -1;
		}
		if (solution != -1)
			solution = rank < col ? 1 : 0;
	}
	//标准型
	for (int i = 1; i <= rank; i++) {
		standard[i][i] = 1;
	}
}


int Matrixs::getInversion() {
	if (rank < row || rank < col) {
		return -1;
	}
	cout << "初等变换过程：" << endl;
	num helpM[MAXSIZE][MAXSIZE << 1];
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= col; j++) {
			helpM[i][j] = origin[i][j];
		}
		helpM[i][col + i].numerator = 1;
	}
	int coltmp = col << 1;
	int curRow = 1;
	for (int j = 1; j <= col && curRow <= row; j++) {
		if (helpM[curRow][j].numerator == 0) {
			bool allZero = true;
			//当前行首元素为0时向下找寻第一个首非0行并交换两行
			for (int i = curRow + 1; i <= row; i++) {
				if (helpM[i][j].numerator != 0) {
					for (int k = j; k <= coltmp; k++) {
						num tmp = helpM[i][k];
						helpM[i][k] = helpM[curRow][k];
						helpM[curRow][k] = tmp;
					}
					allZero = false;
					break;
				}
			}
			if (allZero) {
				return -1;
			}
		}
		num tmp;
		tmp = helpM[curRow][j].inverse();
		helpM[curRow][j].numerator = 1;
		helpM[curRow][j].denominator = 1;
		for (int k = j + 1; k <= coltmp; k++) {
			helpM[curRow][k] = helpM[curRow][k] * tmp;
		}
		for (int i = 1; i <= row; i++) {
			if (i == curRow)
				continue;
			num rowij = helpM[i][j];
			if (rowij.numerator == 0)
				continue;
			helpM[i][j].numerator = 0;
			helpM[i][j].denominator = 1;
			for (int k = j + 1; k <= coltmp; k++) {
				helpM[i][k] = helpM[i][k] - helpM[curRow][k] * rowij;
			}
		}
		cout << "Step#" << curRow << ": " << endl;
		for (int i = 1; i <= row; i++) {
			for (int k = 1; k <= coltmp; k++) {
				cout << helpM[i][k] << "\t";
			}
			cout << endl;
		}
		curRow++;
	}
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= col; j++) {
			inversion[i][j] = helpM[i][j + col];
		}
	}
	return 1;
}

void Matrixs::printOrigin() {
	cout << "原矩阵：" << endl;
	print(origin, row, col);
}
void Matrixs::printSimplest() {
	cout << "行最简矩阵：" << endl;
	print(rowSimplest, row, col);
}
void Matrixs::printStandard() {
	cout << "标准型：" << endl;
	print(standard, row, col);
}
void Matrixs::printInversion() {
	cout << "逆矩阵为：" << endl;
	print(inversion, row, col);
}
void Matrixs::printRank() {
	cout << "矩阵的秩为：" << rank << endl;
}

void Matrixs::print(num a[][MAXSIZE], int row_size, int col_size) {
	for (int i = 1; i <= row_size; i++) {
		for (int j = 1; j <= col_size; j++) {
			cout << a[i][j] << "\t";
		}
		cout << endl;
	}
}

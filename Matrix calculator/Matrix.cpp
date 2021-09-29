#include "Matrix.h"
#include<string>

Matrix::Matrix() :row(row), col(col), setB(false), solution(0) {}

void Matrix::init(int a[][MAXSIZE], int row, int col)
{
	this->row = row;
	this->col = col;
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= col; j++) {
			origin[i][j].numerator = a[i][j];
		}
	}
}

void Matrix::initB(int b[]) {
	for (int i = 1; i <= row; i++)
		origin[i][col + 1].numerator = b[i];
	setB = true;
}

void Matrix::clear() {
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

void Matrix::initStatus() {
	cout << "行简化过程：" << endl;
	int coltmp = setB ? col + 1 : col;
	int curRow = 1;
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= coltmp; j++) {
			rowSimplest[i][j].numerator = origin[i][j].numerator;
		}
	}
	cout << "Step#0" << ": " << endl;
	print(rowSimplest, row, coltmp);
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


int Matrix::getInversion() {
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
	cout << "Step#0" << ": " << endl;
	for (int i = 1; i <= row; i++) {
		for (int k = 1; k <= coltmp; k++) {
			cout << helpM[i][k] << "\t";
		}
		cout << endl;
	}
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

void Matrix::printSolution() {
	if (solution == -1) {
		cout << "该方程组无解" << endl;
	}
	else if (solution == 1) {
		cout << "该方程组有无穷组解，可表示为" << endl;
		string ans[MAXSIZE];
		int ansSet[MAXSIZE] = { 0 };
		for (int i = row; i >= 1; i--) {
			int j;
			for (j = i; j <=col; j++) {
				if (rowSimplest[i][j].numerator == 1)
					break;
			}
			ansSet[j] = 1;
			for (int k = j + 1; k <= col; k++) {
				if (rowSimplest[i][k].numerator != 0) {
					if (rowSimplest[i][k].denominator != 1) {
						ans[j] += "(" + to_string(-rowSimplest[i][k].numerator) + "/"
							+ to_string(rowSimplest[i][k].denominator)
							+ "x" + to_string(k) + ")+";
					}
					else if(abs(rowSimplest[i][k].numerator) != 1){
						ans[j] += "(" + to_string(-rowSimplest[i][k].numerator) 
							+ "x" + to_string(k) + ")+";
					}
					else {
						if (rowSimplest[i][k].numerator == 1)
							ans[j] += "(-x" + to_string(k) + ")+";
						else
							ans[j] += "(x" + to_string(k) + ")+";
					}
				}
			}
			//最右边一列
			if (rowSimplest[i][col + 1].numerator != 0) {
				if (rowSimplest[i][col + 1].denominator != 1) {
					ans[j] += to_string(rowSimplest[i][col + 1].numerator) + "/"
						+ to_string(rowSimplest[i][col + 1].denominator);
				}
				else {
					ans[j] += to_string(rowSimplest[i][col + 1].numerator);
				}
			}
			else {
				if (!ans[j].empty() && ans[j][ans[j].length() - 1] == '+') {
					ans[j][ans[j].length() - 1] = '\0';
				}
			}
		}
		for (int i = 1; i <= col; i++) {
			if (ans[i].empty() && ansSet[i] == 0)
				cout << "x" << i << ": " << "(x" << i << ")" << endl;
			else {
				cout << "x" << i << ": ";
				if (ans[i].empty())
					cout << " " << 0 << endl;
				else
					cout << ans[i] << endl;
			}
		}
		cout << "自由未知量：";
		int cnt = 0;
		int freePos[MAXSIZE] = { 0 };
		for (int i = 1; i <= col; i++) {
			if (!ansSet[i]) {
				freePos[++cnt] = i;
				cout << "x" << i << " ";
			}
		}
		cout << endl;
		bool isnStar = false;
		for (int i = 1; i <= row; i++) {
			if (rowSimplest[i][col + 1].numerator != 0) {
				isnStar = true;
				break;
			}
		}
		num nStar[MAXSIZE];		//特解
		for (int i = 1; i <= row; i++)nStar[i] = rowSimplest[i][col + 1];
		num n[MAXSIZE][MAXSIZE];		//n[i]为第i个解向量
		for (int i = 1; i <= cnt; i++) {
			int valid = freePos[i];
			int curCol = 1;
			for (int j = 1; j <= col; j++) {
				//如果x(j)本身是自由未知量
				if (ansSet[j] == 0) {
					if (j == valid)
						n[i][j] = 1;
					else
						n[i][j] = 0;
					continue;
				}
				n[i][j] = rowSimplest[curCol][valid].opposite();
				curCol++;
			}
		}
		cout << "基础解系为：";
		if (isnStar) {
			cout << "(n*)+";
		}
		for (int i = 1; i < cnt; i++) {
			cout << "k" << i << "(n" << i << ")+";
		}
		cout << "k" << cnt << "(n" << cnt << ")" << endl;
		if (isnStar) {
			cout << "n*" << ": (";
			for (int j = 1; j <= col; j++) {
				cout << nStar[j] << ",";
			}
			cout << ")" << endl;
		}
		for (int i = 1; i <= cnt; i++) {
			cout << "n" << i << ": (";
			for (int j = 1; j < col; j++) {
				cout << n[i][j] << ",";
			}
			cout << n[i][col] << ")" << endl;
		}
	}
	else {
		cout << "唯一解为：" << endl;
		for (int i = 1; i <= row; i++) {
			cout << "x" << i << " :\t" << rowSimplest[i][col + 1] << endl;
		}
	}
}

void Matrix::printOrigin() {
	cout << "原矩阵：" << endl;
	print(origin, row, col);
}
void Matrix::printSimplest() {
	cout << "行最简矩阵：" << endl;
	print(rowSimplest, row, col);
}
void Matrix::printStandard() {
	cout << "标准型：" << endl;
	print(standard, row, col);
}
void Matrix::printInversion() {
	cout << "逆矩阵为：" << endl;
	print(inversion, row, col);
}
void Matrix::printRank() {
	cout << "矩阵的秩为：" << rank << endl;
}

void Matrix::print(num a[][MAXSIZE], int row_size, int col_size) {
	for (int i = 1; i <= row_size; i++) {
		for (int j = 1; j <= col_size; j++) {
			cout << a[i][j] << "\t";
		}
		cout << endl;
	}
}

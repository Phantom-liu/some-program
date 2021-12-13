#include<iostream>
using namespace std;
#include "Matrix.h"

Matrix m;
int a[MAXSIZE][MAXSIZE];
int row, col, tmp;

void showMenu() {
	cout << "欢迎使用矩阵计算器，输入0 - 9继续\n注：矩阵(和列向量)输入完成后使用命令0初始化\n" << endl;
	cout << "  ---------------------- " << endl;
	cout << " | 1.输入(更新)矩阵\t|" << endl;
	cout << " | 2.输入(更新)列向量b\t|" << endl;
	cout << " | 3.打印行最简矩阵\t|" << endl;
	cout << " | 4.打印行标准型矩阵\t|" << endl;
	cout << " | 5.打印线性方程组的解\t|" << endl;
	cout << " | 6.打印矩阵的秩\t|" << endl;
	cout << " | 7.打印逆矩阵\t\t|" << endl;
	cout << " | 8.清屏\t\t|" << endl;
	cout << " | 9.打印菜单\t\t|" << endl;
	cout << " | 0.更新状态\t\t|" << endl;
	cout << " | q.退出\t\t|" << endl;
	cout << "  ---------------------- " << endl;
}
int main()
{
	showMenu();
	char c;
	while (cin >> c && c != 'q') {
		cout << "Instruction_Start______________________________________\n" << endl;
		if ('3' <= c && c <= '7') {
			if (m.getRank() == -1) {
				cout << "请先使用命令0更新状态！" << endl;
				cout << "\n________________________________________Instruction_End\n\n" << endl;
				continue;
			}
		}
		switch (c) {
		case '1':
			m.clear();
			cout << "输入矩阵的行数和列数" << endl;
			cin >> row >> col;
			cout << "输入矩阵" << endl;
			for (int i = 1; i <= row; i++) {
				for (int j = 1; j <= col; j++) {
					cin >> a[i][j];
				}
			}
			m.init(a, row, col);
			cout << "输入完成！" << endl;
			break;
		case '2':
			cout << "请输入列向量" << endl;
			int b[MAXSIZE];
			for (int i = 1; i <= row; i++)
				cin >> b[i];
			m.initB(b);
			cout << "输入完成！" << endl;
			break;
		case '3':m.printSimplest(); break;
		case '4':m.printStandard(); break;
		case '5':m.printSolution(); break;
		case '6':
			cout << "矩阵的秩为：" << m.getRank() << endl;
			break;
		case '7':
			tmp = m.getInversion();
			if (tmp == -1)
				cout << "该矩阵不可逆！" << endl;
			else
				m.printInversion();
			break;
		case '8':
			system("cls");
			showMenu();
			break;
		case '9':showMenu(); break;
		case '0':m.initStatus(); break;
		default:cout << "输入有误！请重新输入指令" << endl;
		}
		cout << "\n________________________________________Instruction_End\n\n" << endl;
	}
	cout << "再见！" << endl;
	return 0;
}
/*
1
3 5
1 1 0 -2 1
-2 -1 1 -4 2
-1 1 -1 -2 1
2
-1 1 2
0
5
*/

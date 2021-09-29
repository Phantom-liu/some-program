#include<iostream>
using namespace std;
#include "Matrix.h"
void showMenu() {
	cout << "欢迎使用矩阵计算器，输入0 - 9继续：\n" << endl;
	cout << "  ---------------------- " << endl;
	cout << " | 1.输入(更新)矩阵\t|" << endl;
	cout << " | 2.输入(更新)列向量b\t|" << endl;
	cout << " | 3.打印行最简矩阵\t|" << endl;
	cout << " | 4.打印行标准型矩阵\t|" << endl;
	cout << " | 5.打印原矩阵\t\t|" << endl;
	cout << " | 6.打印矩阵的秩\t|" << endl;
	cout << " | 7.打印逆矩阵\t\t|" << endl;
	cout << " | 8.清屏\t\t|" << endl;
	cout << " | 9.打印菜单\t\t|" << endl;
	cout << " | 0.更新状态\t\t|" << endl;
	cout << " | q.退出\t\t|" << endl;
	cout << "  ---------------------- " << endl;
}
/*
1
4 5
2 1 8 3 7
2 -3 0 7 -5
3 -2 5 8 0
1 0 3 2 0
*/
int main()
{
	showMenu();
	int n, row, col, tmp;
	int a[MAXSIZE][MAXSIZE];
	Matrix m;
	while (cin >> n && n != 'q') {
		cout << "Instruction_Start______________________________________" << endl;
		switch (n) {
		case 1:
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
			cout << "输入完成！请输入下一条指令" << endl;
			break;
		case 2:
			cout << "请输入列向量" << endl;
			int b[MAXSIZE];
			for (int i = 1; i <= row; i++)
				cin >> b[i];
			m.initB(b);
			cout << "输入完成！" << endl;
			break;
		case 3:m.printSimplest(); break;
		case 4:m.printStandard(); break;
		case 5:m.printOrigin(); break;
		case 6:m.printRank(); break;
		case 7:
			tmp = m.getInversion();
			if (tmp == -1)
				cout << "该矩阵不可逆！" << endl;
			else
				m.printInversion();
			break;
		case 8:
			system("cls");
			showMenu();
			break;
		case 9:showMenu(); break;
		case 0:m.initStatus(); break;
		default:cout << "输入有误！请重新输入指令" << endl;
		}
		cout << "________________________________________Instruction_End\n\n" << endl;
	}
	cout << "再见！" << endl;
	return 0;;
}
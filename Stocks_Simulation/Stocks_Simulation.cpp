#include "TradeManager.h"
#include <iostream>
using namespace std;

TradeManager m;

void showMenu() {
	cout << "1.展示交易存档列表" << endl
		<< "2.添加一条交易存档" << endl
		<< "3.删除一个交易存档" << endl
		<< "4.进入一个交易存档(序号或名称)" << endl
		<< "0.退出" << endl;
}

void addTrade() {
	string name, brief;
	double iniMoney;
	cout << "输入名称" << endl;
	cin >> name;
	cout << "输入简述(以两次回车结束)" << endl;
	string tmp;
	cin.ignore();
	do {
		getline(cin, tmp);
		brief += tmp;
	} while (!tmp.empty());
	cout << "输入初始资金" << endl;
	cin >> iniMoney;
	m.addTrade(name, brief, iniMoney);
	cout << "添加完毕！" << endl;
}

void getInTheTrade() {
	TradeEntry *trade;
	cout << "输入序号或名称" << endl;
	char c;
	int id;
	string name;
	cin.ignore();
	c = cin.peek();
	if ((c >= '0') && (c <= '9')) {
		cin >> id;
		trade = m.getTrade(id);
	}
	else {
		cin >> name;
		trade = m.getTrade(name);
	}
	if (!trade) {
		cout << "找不到存档！" << endl;
		return;
	}
	cout << "你已进入交易存档" << trade->name << endl;
	cout << "1.添加一条记录" << endl
		<< "2.删除上一条记录" << endl
		<< "3.打印简述" << endl
		<< "4.打印操作清单" << endl
		<< "0.退出当前存档" << endl;
	while (1) {
		int tmp;
		cin >> tmp;
		if (!tmp) {
			trade->printResThisTime();
			break;
		}
		switch (tmp) {
		case 1:	{
			cout << "请输入股票名称、清仓盈利率、仓位(RMB)" << endl;
			string stock;
			double rate, positions;
			cin >> stock >> rate >> positions;
			trade->addEntry(stock, rate, positions);
			break;
			}
		case 2:
			trade->deleteEntry();
			break;
		case 3:
			trade->printBrief();
			break;
		case 4:
			trade->printList();
			break;
		default:
			cout << "范围有误！" << endl;
		}
	}
}

int main() {
	m.iniList();
	showMenu();
	while (1) {
		int tmp;
		cin >> tmp;
		if (!tmp) {
			m.updateList();
			break;
		}
		switch (tmp) {
		case 1:
			m.showList();
			break;
		case 2:
			addTrade();
			break;
		case 3:
			cout << "输入要删除存档的序号" << endl;
			int id;
			cin >> id;
			m.deleteTrade(id);
			break;
		case 4:
			getInTheTrade();
			break;
		default:
			cout << "范围有误！" << endl;
		}
	}
	cout << "再见！" << endl;
	Sleep(1);
	return 0;
}
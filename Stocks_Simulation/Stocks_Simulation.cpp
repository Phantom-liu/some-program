#include "TradeManager.h"
#include <iostream>
using namespace std;

TradeManager manager;

void showMenu() {
	cout << "1.展示交易存档列表" << endl
		<< "2.添加一条交易存档" << endl
		<< "3.删除一个交易存档" << endl
		<< "4.进入一个交易存档(序号或名称)" << endl
		<< "m.展示主菜单" << endl
		<< "q.退出" << endl << endl;
}


void addTrade() {
	string name, brief;
	double iniMoney;
	cout << "输入名称" << endl;
	cin >> name;
	cout << "\n输入简述(以两次回车结束)" << endl;
	string tmp;
	cin.ignore();
	do {
		getline(cin, tmp);
		brief += tmp;
		brief += "\n";
	} while (!tmp.empty());
	cout << "输入初始资金" << endl;
	cin >> iniMoney;
	manager.addTrade(name, brief, iniMoney);
	cout << "存档添加完毕！" << endl;
}


//进入后的交易操作----------------------------------
void recordSortInTrade(TradeEntry *trade) {
	cout << "1.按盈利率升序  \t\t2.按盈利率降序" << endl
		<< "3.按盈利数目升序\t\t4.按盈利数目降序" << endl
		<< "5.按时间升序    \t\t6.按时间降序\n" << endl;
	char c;
Sort:
	cin >> c;
	switch (c) {
	case'1':
		trade->sortByRate(1);
		break;
	case'2':
		trade->sortByRate(-1);
		break;
	case'3':
		trade->sortByProfit(1);
		break;
	case'4':
		trade->sortByProfit(-1);
		break;
	case'5':
		trade->sortByTime(1);
		break;
	case'6':
		trade->sortByTime(-1);
		break;
	default:
		cout << "范围有误请重新输入(1 - 6)！\n" << endl;
		goto Sort;
	}
	cout << "排序完毕！" << endl;
}

void tradeMenu() {
	cout << "1.添加一条记录" << endl
		<< "2.删除上一条记录" << endl
		<< "3.打印交易概要" << endl
		<< "4.打印操作清单" << endl
		<< "5.排序" << endl
		<< "m.展示交易菜单" << endl
		<< "q.退出当前存档" << endl << endl;
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
		trade = manager.getTrade(id);
	}
	else {
		cin >> name;
		trade = manager.getTrade(name);
	}
	if (!trade) {
		cout << "找不到存档！" << endl;
		return;
	}
	cout << "你已进入交易存档" << trade->name << endl << endl;
	tradeMenu();
	trade->printInfo();
	while (1) {
		char tmp;
		cin >> tmp;
		if (tmp == 'q') {
			trade->printResThisTime();
			break;
		}
		SET_FORE_BRIWHITE();
		cout << "Trade_Operation_Start------------------------------------------------------------------------------------\n" << endl;
		SET_FORE_WHITE();
		switch (tmp) {
		case '1': {
			cout << "请输入股票名称、清仓盈利率、仓位(RMB)" << endl;
			string stock;
			double rate, positions;
			cin >> stock >> rate >> positions;
			if (positions < 0 && positions != -1) {
				cout << "输入有误！" << endl;
				break;
			}
			if (positions == -1)
				positions = trade->money;
			trade->addEntry(stock, rate, positions);
			double changes = positions * rate / 100;
			cout << "盈亏: ";
			if (rate > 0) {
				SET_FORE_RED();
				cout << changes << endl;
				SET_FORE_WHITE();
			}
			else if (rate < 0) {
				SET_FORE_GREEN();
				cout << changes << endl;
				SET_FORE_WHITE();
			}
			else {
				cout << changes << endl;
			}
			cout << "余额: " << trade->money << endl;
			break;
		}
		case '2':
			try {
				trade->deleteEntry();
			}
			catch (int e) {
				break;
			}
			cout << "已删除上条记录！" << endl;
			break;
		case '3':
			trade->printInfo();
			break;
		case '4':
			trade->printList();
			break;
		case '5':
			recordSortInTrade(trade);
			break;
		case'm':
			tradeMenu();
			break;
		default:
			cout << "范围有误请重新输入！" << endl;
		}
		SET_FORE_BRIWHITE();
		cout << "\n--------------------------------------------------------------------------------------Trade-Operation-End\n" << endl;
		SET_FORE_WHITE();
	}
}
//--------------------------------------------------
int main() {
	manager.iniList();
	showMenu();
	while (1) {
		char tmp;
		cin >> tmp;
		if (tmp == 'q') {
			manager.updateList();
			break;
		}
		SET_FORE_BLUE();
		cout << "Instruction_Start______________________________________\n" << endl;
		SET_FORE_WHITE();
		switch (tmp) {
		case '1':
			manager.showList();
			break;
		case '2':
			addTrade();
			break;
		case '3': {
			cout << "输入要删除存档的序号" << endl;
			int id;
			cin >> id;
			string name = manager.getTrade(id)->name;
			manager.deleteTrade(id);
			cout << "存档" << name << "已删除！" << endl;
			break;
		}
		case '4':
			getInTheTrade();
			break;
		case 'm':
			showMenu();
			break;
		default:
			cout << "范围有误请重新输入！" << endl;
		}
		SET_FORE_BLUE();
		cout << "\n________________________________________Instruction_End\n\n" << endl;
		SET_FORE_WHITE();
	}
	cout << "再见！" << endl;
	Sleep(1);
	return 0;
}
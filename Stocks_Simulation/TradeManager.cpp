#include "TradeManager.h"

TradeEntry::TradeEntry() {
	money = 20000;
	iniMoney = 20000;
	yieldRate = 0;
	moneyThisTime = 20000;
	rateThisTime = 0;
}

TradeEntry::TradeEntry(string name, string brief, double iniMoney) {
	this->name = name;
	this->brief = brief;
	this->iniMoney = iniMoney;
	this->money = iniMoney;
	this->yieldRate = 0;
	this->moneyThisTime = iniMoney;
	this->rateThisTime = 0;
	this->fileName = name + ".txt";
}

void TradeEntry::addEntry(string stock, double rate, double positions = -1) {
	RecordEntry rec;
	rec.stock = stock;
	rec.rate = rate;
	if (positions == -1) {
		positions = money;
	}
	rec.positions = positions;
	rec.profit = positions * rate / 100;
	money += rec.profit;
	UPDATE_YIELDRATE();
	UPDATE_THISTIME_RATE();
	SYSTEMTIME t;
	GetLocalTime(&t);
	rec.date = to_string(t.wYear) + "-" + to_string(t.wMonth) + "-" + to_string(t.wDay);
	rec.time = to_string(t.wHour) + ":" + to_string(t.wMinute);
	recordList.push_back(rec);
}

void TradeEntry::deleteEntry() {
	money -= recordList.back().profit;
	UPDATE_YIELDRATE();
	UPDATE_THISTIME_RATE();
	recordList.pop_back();
}

void TradeEntry::readFile(string filename) {
	/*
	名称：
	简述：

	资金：
	盈利率：
	初始资金：

	东方证券		20000		+3.6		+720		2021-12-12	17:31
	三一重工		10000		-3.7		-370		2021-12-12	18:00
	...

	*/
	fstream preTrade(filename, ios::in);
	string tmp;
	preTrade >> tmp >> name;
	preTrade >> tmp;
	getline(preTrade, tmp);
	//多行简述
	do {
		getline(preTrade, tmp);
		brief += tmp;
	} while (!tmp.empty());

	preTrade >> tmp >> money;
	preTrade >> tmp >> yieldRate;
	preTrade >> tmp >> iniMoney;
	moneyThisTime = money;
	rateThisTime = 0;
	fileName = filename;
	string record;
	while (!preTrade.eof()) {
		getline(preTrade, record);
		if (record.empty()) {
			continue;
		}
		stringstream line(record);
		RecordEntry rec;
		line >> rec.stock >> rec.positions >> rec.rate >> rec.profit >> rec.date >> rec.time;
		recordList.push_back(rec);
	}
	preTrade.close();
}

void TradeEntry::writeBack() {
	/*
	名称：
	简述：

	资金：
	盈利率：
	初始资金：

	东方证券		20000		+3.6		+720		2021-12-12	17:31
	三一重工		10000		-3.7		-370		2021-12-12	18:00
	...

	*/
	fstream output(fileName, ios::out);
	output << "名称: " << name << endl
		<< "简述: \n" << brief << endl << endl
		<< "资金: " << money << endl
		<< "盈利率: " << yieldRate << endl
		<< "初始资金: " << iniMoney << endl << endl;
	for (auto rec : recordList) {
		output << rec.stock;
		char stmp[100];
		sprintf(stmp,"\t%9.1f\t%+9.1f\t%+9.1f\t", rec.positions, rec.rate, rec.profit);
		output << stmp;
		output << rec.date << " " << rec.time << endl;
	}
	output.close();
}

void TradeEntry::printBrief() {
	cout << "方式简述" << endl;
	cout << brief << endl;
}

void TradeEntry::printInfo() {
	cout << "资金：" << money << endl
		<< "此次总盈利：" << money - moneyThisTime << endl
		<< "盈利率" << rateThisTime << endl;
	cout << endl;
}

void TradeEntry::printList() {
	cout << "名称: " << name << endl
		<< "简述: \n" << brief << endl << endl
		<< "资金: " << money << endl
		<< "盈利率: " << yieldRate << endl
		<< "初始资金: " << iniMoney << endl << endl;
	cout << "股票\t  仓位\t\t     盈利率\t    盈利\t日期" << endl;
	for (auto rec : recordList) {
		cout << rec.stock;
		printf("\t%9.1f\t%+9.2f\t%+9.1f\t", rec.positions, rec.rate, rec.profit);
		cout << rec.date << " " << rec.time << endl;
	}
}

void TradeEntry::printResThisTime() {
	printf("本次盈亏: %+.1f\n盈利率: %.2f\n\n", money - moneyThisTime, rateThisTime);
}

void TradeEntry::sortByRate(){}
void TradeEntry::sortByProfit(){}
void TradeEntry::sortByTime(){}
 

void TradeManager::iniList() {
	fstream nameList("nameList.txt", ios::in);
	if (!nameList.is_open()) {
		return;
	}
	while (!nameList.eof()) {
		string filename;
		nameList >> filename;
		if (filename.empty()) {
			continue;
		}
		TradeEntry* trade = new TradeEntry();
		trade->readFile(filename);
		int pos = filename.find(".txt");
		filename.erase(pos, 4);
		tradeList.push_back(filename);
		m[filename] = trade;
	}
	nameList.close();
}

void TradeManager::addTrade(string name, string brief, double iniMoney) {
	if (m[name] != nullptr) {
		cout << "该名称已被使用！" << endl;
		return;
	}
	TradeEntry *trade = new TradeEntry(name, brief, iniMoney);	
	tradeList.push_back(name);
	m[name] = trade;
}

void TradeManager::deleteTrade(int id) {
	if (id < 1 || id > tradeList.size()) {
		cout << "请输入正确的序号！" << endl;
		return;
	}
	string name = tradeList[id - 1];
	delete m[name];
	m.erase(name);
	name += ".txt";
	fstream fs(name, ios::in);
	int isexist = 0;
	if (fs.is_open()) {
		isexist = 1;
	}
	if (isexist) {
		fs.close();
		system(("del "+name).c_str());
	}
	tradeList.erase(tradeList.begin() + id - 1);
}

TradeEntry* TradeManager::getTrade(int id){
	if (id < 1 || id > tradeList.size()) {
		return nullptr;
	}
	return m[tradeList[id - 1]];
}

TradeEntry* TradeManager::getTrade(string name){
	return m[name];
}

void TradeManager::showList(){
	int cnt = 1;
	cout << "交易列表：" << endl;
	for (auto s : tradeList) {
		cout << cnt++ << ". " << s << endl;
	}
	cout << endl;
}

void TradeManager::updateList() {
	fstream nameList("nameList.txt", ios::out);
	for (auto trade : m) {
		trade.second->writeBack();
		nameList << trade.second->fileName << endl;
	}

}




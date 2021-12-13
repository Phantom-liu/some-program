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
	���ƣ�
	������

	�ʽ�
	ӯ���ʣ�
	��ʼ�ʽ�

	����֤ȯ		20000		+3.6		+720		2021-12-12	17:31
	��һ�ع�		10000		-3.7		-370		2021-12-12	18:00
	...

	*/
	fstream preTrade(filename, ios::in);
	string tmp;
	preTrade >> tmp >> name;
	preTrade >> tmp;
	getline(preTrade, tmp);
	//���м���
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
	���ƣ�
	������

	�ʽ�
	ӯ���ʣ�
	��ʼ�ʽ�

	����֤ȯ		20000		+3.6		+720		2021-12-12	17:31
	��һ�ع�		10000		-3.7		-370		2021-12-12	18:00
	...

	*/
	fstream output(fileName, ios::out);
	output << "����: " << name << endl
		<< "����: \n" << brief << endl << endl
		<< "�ʽ�: " << money << endl
		<< "ӯ����: " << yieldRate << endl
		<< "��ʼ�ʽ�: " << iniMoney << endl << endl;
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
	cout << "��ʽ����" << endl;
	cout << brief << endl;
}

void TradeEntry::printInfo() {
	cout << "�ʽ�" << money << endl
		<< "�˴���ӯ����" << money - moneyThisTime << endl
		<< "ӯ����" << rateThisTime << endl;
	cout << endl;
}

void TradeEntry::printList() {
	cout << "����: " << name << endl
		<< "����: \n" << brief << endl << endl
		<< "�ʽ�: " << money << endl
		<< "ӯ����: " << yieldRate << endl
		<< "��ʼ�ʽ�: " << iniMoney << endl << endl;
	cout << "��Ʊ\t  ��λ\t\t     ӯ����\t    ӯ��\t����" << endl;
	for (auto rec : recordList) {
		cout << rec.stock;
		printf("\t%9.1f\t%+9.2f\t%+9.1f\t", rec.positions, rec.rate, rec.profit);
		cout << rec.date << " " << rec.time << endl;
	}
}

void TradeEntry::printResThisTime() {
	printf("����ӯ��: %+.1f\nӯ����: %.2f\n\n", money - moneyThisTime, rateThisTime);
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
		cout << "�������ѱ�ʹ�ã�" << endl;
		return;
	}
	TradeEntry *trade = new TradeEntry(name, brief, iniMoney);	
	tradeList.push_back(name);
	m[name] = trade;
}

void TradeManager::deleteTrade(int id) {
	if (id < 1 || id > tradeList.size()) {
		cout << "��������ȷ����ţ�" << endl;
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
	cout << "�����б�" << endl;
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




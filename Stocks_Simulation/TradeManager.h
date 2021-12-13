#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <map>
#include <iomanip>

#define UPDATE_YIELDRATE() (yieldRate = (money - iniMoney) / iniMoney * 100)
#define UPDATE_THISTIME_RATE() (rateThisTime = (money - moneyThisTime) / moneyThisTime * 100)
#define SET_FORE_RED() SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED)
#define SET_FORE_GREEN() SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN)
#define SET_FORE_BLUE() SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE)
#define SET_BACK_BLUE() SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE)
#define SET_BACK_GREEN() SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN)
#define SET_FORE_WHITE() SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN)
#define SET_FORE_BRIWHITE() SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN)
using namespace std;


struct RecordEntry {
	string stock;
	double positions;
	double rate = 0;
	double profit = 0;
	string date;
	string time;
};


class TradeEntry {
public:
	string name;		//�µĽ���(��ʼ�ʽ����ã�
	string brief;		//�˴�ʹ�õĽ��׷�������
	vector<RecordEntry> recordList;	//ÿ�β���
	double money;		//��ǰ�ʽ�
	double iniMoney;	//��ʼ���ʽ�
	double yieldRate;	//ӯ����
	string fileName;	//�����¼ÿ�β������ļ�����(.txt)
	double moneyThisTime;	//��������ʱ���ʽ�
	double rateThisTime;	//�����������ڵ�Ӯ����
	TradeEntry();
	TradeEntry(string name, string brief, double iniMoney);
	//���һ�β��� positions���λ -1Ϊȫ��
	void addEntry(string stock, double rate, double positions);
	
	void deleteEntry();		//ɾ����һ�β���(��¼����)
	void readFile(string filename);	//���ļ������ڴ�
	void writeBack();		//д���ļ�(.txt)
	void printInfo();		
	void printList();		//��ӡ���в�����¼
	void printResThisTime();//��ӡ����ս��(�Ա�������)
	void sortByRate(int upOrDown);		//��ÿ�β�����ӯ�������� 1�� -1��
	void sortByProfit(int upOrDown);	//��ӯ����С���� 1�� -1��
	void sortByTime(int upOrDown);		//������ʱ�����򣨱���ʱ�䣩 1�� -1��
};


class TradeManager
{
private:
	vector<string> tradeList;	//���������ʽ�Ľ����б�
	map<string, TradeEntry*> m;
public:
	void iniList();
	void addTrade(string name,string brief,double iniMoney);
	void deleteTrade(int id);
	TradeEntry* getTrade(int id);
	TradeEntry* getTrade(string name);
	void showList();
	void updateList();
	~TradeManager() {
		for (auto trade : m) {
			delete trade.second;
		}
	}
};


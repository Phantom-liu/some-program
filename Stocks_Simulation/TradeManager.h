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

#define UPDATE_YIELDRATE() (yieldRate = (money - iniMoney) / iniMoney * 100)
#define UPDATE_THISTIME_RATE() (rateThisTime = (money - moneyThisTime) / moneyThisTime * 100)
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
	void printBrief();		
	void printInfo();		
	void printList();		//��ӡ���в�����¼
	void printResThisTime();//��ӡ����ս��(�Ա�������)
	void sortByRate();		//��ÿ�β�����ӯ��������
	void sortByProfit();	//��ӯ����С����
	void sortByTime();		//������ʱ�����򣨱���ʱ�䣩
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
};


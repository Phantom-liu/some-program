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
	string name;		//新的交易(初始资金重置）
	string brief;		//此次使用的交易方法简述
	vector<RecordEntry> recordList;	//每次操作
	double money;		//当前资金
	double iniMoney;	//初始的资金
	double yieldRate;	//盈亏率
	string fileName;	//负责记录每次操作的文件名称(.txt)
	double moneyThisTime;	//启动程序时的资金
	double rateThisTime;	//从启动到现在的赢亏率
	TradeEntry();
	TradeEntry(string name, string brief, double iniMoney);
	//添加一次操作 positions表仓位 -1为全仓
	void addEntry(string stock, double rate, double positions);
	
	void deleteEntry();		//删除上一次操作(记录有误)
	void readFile(string filename);	//从文件读入内存
	void writeBack();		//写回文件(.txt)
	void printInfo();		
	void printList();		//打印所有操作记录
	void printResThisTime();//打印操作战果(自本次启动)
	void sortByRate(int upOrDown);		//将每次操作按盈利率排序 1升 -1降
	void sortByProfit(int upOrDown);	//按盈利大小排序 1升 -1降
	void sortByTime(int upOrDown);		//按操作时间排序（本地时间） 1升 -1降
};


class TradeManager
{
private:
	vector<string> tradeList;	//所有重置资金的交易列表
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


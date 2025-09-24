#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
#include<chrono>

using namespace std;
using namespace std::chrono;

typedef struct my_str {
	char* str;
	int count;
} my_str;

class BinFile : fstream
{
public:
	BinFile(char*);
	~BinFile();
	void ToFileEnd(char*);  // ���������� 
	void ToFileInd(char*, int);  // ������� 
	void DeleteStr(int);   // ��������
	void Edit(char*, int);   //��������������
	void ToTXT(char*);  //������ ��������� ����� � ���������
	void FromTXT(char*);    //���������� ��������� ����� �� ����������
	void print(my_str*, ostream&);
	void printFile();
	void Extend();
private:
	fstream bin;
};
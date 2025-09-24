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
	void ToFileEnd(char*);  // добавление 
	void ToFileInd(char*, int);  // вставка 
	void DeleteStr(int);   // удаление
	void Edit(char*, int);   //редактирование
	void ToTXT(char*);  //запись бинарного файла в текстовый
	void FromTXT(char*);    //заполнение бинарного файла из текстового
	void print(my_str*, ostream&);
	void printFile();
	void Extend();
private:
	fstream bin;
};
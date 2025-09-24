#include "headers.h"

class BinFile;

int main()
{
	system("cls");
	setlocale(LC_ALL, "Russian");

	char* filename = new char[100];
	cout << "Введите название файла: " << endl;
	while (1)
	{
		cin >> filename;
		char* p = strstr(filename, ".txt");
		if (p) break;
		else
		{
			cout << "Попробуйте снова: ";
		}
	}
	char* BinName = (char*)"o.dat";
	BinFile my_file(BinName);
	my_file.FromTXT(filename);
	my_file.printFile();
	char* newelement = new char[100];
	int ind;
	
	while (1)
	{
		char* commandchar = new char[100];
		
		cout << "Выберите команду:" << endl;
		cout << "1 - добавление строки в конец файла" << endl;
		cout << "2 - вставка строки по индексу" << endl;
		cout << "3 - удaление строки по индексу" << endl;
		cout << "4 - редактирование строки" << endl;
		cout << "5 - вывести файл" << endl;
		cout << "6 - выйти" << endl;
		
		cin >> commandchar;
		switch (atoi(commandchar))
		{
		case 1:
			cout << "Введите новую строку: " << endl;
			scanf(" %99[^\n]", newelement);

			my_file.ToFileEnd(newelement);
			break;
		case 2:
			cout << "Введите новую строку: " << endl;
			scanf(" %99[^\n]", newelement);

			cout << "Введите индекс: " << endl;
			cin >> ind;
			my_file.ToFileInd(newelement, ind);
			break;
		case 3:
			cout << "Введите индекс: " << endl;
			cin >> ind;
			my_file.DeleteStr(ind);
			break;
		case 4:
			cout << "Введите новую строку: " << endl;
			scanf(" %99[^\n]", newelement);

			cout << "Введите индекс: " << endl;
			cin >> ind;
			my_file.Edit(newelement, ind);
			break;
		case 5:
			my_file.printFile();
			break;
		case 6:
			return 0;
		defoult:
			cout << "Попробуйте снова" << endl;
			break;
		}
	}
	my_file.ToTXT(filename);
	return 0;
}
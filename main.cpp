#include "headers.h"

class BinFile;

int main()
{
	system("cls");
	setlocale(LC_ALL, "Russian");

	char* filename = new char[100];
	cout << "������� �������� �����: " << endl;
	while (1)
	{
		cin >> filename;
		char* p = strstr(filename, ".txt");
		if (p) break;
		else
		{
			cout << "���������� �����: ";
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
		
		cout << "�������� �������:" << endl;
		cout << "1 - ���������� ������ � ����� �����" << endl;
		cout << "2 - ������� ������ �� �������" << endl;
		cout << "3 - ��a����� ������ �� �������" << endl;
		cout << "4 - �������������� ������" << endl;
		cout << "5 - ������� ����" << endl;
		cout << "6 - �����" << endl;
		
		cin >> commandchar;
		switch (atoi(commandchar))
		{
		case 1:
			cout << "������� ����� ������: " << endl;
			scanf(" %99[^\n]", newelement);

			my_file.ToFileEnd(newelement);
			break;
		case 2:
			cout << "������� ����� ������: " << endl;
			scanf(" %99[^\n]", newelement);

			cout << "������� ������: " << endl;
			cin >> ind;
			my_file.ToFileInd(newelement, ind);
			break;
		case 3:
			cout << "������� ������: " << endl;
			cin >> ind;
			my_file.DeleteStr(ind);
			break;
		case 4:
			cout << "������� ����� ������: " << endl;
			scanf(" %99[^\n]", newelement);

			cout << "������� ������: " << endl;
			cin >> ind;
			my_file.Edit(newelement, ind);
			break;
		case 5:
			my_file.printFile();
			break;
		case 6:
			return 0;
		defoult:
			cout << "���������� �����" << endl;
			break;
		}
	}
	my_file.ToTXT(filename);
	return 0;
}
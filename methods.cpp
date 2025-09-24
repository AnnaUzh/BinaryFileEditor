#include "headers.h"

// ����� ������ � ������������ �����
void BinFile::print(my_str* str, ostream& stream) {
    for (int i = 0; i < str->count; i++) {
        stream << str->str[i];
    }
    stream << endl;
}
// ���������� � ����������
BinFile::BinFile(char* nam) { 
    bin.open(nam, ios::binary | ios::in | ios::out);

    int cap = 2, cnt = 0;
    int shift = 3 * sizeof(int);
    // ������ ����������� ���������� � ������ �����
    bin.write((char*)&cap, sizeof(cap));
    bin.write((char*)&cnt, sizeof(cnt));
    bin.write((char*)&shift, sizeof(shift));
}
BinFile::~BinFile() { bin.close(); }

void BinFile::Extend() {
    int shift, len;

    // �������� �������� ��������� �� ��� int � ��������� �����
    bin.seekg(2 * sizeof(int), ios::beg);
    bin.read((char*)&shift, sizeof(shift));

    //�������� �������� ��������� � ������ � ��������� ������������ ���������� ���������
    bin.seekg(ios::beg);
    bin.read((char*)&len, sizeof(len));

    int old_shift = shift, old_len = len;
    // ���������� ������� � 2 ����
    len *= 2;
    shift += old_len * 8;

    bin.seekg(old_shift, ios::beg);
    // ����������� ��������� � ����� ������
    while (bin.tellg() != shift) {
        my_str* buf;
        bin.read((char*)&buf, sizeof(buf));
        bin.seekg((old_len - 1) * 8, ios::cur);
        bin.write((char*)&buf, sizeof(buf));
        bin.seekg(-old_len * 8, ios::cur);
    }
    // ������ ����� shift � len  � ����
    bin.seekg(2 * sizeof(int), ios::beg);
    bin.write((char*)&shift, sizeof(shift));

    bin.seekg(ios::beg);
    bin.write((char*)&len, sizeof(len));
}


// ���������� ������ � ����� �����
void BinFile::ToFileEnd(char* newelem)
{
    int shift, num, len; 
    // �������� �������� ��������� �� ���� int �� ������ � ��������� ���������� ���������
    bin.seekg(1 * sizeof(int), ios::beg);
    bin.read((char*)&num, sizeof(num));

    // �������� �������� ��������� �� ��� int � ��������� �����
    bin.seekg(2 * sizeof(int), ios::beg);
    bin.read((char*)&shift, sizeof(shift)); 

    //�������� �������� ��������� � ������ � ��������� ������������ ���������� ���������
    bin.seekg(ios::beg);
    bin.read((char*)&len, sizeof(len));
    // ����� ��������� ��������� � ����� �������
    bin.seekg(shift + sizeof(my_str*) * num, ios::beg);

    if (num == len) {
        Extend();

        bin.seekg(1 * sizeof(int), ios::beg);
        bin.read((char*)&num, sizeof(num));

        // �������� �������� ��������� �� ��� int � ��������� �����
        bin.seekg(2 * sizeof(int), ios::beg);
        bin.read((char*)&shift, sizeof(shift));

        //�������� �������� ��������� � ������ � ��������� ������������ ���������� ���������
        bin.seekg(ios::beg);
        bin.read((char*)&len, sizeof(len));
        // ����� ��������� ��������� � ����� �������
        bin.seekg(shift + sizeof(my_str*) * num, ios::beg);
    }

    // �������� ������ ��������
    my_str* buff = new my_str;
    buff->count = strlen(newelem);
    buff->str = new char[buff->count];
    //����������� � ����� ������ newelem
    strcpy(buff->str, newelem);
    // ������ � �������� ����
    bin.write((char*)&buff, sizeof(buff));
    num++;
    // ������ � �������� ���� ������ ���������� ���������
    bin.seekg(1 * sizeof(int), ios::beg);
    bin.write((char*)&num, sizeof(int));
    
}

// ���������� ������ �� �������
void BinFile::ToFileInd(char* newelem, int index) {
    int shift = 0, num = 0, len = 0;

    // �������� �������� ��������� �� ���� int �� ������ � ��������� ���������� ���������
    bin.seekg(1 * sizeof(int), ios::beg);
    bin.read((char*)&num, sizeof(num));

    // �������� �������� ��������� �� ��� int � ��������� �����
    bin.seekg(2 * sizeof(int), ios::beg);
    bin.read((char*)&shift, sizeof(shift));

    //�������� �������� ��������� � ������ � ��������� ������������ ���������� ���������
    bin.seekg(ios::beg);
    bin.read((char*)&len, sizeof(len));
 
    if (index > num || index < 0)
    {
        cout << "������� �� ��� ��������" << endl;
        return;
    }

    if (num == len) {
        Extend();

        // �������� �������� ��������� �� ���� int �� ������ � ��������� ���������� ���������
        bin.seekg(1 * sizeof(int), ios::beg);
        bin.read((char*)&num, sizeof(num));

        // �������� �������� ��������� �� ��� int � ��������� �����
        bin.seekg(2 * sizeof(int), ios::beg);
        bin.read((char*)&shift, sizeof(shift));

        //�������� �������� ��������� � ������ � ��������� ������������ ���������� ���������
        bin.seekg(ios::beg);
        bin.read((char*)&len, sizeof(len));
    }

    // �������� ��������� ��������� � ����� �������
    bin.seekg(shift + sizeof(my_str*) * (num - 1), ios::beg);

    my_str* buff1;
    // ���� ������� ��� �������� �� ������� ������� ������, ����� � ���������� �� ������ ����� �������� ����� �������
    while (bin.tellg() > (index - 1) * sizeof(my_str*) + shift) {
        bin.read((char*)(&buff1), sizeof(my_str*));
        bin.seekg(bin.tellg(), ios::beg);
        bin.write((char*)(&buff1), sizeof(my_str*));
        bin.seekg(-3 * sizeof(my_str*), ios::cur);
    }
    // � ���������� ����� ��������� ��������� �� ���� ������� ������, ��� ����� ��� ������
    bin.seekg(1 * sizeof(my_str*), ios::cur);

    buff1 = new my_str;
    buff1->count = strlen(newelem);
    buff1->str = new char[buff1->count];
    strcpy(buff1->str, newelem);

    // ������ ������ ��������
    bin.write((char*)&buff1, sizeof(my_str*));

    // ���������� num �� �������
    num++;
    bin.seekg(1 * sizeof(int), ios::beg);
    bin.write((char*)&num, sizeof(int));
}


// �������� ������ �� �������
void BinFile::DeleteStr(int index)
{
    int shift, num;
    // �������� �������� ��������� �� ���� int �� ������ � ��������� ���������� ���������
    bin.seekg(1 * sizeof(int), ios::beg);
    bin.read((char*)&num, sizeof(num));

    // �������� �������� ��������� �� ��� int � ��������� �����
    bin.seekg(2 * sizeof(int), ios::beg);
    bin.read((char*)&shift, sizeof(shift));
    
    if (index > num || index < 0)
    {
        cout << "������� �� ��� ������" << endl;
        return;
    }

    my_str* buff;

    // �������� �������� ��������� �� ���� ������� ������ ����������
    bin.seekg(shift + sizeof(my_str*) * (index + 1), ios::beg);


    // ���� ������� �������� �����
    while (bin.tellg() < shift + sizeof(my_str*) * (num)) {
        bin.read((char*)(&buff), sizeof(my_str*));
        bin.seekg(-2 * sizeof(my_str*), ios::cur);
        bin.write((char*)(&buff), sizeof(my_str*));
        bin.seekg(1 * sizeof(my_str*), ios::cur);
    }


    // ���������� ���������� ��������� �� �������
    num--;
    bin.seekg(1 * sizeof(int), ios::beg);
    bin.write((char*)&num, sizeof(int));

}
// �������������� ������ �� �������
void BinFile::Edit(char* newelem, int index)
{
    int shift, num;
    // �������� �������� ��������� �� ���� int �� ������ � ��������� ���������� ���������
    bin.seekg(1 * sizeof(int), ios::beg);
    bin.read((char*)&num, sizeof(num));

    // �������� �������� ��������� �� ��� int � ��������� �����
    bin.seekg(2 * sizeof(int), ios::beg);
    bin.read((char*)&shift, sizeof(shift));

    if (index >= num || index < 0)
    {
        cout << "������� �� ��� ��������������";
        return;
    }

    // ����� ��������� ��������� � �������������� ��������
    bin.seekg(shift + sizeof(my_str*) * index, ios::beg);

    // �������� ������ �������� � ����������� ���� newelem
    my_str* buff = new my_str;
    buff->count = strlen(newelem);
    buff->str = new char[buff->count];
    strcpy(buff->str, newelem);
    // ������ � �������� ����
    bin.write((char*)&buff, sizeof(buff));
}

// ������ ��������� ����� � ���������
void BinFile::ToTXT(char* n) {
    ofstream res(n);

    int shift, num;
    // �������� �������� ��������� �� ���� int �� ������ � ��������� ���������� ���������
    bin.seekg(1 * sizeof(int), ios::beg);
    bin.read((char*)&num, sizeof(num));

    // �������� �������� ��������� �� ��� int � ��������� �����
    bin.seekg(2 * sizeof(int), ios::beg);
    bin.read((char*)&shift, sizeof(shift));


    // ����� ��������� ��������� �� shift, �.�. � ������  �������
    bin.seekg(shift, ios::beg);

    for (int i = 0; i < num; i++) {
        my_str* buff = nullptr;

        // ������ ������
        bin.read((char*)&buff, sizeof(buff));

        //����� ������
        print(buff, res);
    }

    res.close();
}

// ����� ����� � �������
void BinFile::printFile() 
{

    int shift, num, len;
    // �������� �������� ��������� �� ���� int �� ������ � ��������� ���������� ���������
    bin.seekg(1 * sizeof(int), ios::beg);
    bin.read((char*)&num, sizeof(num));
    
    // �������� �������� ��������� �� ��� int � ��������� �����
    bin.seekg(2 * sizeof(int), ios::beg);
    bin.read((char*)&shift, sizeof(shift));

    bin.seekg(ios::beg);
    bin.read((char*)&len, sizeof(len));

    bin.seekg(shift, ios::beg);
    cout << num << " " << len << endl;
    // ���������� �����
    for (int i = 0; i < num; i++) {
        my_str* buff = nullptr;
        bin.read((char*)&buff, sizeof(buff));
        print(buff, cout);
    }
}


// ���������� ��������� ����� �� ����������
void BinFile::FromTXT(char* n) {
    ifstream in(n);

    string buffer;
    int cnt = 0;

    // ������� ����� � �����
    while (std::getline(in, buffer)) {
        cnt++;
    }
    int cap = 2;
    int shift = 3 * sizeof(int);
    // ������ ����������� ���������� � ������ �����
    bin.write((char*)&cap, sizeof(cap));
    bin.write((char*)&cnt, sizeof(cnt));
    bin.write((char*)&shift, sizeof(shift));

    in.clear();
    in.seekg(0);
    // ���� ��������� ��������� �������� ���� � ���������� ������ � ��������
    for (int i = 0; i < cnt; i++) {
        std::getline(in, buffer);
        ToFileEnd((char*)buffer.data());
    }

    in.close();
}
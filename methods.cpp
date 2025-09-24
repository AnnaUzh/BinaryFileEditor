#include "headers.h"

// вывод строки в передаваемый поток
void BinFile::print(my_str* str, ostream& stream) {
    for (int i = 0; i < str->count; i++) {
        stream << str->str[i];
    }
    stream << endl;
}
// конструкор и деструктор
BinFile::BinFile(char* nam) { 
    bin.open(nam, ios::binary | ios::in | ios::out);

    int cap = 2, cnt = 0;
    int shift = 3 * sizeof(int);
    // запись необходимых параметров в начало файла
    bin.write((char*)&cap, sizeof(cap));
    bin.write((char*)&cnt, sizeof(cnt));
    bin.write((char*)&shift, sizeof(shift));
}
BinFile::~BinFile() { bin.close(); }

void BinFile::Extend() {
    int shift, len;

    // сдвигаем файловый указатель на два int и считываем сдвиг
    bin.seekg(2 * sizeof(int), ios::beg);
    bin.read((char*)&shift, sizeof(shift));

    //сдвигаем файловый указатель в начало и считываем саксимальное количество элементов
    bin.seekg(ios::beg);
    bin.read((char*)&len, sizeof(len));

    int old_shift = shift, old_len = len;
    // увеличение массива в 2 раза
    len *= 2;
    shift += old_len * 8;

    bin.seekg(old_shift, ios::beg);
    // копирование элементов в новый массив
    while (bin.tellg() != shift) {
        my_str* buf;
        bin.read((char*)&buf, sizeof(buf));
        bin.seekg((old_len - 1) * 8, ios::cur);
        bin.write((char*)&buf, sizeof(buf));
        bin.seekg(-old_len * 8, ios::cur);
    }
    // запись новых shift и len  в файл
    bin.seekg(2 * sizeof(int), ios::beg);
    bin.write((char*)&shift, sizeof(shift));

    bin.seekg(ios::beg);
    bin.write((char*)&len, sizeof(len));
}


// добавление строки в конец файла
void BinFile::ToFileEnd(char* newelem)
{
    int shift, num, len; 
    // сдвигаем файловый указатель на один int от начала и считываем количество элементов
    bin.seekg(1 * sizeof(int), ios::beg);
    bin.read((char*)&num, sizeof(num));

    // сдвигаем файловый указатель на два int и считываем сдвиг
    bin.seekg(2 * sizeof(int), ios::beg);
    bin.read((char*)&shift, sizeof(shift)); 

    //сдвигаем файловый указатель в начало и считываем саксимальное количество элементов
    bin.seekg(ios::beg);
    bin.read((char*)&len, sizeof(len));
    // сдвиг файлового указателя в конец массива
    bin.seekg(shift + sizeof(my_str*) * num, ios::beg);

    if (num == len) {
        Extend();

        bin.seekg(1 * sizeof(int), ios::beg);
        bin.read((char*)&num, sizeof(num));

        // сдвигаем файловый указатель на два int и считываем сдвиг
        bin.seekg(2 * sizeof(int), ios::beg);
        bin.read((char*)&shift, sizeof(shift));

        //сдвигаем файловый указатель в начало и считываем саксимальное количество элементов
        bin.seekg(ios::beg);
        bin.read((char*)&len, sizeof(len));
        // сдвиг файлового указателя в конец массива
        bin.seekg(shift + sizeof(my_str*) * num, ios::beg);
    }

    // создание нового элемента
    my_str* buff = new my_str;
    buff->count = strlen(newelem);
    buff->str = new char[buff->count];
    //копирование в новую строку newelem
    strcpy(buff->str, newelem);
    // запись в бинарный файл
    bin.write((char*)&buff, sizeof(buff));
    num++;
    // запись в бинарный файл нового количества элементов
    bin.seekg(1 * sizeof(int), ios::beg);
    bin.write((char*)&num, sizeof(int));
    
}

// добавление строки по индексу
void BinFile::ToFileInd(char* newelem, int index) {
    int shift = 0, num = 0, len = 0;

    // сдвигаем файловый указатель на один int от начала и считываем количество элементов
    bin.seekg(1 * sizeof(int), ios::beg);
    bin.read((char*)&num, sizeof(num));

    // сдвигаем файловый указатель на два int и считываем сдвиг
    bin.seekg(2 * sizeof(int), ios::beg);
    bin.read((char*)&shift, sizeof(shift));

    //сдвигаем файловый указатель в начало и считываем саксимальное количество элементов
    bin.seekg(ios::beg);
    bin.read((char*)&len, sizeof(len));
 
    if (index > num || index < 0)
    {
        cout << "Элемент не был добавлен" << endl;
        return;
    }

    if (num == len) {
        Extend();

        // сдвигаем файловый указатель на один int от начала и считываем количество элементов
        bin.seekg(1 * sizeof(int), ios::beg);
        bin.read((char*)&num, sizeof(num));

        // сдвигаем файловый указатель на два int и считываем сдвиг
        bin.seekg(2 * sizeof(int), ios::beg);
        bin.read((char*)&shift, sizeof(shift));

        //сдвигаем файловый указатель в начало и считываем саксимальное количество элементов
        bin.seekg(ios::beg);
        bin.read((char*)&len, sizeof(len));
    }

    // файловый указатель смещается в конец массива
    bin.seekg(shift + sizeof(my_str*) * (num - 1), ios::beg);

    my_str* buff1;
    // цикл смещает все элементы до нужного индекса вправо, чтобы в дальнейшем на пустое место вставить новый элемент
    while (bin.tellg() > (index - 1) * sizeof(my_str*) + shift) {
        bin.read((char*)(&buff1), sizeof(my_str*));
        bin.seekg(bin.tellg(), ios::beg);
        bin.write((char*)(&buff1), sizeof(my_str*));
        bin.seekg(-3 * sizeof(my_str*), ios::cur);
    }
    // в результате цикла указатель находится на один элемент правее, чем нужно для записи
    bin.seekg(1 * sizeof(my_str*), ios::cur);

    buff1 = new my_str;
    buff1->count = strlen(newelem);
    buff1->str = new char[buff1->count];
    strcpy(buff1->str, newelem);

    // запись нового элемента
    bin.write((char*)&buff1, sizeof(my_str*));

    // увеличение num на единицу
    num++;
    bin.seekg(1 * sizeof(int), ios::beg);
    bin.write((char*)&num, sizeof(int));
}


// удаление строки по индексу
void BinFile::DeleteStr(int index)
{
    int shift, num;
    // сдвигаем файловый указатель на один int от начала и считываем количество элементов
    bin.seekg(1 * sizeof(int), ios::beg);
    bin.read((char*)&num, sizeof(num));

    // сдвигаем файловый указатель на два int и считываем сдвиг
    bin.seekg(2 * sizeof(int), ios::beg);
    bin.read((char*)&shift, sizeof(shift));
    
    if (index > num || index < 0)
    {
        cout << "Элемент не был удален" << endl;
        return;
    }

    my_str* buff;

    // сдвигаем файловый указатель на один элемент дальше удаляемого
    bin.seekg(shift + sizeof(my_str*) * (index + 1), ios::beg);


    // цикл смещает элементы влево
    while (bin.tellg() < shift + sizeof(my_str*) * (num)) {
        bin.read((char*)(&buff), sizeof(my_str*));
        bin.seekg(-2 * sizeof(my_str*), ios::cur);
        bin.write((char*)(&buff), sizeof(my_str*));
        bin.seekg(1 * sizeof(my_str*), ios::cur);
    }


    // уменьшение количества элементов на единицу
    num--;
    bin.seekg(1 * sizeof(int), ios::beg);
    bin.write((char*)&num, sizeof(int));

}
// редактирование строки по индексу
void BinFile::Edit(char* newelem, int index)
{
    int shift, num;
    // сдвигаем файловый указатель на один int от начала и считываем количество элементов
    bin.seekg(1 * sizeof(int), ios::beg);
    bin.read((char*)&num, sizeof(num));

    // сдвигаем файловый указатель на два int и считываем сдвиг
    bin.seekg(2 * sizeof(int), ios::beg);
    bin.read((char*)&shift, sizeof(shift));

    if (index >= num || index < 0)
    {
        cout << "Элемент не был отредактирован";
        return;
    }

    // сдвиг файлового указателя к редактируемому элементу
    bin.seekg(shift + sizeof(my_str*) * index, ios::beg);

    // создание нового элемента и копирование туда newelem
    my_str* buff = new my_str;
    buff->count = strlen(newelem);
    buff->str = new char[buff->count];
    strcpy(buff->str, newelem);
    // запись в бинарный файл
    bin.write((char*)&buff, sizeof(buff));
}

// запись бинарного файла в текстовый
void BinFile::ToTXT(char* n) {
    ofstream res(n);

    int shift, num;
    // сдвигаем файловый указатель на один int от начала и считываем количество элементов
    bin.seekg(1 * sizeof(int), ios::beg);
    bin.read((char*)&num, sizeof(num));

    // сдвигаем файловый указатель на два int и считываем сдвиг
    bin.seekg(2 * sizeof(int), ios::beg);
    bin.read((char*)&shift, sizeof(shift));


    // сдвиг файлового указателя на shift, т.е. в начало  массива
    bin.seekg(shift, ios::beg);

    for (int i = 0; i < num; i++) {
        my_str* buff = nullptr;

        // чтение строки
        bin.read((char*)&buff, sizeof(buff));

        //вывод строки
        print(buff, res);
    }

    res.close();
}

// вывод файла в консоль
void BinFile::printFile() 
{

    int shift, num, len;
    // сдвигаем файловый указатель на один int от начала и считываем количество элементов
    bin.seekg(1 * sizeof(int), ios::beg);
    bin.read((char*)&num, sizeof(num));
    
    // сдвигаем файловый указатель на два int и считываем сдвиг
    bin.seekg(2 * sizeof(int), ios::beg);
    bin.read((char*)&shift, sizeof(shift));

    bin.seekg(ios::beg);
    bin.read((char*)&len, sizeof(len));

    bin.seekg(shift, ios::beg);
    cout << num << " " << len << endl;
    // построчный вывод
    for (int i = 0; i < num; i++) {
        my_str* buff = nullptr;
        bin.read((char*)&buff, sizeof(buff));
        print(buff, cout);
    }
}


// заполнение двоичного файла из текстового
void BinFile::FromTXT(char* n) {
    ifstream in(n);

    string buffer;
    int cnt = 0;

    // счетчик строк в файле
    while (std::getline(in, buffer)) {
        cnt++;
    }
    int cap = 2;
    int shift = 3 * sizeof(int);
    // запись необходимых параметров в начало файла
    bin.write((char*)&cap, sizeof(cap));
    bin.write((char*)&cnt, sizeof(cnt));
    bin.write((char*)&shift, sizeof(shift));

    in.clear();
    in.seekg(0);
    // цикл построчно считывает тексовый файл и записывает строки в бинарный
    for (int i = 0; i < cnt; i++) {
        std::getline(in, buffer);
        ToFileEnd((char*)buffer.data());
    }

    in.close();
}
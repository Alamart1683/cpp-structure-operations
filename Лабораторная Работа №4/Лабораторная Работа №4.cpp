#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <string>
#include <windows.h>
#include <stdlib.h>
using namespace std;

//СТРУКТУРЫ:

/*____________________________________________________________________________________________________*/

//СТРУКТУРА НОМЕРА: Буквенная часть и код региона
struct GIBDD_NumberOfCar
{
	char LetterAndNumberCode[7];
	unsigned short int RegionNumber : 10;
};

//СТРУКТУРА АДРЕС: Область, Город, Улица, Дом, Квартира
struct GIBDD_Owner_Adress
{
	char Region[40];
	char City[40];
	char Street[60];
	char BuildingNumber[7];
	unsigned short int ApartamensNumber : 12;
};

//CТРУКТУРА ВЛАДЕЛЕЦ: Фамилия Имя Отчество
struct GIBDD_Owner
{
	char Surname[20];
	char Name[20];
	char Patronymic[30];
	GIBDD_Owner_Adress Adress;
};

//СТРУКТУРА ДАТА ПОСЛЕДНЕГО ТЕХОСМОТРА: День, Месяц, Год
struct GIBBD_Date
{
	unsigned short int Day : 5;
	unsigned short int Mounth : 4;
	unsigned short int Year : 12;
};

//СТРУКТУРА ДАННЫХ ОБ ОДНОМ АВТОМОБИЛЕ В БД ГИБДД (ВКЛЮЧАЕТ В СЕБЯ ВСЕ ПОДСТРУКТУРЫ ВЫШЕ)
struct GIBDD
{
	char Model[30];
	GIBDD_NumberOfCar NumberOfCar;
	char Color[40];
	GIBDD_Owner Owner;
	GIBBD_Date Date;
};

//СТРУКТУРА ДЕРЕВА ДЛЯ ОСНОВНОГО ЗАДАНИЯ
struct Tree
{
	GIBDD TreeField_DataBase_Element;
	Tree* Left; //Левый сын
	Tree* Right; //Правый сын
};

//СТРУКТУРА СПИСКА ДЛЯ ДОПОЛНИТЕЛЬНОГО ЗАДАНИЯ
struct Node
{
	GIBDD_NumberOfCar Number;
	Node *Next;
	Node *Prev;
};

//Создаем тип данных Список
class List
{
	Node *Head, *Tail;//Указатели на адреса начала списка и его конца
public:
	List() :Head(NULL), Tail(NULL) {};//Инициализируем адреса как пустые
	~List(); //Деструктор
	void Show(); //Функция отображения списка на экране
	void Add(GIBDD_NumberOfCar Number); //Функция добавления элементов в список
};

//Деструктор
List::~List() 
{
	while (Head) //Пока по адресу на начало списка что-то есть
	{
		Tail = Head->Next; //Резервная копия адреса следующего звена списка
		delete Head; //Очистка памяти от первого звена
		Head = Tail; //Смена адреса начала на адрес следующего элемента
	}
}

/*____________________________________________________________________________________________________*/

//ПРОТОТИПЫ ФУНКЦИЙ:

//Интерфейс
void Interface_Main();
void Interface_InputArray();
void Interface_OutputArray();
void Interface_Task1();
void Interface_Task2();
//Ввод и вывод
int InputController(int Max_Variants, int Min_Variants);
GIBDD* InputStructsInArray(GIBDD* DataBase, int* Database_Length);
GIBDD* AddStruct(GIBDD* DataBase, const int Database_Length);
void InputStruct(GIBDD* DataBase, int DataBase_Length);
void OutputStructsInArray(GIBDD* DataBase, int DataBase_Length);
void OutputStruct(GIBDD CarData);
//Основное задание
GIBDD* Tree_Sort(GIBDD* DataBase, int DataBase_Length);
struct Tree * AddNode(GIBDD DataBase_Element, Tree *TreeNode);
GIBDD* PrintTreeInArray(GIBDD* DataBase, int DataBase_Length, Tree* TreeNode, int* ptr_Count);
void FreeMemory(Tree* TreeNode);
//Дополнительное задание
void VolgasWithoutMot(GIBDD* DataBase, int Database_Length);

//МАССИВ ДЛЯ СТРУКТУР И ИЖЕ С НИМ
GIBDD* DataBase = 0;
int DataBase_Length = 0;
int* ptr_DataBase_Length = &DataBase_Length;
int Count = 0;
int* ptr_Count = &Count;
int main()
{
	system("cls");
	setlocale(LC_CTYPE, "RUS");

	//ПОПЫТКА УВЕЛИЧИТЬ ОКНО КОНСОЛИ
	HANDLE out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD maxWindow = GetLargestConsoleWindowSize(out_handle); //Размер самого большого возможного консольного окна
	SMALL_RECT srctWindow = { 0, 0, maxWindow.X - 50, maxWindow.Y - 25 };
	SMALL_RECT minWindow = { 0, 0, 0, 0 };
	SetConsoleWindowInfo(out_handle, true, &minWindow);
	SetConsoleScreenBufferSize(out_handle, maxWindow);
	SetConsoleWindowInfo(out_handle, true, &srctWindow);

	int step;
	int Controller = 0;
	while (true)
	{
		Interface_Main();
		int Step_MaxVariants = 5;
		int Step_MinVariants = 1;
		step = InputController(Step_MaxVariants, Step_MinVariants);

/*____________________________________________________________________________________________________*/

		//ЗАПОЛНЕНИЕ МАССИВА СТРУКТУР
		if (step == 1)
		{
			while (true) 
			{
				system("cls");
				Interface_InputArray();
				Step_MaxVariants = 3;
				step = InputController(Step_MaxVariants, Step_MinVariants);
				
				//ВЫЗОВ ПРОЦЕДУРЫ ЗАПОЛНЕНИЯ
				if (step == 1)
				{
					system("cls");
					DataBase = InputStructsInArray(DataBase, ptr_DataBase_Length);
					Controller = 1;
					step = 0;
				}

				//ВЫЗОВ ПРОЦЕДУРЫ УДАЛЕНИЯ
				else if (step == 2)
				{
					while (true)
					{
						if (Controller == 0)
						{
							system("cls");
							cout << "Прежде чем удалить массив, следует его ввести." << endl;
							system("pause");
							break;
						}
						else if (Controller == 1)
						{
							system("cls");
							cout << "Вы точно уверены? (1 - Да, 2 - нет)" << endl;
							step = InputController(2, 1);
							if (step == 1)
							{
								Controller = 0;
								DataBase_Length = 0;
								delete[] DataBase;
								system("cls");
								cout << "Массив структур был успешно удален!" << endl;
								system("pause");
								break;
							}
							else if (step == 2)
							{
								step = 0;
								break;
							}
						}					
					}				
				}

				//ВЫХОД В МЕНЮ ЗАДАНИЯ
				else if (step == 3)
				{
					step = 0;
					break;
				}
			}		
		}

/*____________________________________________________________________________________________________*/

		//ВЫВОД МАССИВА СТРУКТУР
		else if (step == 2)
		{
			while (true)
			{
				system("cls");
				Interface_OutputArray();
				Step_MaxVariants = 2;
				step = InputController(Step_MaxVariants, Step_MinVariants);
				//ВЫВОД
				if (step == 1)
				{
					if (Controller != 1)
					{
						system("cls");
						cout << "Для начала введите массив структур! " << endl;
						system("pause");
					}
					else
					{
						OutputStructsInArray(DataBase, DataBase_Length);
						system("cls");
						step = 0;
					}
				}
				//ВОЗВРАТ
				else if (step == 2)
				{
					step = 0;
					break;
				}
			}
		}

/*____________________________________________________________________________________________________*/

		//ОСНОВНОЕ ЗАДАНИЕ
		else if (step == 3)
		{
			while (true)
			{
				system("cls");
				Interface_Task1();
				Step_MaxVariants = 2;
				step = InputController(Step_MaxVariants, Step_MinVariants);
				
				//БИНАРНАЯ СОРТИРОВКА ПО МОДЕЛЯМ
			    if (step == 1)
				{
					if (Controller != 1)
					{
						system("cls");
						cout << "Для начала введите массив структур! " << endl;
						system("pause");
					}
					else
					{						
						DataBase = Tree_Sort(DataBase, DataBase_Length);
						system("cls");
						cout << "Отсортированный массив записей по моделям:" << endl;
						OutputStructsInArray(DataBase, DataBase_Length);
						step = 0;
					}
				}
				//ВОЗВРАТ
				else if (step == 2)
				{
					step = 0;
					break;
				}
			}
		}

/*____________________________________________________________________________________________________*/

		//ДОПОЛНИТЕЛЬНОЕ ЗАДАНИЕ
		else if (step == 4)
		{
			while (true)
			{
				system("cls");
				Interface_Task2();
				Step_MaxVariants = 2;
				step = InputController(Step_MaxVariants, Step_MinVariants);

				//СПИСОК ПО ТЕХОСМОТРУ
				if (step == 1)
				{
					if (Controller != 1)
					{
						system("cls");
						cout << "Для начала введите массив структур! " << endl;
						system("pause");
					}
					else
					{
						VolgasWithoutMot(DataBase, DataBase_Length);
					}
				}
				
				//ВОЗВРАТ
				else if (step == 2)
				{
					step = 0;
					Controller = 0;
					break;
				}
			}
		}

/*____________________________________________________________________________________________________*/

		//ВЫХОД
		else if (step == 5)
		{
			system("cls");
			cout << "Выход из программы осуществлен!" << endl;
			system("pause");
			break;
		}
	}
	if (Controller == 1)
	   delete[] DataBase;
	ptr_DataBase_Length = NULL;
	ptr_Count = NULL;
}

//ФУНКЦИИ ИНТЕРФЕЙСА:
void Interface_Main()
{
	system("cls");
	cout << "Лабораторная работа №4: Вариант 14" << endl;
	cout << "Выберите действие:" << endl;
	cout << "1. Заполнить массив записей;" << endl;
	cout << "2. Вывести массив записей;" << endl;
	cout << "3. Основное задание;" << endl;
	cout << "4. Дополнительное задание;" << endl;
	cout << "5. Выход из программы." << endl;
}

void Interface_InputArray() 
{
	system("cls");
	cout << "Выбрано меню ввода." << endl;
	cout << "Выберите действие: " << endl;
	cout << "1. Начать/Продолжить процедуру заполнения." << endl;
	cout << "2. Удалить массив." << endl;
	cout << "3. Вернуться к выбору подзадания." << endl;
}

void Interface_OutputArray()
{
	system("cls");
	cout << "Выбрано меню вывода." << endl;
	cout << "Выберите действие: " << endl;
	cout << "1. Вывести массив записей." << endl;
	cout << "2. Вернуться к выбору подзадания." << endl;
}

void Interface_Task1()
{
	system("cls");
	cout << "Выбрано Основное задание." << endl;
	cout << "Выберите действие:" << endl;
	cout << "1. Бинарная сортировка по моделям." << endl;
	cout << "2. Вернуться к выбору задания." << endl;
}

void Interface_Task2()
{
	cout << "Выбрано Дополнительное задание." << endl;
	cout << "Выберите действие:" << endl;
	cout << "1. Сформировать список номеров машин модели “Волга” не прошедших техосмотр в этом году." << endl;
	cout << "2. Вернуться к выбору задания." << endl;
}

//ФУНКЦИИ ВВОДА/ВЫВОДА:

//Защита от дурака при вводе
int InputController(int Max_Variants, int Min_Variants)
{
	int Input_Value;
	while (!(cin >> Input_Value) || Input_Value < Min_Variants || Input_Value > Max_Variants)
	{
		cin.clear();
		while (cin.get() != '\n');
		cout << "Вы ввели некорректное значение!" << endl;
		cout << "Повторите ввод." << endl;
	}
	return Input_Value;
}

//Заполнение динамического массива структур указанной длины
GIBDD* InputStructsInArray(GIBDD* DataBase, int* ptr_DataBase_Length)
{	
	int step = 0;
	DataBase_Length = *ptr_DataBase_Length;
	do
	{	
		system("cls");
		cout << "Идет процесс ввода данных в массив:" << endl;
		cout << endl;
		cout << "Запись №" << DataBase_Length + 1 << ":" << endl;
		cout << "-------------------------------" << endl;
		cout << endl;
		DataBase = AddStruct(DataBase, DataBase_Length);
		InputStruct(DataBase, DataBase_Length);
		DataBase_Length++;
		cout << "Продолжить ввод данных (1 - Да, 2 - Нет): " << endl;
		step = InputController(2, 1);
	} while (step != 2);
	*ptr_DataBase_Length = DataBase_Length;
	system("cls");
	cout << "Ввод массива окончен!" << endl;
	system("pause");
	return DataBase;
}

//Увеличение массива
GIBDD* AddStruct(GIBDD* DataBase, const int DataBase_Length )
{
	if (DataBase_Length == 0)
	{
		DataBase = new GIBDD[DataBase_Length + 1]; //Память первой структуре
	}
	else
	{
		GIBDD* DataBase_Temp = new GIBDD[DataBase_Length + 1];

		for (int i = 0; i < DataBase_Length; i++)
		{
			DataBase_Temp[i] = DataBase[i]; //Резервное копирование
		}
		delete[] DataBase;

		DataBase = DataBase_Temp;
	}
	return DataBase;
}

//Заполнение одной структуры
void InputStruct(GIBDD* DataBase, int DataBase_Length)
{
	//АВТОМОБИЛЬ
	cout << "Необходимые данные автомобиля :" << endl;
	cout << "===============================" << endl;
	cin.ignore();
	cout << "Модель  : ";
	cin.getline(DataBase[DataBase_Length].Model, 30);
	cout << "Номер   : ";
	cin.getline(DataBase[DataBase_Length].NumberOfCar.LetterAndNumberCode, 7);
	cout << "Код(xxx): ";
	DataBase[DataBase_Length].NumberOfCar.RegionNumber = InputController(797, 0);
	cin.ignore();
	cout << "Цвет    : ";
	cin.getline(DataBase[DataBase_Length].Color, 40);
	cout << endl;

	//ВЛАДЕЛЕЦ
	cout << "Фамилия Имя Отчество владельца:" << endl;
	cout << "===============================" << endl;
	cout << "Имя     : ";
	cin.getline(DataBase[DataBase_Length].Owner.Name, 20);
	cout << "Фамилия : ";
	cin.getline(DataBase[DataBase_Length].Owner.Surname, 20);
	cout << "Отчество: ";
	cin.getline(DataBase[DataBase_Length].Owner.Patronymic, 30);
	cout << endl;

	//АДРЕС
	cout << "Место жительства владельца    :" << endl;
	cout << "===============================" << endl;
	cout << "Область : ";
	cin.getline(DataBase[DataBase_Length].Owner.Adress.Region, 40);
	cout << "Город   : ";
	cin.getline(DataBase[DataBase_Length].Owner.Adress.City, 40);
	cout << "Улица   : ";
	cin.getline(DataBase[DataBase_Length].Owner.Adress.Street, 60);
	cout << "Дом     : ";
	cin.getline(DataBase[DataBase_Length].Owner.Adress.BuildingNumber, 7);
	cout << "Номер КВ: ";
	DataBase[DataBase_Length].Owner.Adress.ApartamensNumber = InputController(2048, 1);
	cout << endl;

	//ДАТА ПОСЛЕДНЕГО ТЕХОСМОТРА
	cout << "Дата последнего техосмотра    :" << endl;
	cout << "===============================" << endl;
	cout << "Число   : ";
	DataBase[DataBase_Length].Date.Day = InputController(31, 1);
	cout << "Месяц   : ";
	DataBase[DataBase_Length].Date.Mounth = InputController(12, 1);
	cout << "Год     : ";
	DataBase[DataBase_Length].Date.Year = InputController(2100, 1892);
	cout << endl;

	system("cls");
}

//Вывод массива структур
void OutputStructsInArray(GIBDD* DataBase, int DataBase_Length)
{
	system("cls");
	cout << "Вывод осуществляется построчно, каждая запись суть строка, где в порядке, указанном ниже перечислены:" << endl;
	cout << "Сведения об автомобиле: Модель,Номер(xNNNxx),Регион(код),Цвет. Личные данные владельца: Фамилия,Имя,Отчество,Регион,Город,Улица,Дом,Квартира. Дата техосмотра: День,Месяц,Год." << endl;
	cout << "==============================================================================================================================================================================" << endl;
	for (int i = 0; i < DataBase_Length; i++)
	{	
		if (i < 10)
		{
			cout << "Запись №" << i + 1 << " : ";
		}
		else if (i < 100)
		{
			cout << "Запись №" << i + 1 << ": ";
		}
		OutputStruct(DataBase[i]);
		cout << endl;
	}
	cout << "==============================================================================================================================================================================" << endl;
	cout << "Вывод массива окончен!" << endl;
	system("pause");
}

//Вывод одной структуры
void OutputStruct(GIBDD DataBase_Element)
{
	cout << DataBase_Element.Model << '\t';
	cout << DataBase_Element.NumberOfCar.LetterAndNumberCode << '\t';
	cout << DataBase_Element.NumberOfCar.RegionNumber << '\t';
	cout << DataBase_Element.Color << '\t';
	cout << DataBase_Element.Owner.Name << '\t';
	cout << DataBase_Element.Owner.Surname << '\t';
	cout << DataBase_Element.Owner.Patronymic << '\t';
	cout << DataBase_Element.Owner.Adress.Region << '\t';
	cout << DataBase_Element.Owner.Adress.City << '\t';
	cout << DataBase_Element.Owner.Adress.Street << '\t';
	cout << DataBase_Element.Owner.Adress.BuildingNumber << '\t';
	cout << DataBase_Element.Owner.Adress.ApartamensNumber << '\t';
	cout << DataBase_Element.Date.Day << '\t';
	cout << DataBase_Element.Date.Mounth << '\t';
	cout << DataBase_Element.Date.Year << '\t';
}

//ОСНОВНОЕ ЗАДАНИЕ

//Бинарная сортировка
GIBDD* Tree_Sort(GIBDD* DataBase, int DataBase_Length)
{
	struct Tree *Root = 0;
	for (int i = 0; i < DataBase_Length; i++) //Заполнение узлов дерева элементами массива
	{
		Root = AddNode(DataBase[i],Root); 
	}
	DataBase = PrintTreeInArray(DataBase, DataBase_Length, Root, ptr_Count); //Перезаполняем отсортированный массив из дерева
	return DataBase;
	FreeMemory(Root); //Сожжение дерева
}

// Добавление узлов в дерево
struct Tree * AddNode(GIBDD DataBase_Element, Tree *TreeNode) 
{
	if (TreeNode == NULL)     // Если дерева нет, то формируем корень
	{
		TreeNode = new Tree; //Память под узел дерева
		TreeNode->TreeField_DataBase_Element = DataBase_Element;   //Поле данных, хранящее данные автомобиля
		TreeNode->Left = NULL;
		TreeNode->Right = NULL; //ветви инициализируем пустотой
	}
	else     
		if (strncmp(DataBase_Element.Model, TreeNode->TreeField_DataBase_Element.Model, 30) < 0)  //Если модель авто по алфавиту меньше корневого, уходим влево
			TreeNode->Left = AddNode(DataBase_Element, TreeNode->Left); //Рекурсивно добавляем элемент
		else  //Иначе уходим вправо
			TreeNode->Right = AddNode(DataBase_Element, TreeNode->Right); //Рекурсивно добавляем элемент
	return(TreeNode);
}

//Перезаполнение отсортированного массива из дерева
GIBDD* PrintTreeInArray(GIBDD* DataBase, int DataBase_Length, Tree* TreeNode, int* ptr_Count)
{
	if (TreeNode != NULL && Count < DataBase_Length)
	{
		PrintTreeInArray(DataBase, DataBase_Length, TreeNode->Left, ptr_Count); //Рекурсивная функция вывода левого поддерева
		Count = *ptr_Count;
		DataBase[Count] = TreeNode->TreeField_DataBase_Element;
		Count++;
		*ptr_Count = Count;
		PrintTreeInArray(DataBase, DataBase_Length, TreeNode->Right, ptr_Count); //Рекурсивная функция вывода правого поддерева
	}
	return DataBase;
}

//Освобождение памяти из-под дерева
void FreeMemory(Tree* TreeNode)
{
	if (TreeNode != NULL)    //Если дерево не пустое
	{
		FreeMemory(TreeNode->Left); //Рекурсивно удаляем левую ветку
		FreeMemory(TreeNode->Right); //Рекурсивно удаляем правую ветку
		delete TreeNode; //Удаляем корень
	}
}

//ДОПОЛНИТЕЛЬНОЕ ЗАДАНИЕ

//Процедура наполнения и вывода списка
void VolgasWithoutMot(GIBDD* DataBase, int Database_Length)
{
	List NumberList;
	for (int i = 0; i < DataBase_Length; i++)
	{
		if (DataBase[i].Date.Year < 2018 && (strncmp(DataBase[i].Model,"Volga",30) == 0))
			NumberList.Add(DataBase[i].NumberOfCar);
	}
	system("cls");
	cout << "Cписок:" << endl;
	cout << "Номер |Код" << endl;
	cout << "==========" << endl;
	NumberList.Show();
	cout << "==========" << endl;
	NumberList.~List();
	system("pause");	
}

//Добавление элемента в список
void List::Add(GIBDD_NumberOfCar Number)
{
	Node *Temp = new Node; //Выделение памяти под новый элемент структуры
	Temp->Next = NULL;  //Указываем, что изначально по следующему адресу пусто
	Temp->Number = Number;//Записываем значение в структуру

	if (Head != NULL) //Если список не пуст
	{
		Temp->Prev = Tail; //Указание на предыдущий элемент
		Tail->Next = Temp; //Указание на следующий элемент
		Tail = Temp; //Смена адреса хвоста
	}
	else //Если список пустой
	{
		Temp->Prev = NULL; //Предыдущий элемент указывает в небытие
		Head = Tail = Temp; //Голова=Хвост=тот элемент, что сейчас был добавлен
	}
}

//Вывод списка
void List::Show()
{
	//ВЫВОДИМ СПИСОК С НАЧАЛА
	Node *Temp = Head; //Временное указание на первый элемент
	while (Temp != NULL) //Пока не встретим небытие
	{
		cout << Temp->Number.LetterAndNumberCode << "|" <<  Temp->Number.RegionNumber<< endl; 
		Temp = Temp->Next; //Смена адреса на адрес следующего элемента
	}
}
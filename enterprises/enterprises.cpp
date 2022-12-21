#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
using namespace std;
using namespace nlohmann;

//для создания рандомных значений
#define NUMBER_RECORDS 2

struct ZAP
{
	string Company;
	int CountPersonnel;
	int Industrial;
	int NonIndustrial;
	float Share;
	float Plan;
};

string FIELD_1 = "|№ п/п",
       FIELD_2 = "|Наим. предприятия|",
       FIELD_3 = "|Всего перс. по плану|",
       FIELD_4 = "|Промышленного|",
       FIELD_5 = "|Непромышленного|",
       FIELD_6 = "|Доля не промыш. перс.|",
       FIELD_7 = "|Выпол. плана по перс.|";

void menu(vector<ZAP>& intelligence, vector<ZAP>& workIntelligence, string fileName);
string getFileName();
int getAmountZAP(string fileName);
void addNote(vector<ZAP>& intelligence, string fileName);
void closeProgram();
void createOrOpen(vector<ZAP>& intelligence, string fileName);
void open(vector<ZAP>& intelligence, string fileName);
json getStruct(string fileName);
void addInStruct(vector<ZAP>& intelligence, json jsonData, int i);
void create(vector<ZAP>& intelligence, string fileName);
string getRandomStr(int sizeStr);
void createJsonFile(json jsonData, string fileName);
void printData(vector<ZAP>& intelligence, string fileName);
void printCap(bool isCountPersonnel = true);
void requestPlan(vector<ZAP>& intelligence,
                 vector<ZAP>& workIntelligence);
void findCopyAndPrint(vector<ZAP>& intelligence,
                                  vector<ZAP>& workIntelligence,
                                  float requestedPlan);
void copy(vector<ZAP>& intelligence,
                      vector<ZAP>& workIntelligence,
                      int k,
                      int i);
void printSelectedPlan(vector<ZAP>& workIntelligence,
                                 int index,
                                 float requestedPlan);
void printRecords(vector<ZAP>& workIntelligence, int serialNumber, bool isCountPersonnel = true);
void printTotals(json sum);
void printFieldsAndRecords(vector<ZAP>& workIntelligence, int serialNumber, float requestedPlan);
void printFields(float requestedPlan);
void printMinCompany(string minCompany);

int main()
{
	string fileName;
	json jsonData;

	srand(time(NULL));
	setlocale(LC_ALL, "ru");

	fileName = getFileName();
	vector<ZAP> intelligence,
	            workIntelligence;
	createOrOpen(intelligence, fileName);
	menu(intelligence, workIntelligence, fileName);

	system("PAUSE");
	return 0;
}

void menu(vector<ZAP>& intelligence, vector<ZAP>& workIntelligence, string fileName)
{
	string menuNumber;

	while (1)
	{
		cout << "\n1 - Вывести данные" << endl;
		cout << "2 - Добавить запись" << endl;
		cout << "3 - Рассчитать сведения" << endl;
		cout << "4 - Закрыть программу" << endl;
		cout << "\nВведите номер пункта меню: ";
		cin >> menuNumber;

		if (menuNumber == "1")
			printData(intelligence, fileName);
		else if (menuNumber == "2")
			addNote(intelligence, fileName);
		else if (menuNumber == "3")
			requestPlan(intelligence, workIntelligence);
		else if (menuNumber == "4")
			closeProgram();
		else
			cout << "Такого пункта меню нет" << endl;
	}
}

string getFileName()
{
	string fileName = "intelligence",
	       fileType = ".json",
	       changeFileName;

	while (1)
	{
		cout << "По умолчанию имя файла \"" << fileName << "\"\nХотите его изменить? y/n: ";
		cin >> changeFileName;

		if (changeFileName == "y")
		{
			cout << "\nВведите новое имя файла: ";
			cin >> fileName;
			break;
		}
		if (changeFileName == "n") break;
		if (changeFileName != "y" && changeFileName != "n")
			cout << "Не корректный ввод" << endl;
	}
	fileName += fileType;
	return fileName;
}

int getAmountZAP(string fileName)
{
	json ZAP = getStruct(fileName);
	bool isErrorFileNotFound = ZAP.contains("Error file not found");
	int amountZAP;

	if (isErrorFileNotFound)
		return NUMBER_RECORDS;

	amountZAP = ZAP.size();
	return amountZAP;
}

void addNote(vector<ZAP>& intelligence, string fileName)
{
	string Company;
	int CountPersonnel;
	int Industrial;
	int NonIndustrial;
	float Share;
	float Plan;

	int indexNewNote;

	json dataJson = getStruct(fileName);

	cout << "Введите данные" << endl;

	cout << "Наименование предприятия: " << endl;
	cin >> Company;

	cout << "Всего персонала по плану: " << endl;
	cin >> CountPersonnel;

	cout << "Промышленного: " << endl;
	cin >> Industrial;

	cout << "Непромышленного: " << endl;
	cin >> NonIndustrial;

	Share = NonIndustrial * 100 / CountPersonnel;
	Plan = (Industrial + NonIndustrial) / CountPersonnel * 100;

	json enteredData{
		{"Company", Company},
		{"CountPersonnel", CountPersonnel},
		{"Industrial", Industrial},
		{"NonIndustrial", NonIndustrial},
		{"Share", Share},
		{"Plan", Plan}
	};

	dataJson.insert(dataJson.end(), enteredData);
	createJsonFile(dataJson, fileName);

	indexNewNote = getAmountZAP(fileName) - 1;
	addInStruct(intelligence, dataJson, indexNewNote);

	cout << "Запись успешно добавлена" << endl;
}

void closeProgram()
{
	cout << "Программа завершилась";
	exit(0);
}

void createOrOpen(vector<ZAP>& intelligence, string fileName)
{
	ifstream jsonFile(fileName, ifstream::binary);
	bool isOpen = jsonFile.is_open();
	jsonFile.close();

	if (isOpen)
		open(intelligence, fileName);
	else
		create(intelligence, fileName);
}

void open(vector<ZAP>& intelligence, string fileName)
{
	int amountZAP = getAmountZAP(fileName);
	json dataJson = getStruct(fileName);
	for (int i = 0; i < amountZAP; i++)
		addInStruct(intelligence, dataJson, i);
	printData(intelligence, fileName);
}

json getStruct(string fileName)
{
	ifstream jsonFile(fileName, ifstream::binary);
	bool isOpen = jsonFile.is_open();

	if (isOpen)
	{
		json jsonData = json::parse(jsonFile);
		jsonFile.close();
		return jsonData;
	}
	jsonFile.close();
	return {{"Error file not found", true}};
}

void addInStruct(vector<ZAP>& intelligence, json jsonData, int i)
{
	string Company;
	int CountPersonnel;
	int Industrial;
	int NonIndustrial;
	float Share;
	float Plan;

	Company = jsonData[i]["Company"].get<string>();
	CountPersonnel = jsonData[i]["CountPersonnel"].get<int>();
	Industrial = jsonData[i]["Industrial"].get<int>();
	NonIndustrial = jsonData[i]["NonIndustrial"].get<int>();
	Share = NonIndustrial * 100 / CountPersonnel;
	Plan = (Industrial + NonIndustrial) / CountPersonnel * 100;

	intelligence.push_back(ZAP{
		Company,
		CountPersonnel,
		Industrial,
		NonIndustrial,
		Share,
		Plan
	});
}

void create(vector<ZAP>& intelligence, string fileName)
{
	string Company;
	int CountPersonnel;
	int Industrial;
	int NonIndustrial;
	float Share;
	float Plan;

	int amountZAP = getAmountZAP(fileName);
	json jsonData{};
	string userResponse;

	while (1)
	{
		cout << "Хотите чтобы данные заполнились рандомно? (y/n): ";
		cin >> userResponse;

		if (userResponse == "y")
		{
			for (int i = 0; i < amountZAP; i++)
			{
				Company = getRandomStr(10);
				CountPersonnel = rand() % 99999;
				Industrial = rand() % 99999;
				NonIndustrial = rand() % 99;
				Share = (float)(NonIndustrial * 100) / (float)CountPersonnel;
				Plan = (float)(Industrial + NonIndustrial) / (float)CountPersonnel * 100;

				intelligence.push_back(ZAP{
					Company,
					CountPersonnel,
					Industrial,
					NonIndustrial,
					Share,
					Plan
				});

				jsonData[i]["Company"] = Company;
				jsonData[i]["CountPersonnel"] = CountPersonnel;
				jsonData[i]["Industrial"] = Industrial;
				jsonData[i]["NonIndustrial"] = NonIndustrial;
				jsonData[i]["Share"] = Share;
				jsonData[i]["Plan"] = Plan;
			}
			break;
		}

		if (userResponse == "n")
		{
			for (int i = 0; i < amountZAP; i++)
			{
				cout << "Введите данные " << i + 1 << endl;

				cout << "Наименование предприятия: " << endl;
				cin >> Company;

				cout << "Всего персонала по плану: " << endl;
				cin >> CountPersonnel;

				cout << "Промышленного: " << endl;
				cin >> Industrial;

				cout << "Непромышленного: " << endl;
				cin >> NonIndustrial;

				Share = (float)(NonIndustrial * 100) / (float)CountPersonnel;
				Plan = (float)(Industrial + NonIndustrial) / (float)CountPersonnel * 100;

				intelligence.push_back(ZAP{
					Company,
					CountPersonnel,
					Industrial,
					NonIndustrial,
					Share,
					Plan
				});

				jsonData[i]["Company"] = Company;
				jsonData[i]["CountPersonnel"] = CountPersonnel;
				jsonData[i]["Industrial"] = Industrial;
				jsonData[i]["NonIndustrial"] = NonIndustrial;
				jsonData[i]["Share"] = Share;
				jsonData[i]["Plan"] = Plan;
			}
			break;
		}

		if (userResponse != "y" && userResponse != "n")
			cout << "Не корректный ввод" << endl;
	}
	createJsonFile(jsonData, fileName);
}

string getRandomStr(int sizeStr)
{
	const char POOL[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789";
	std::string randomStr = "";
	int poolSize = sizeof(POOL) - 1;

	for (int i = 0; i < sizeStr; i++)
		randomStr += POOL[rand() % poolSize];

	return randomStr;
}

void createJsonFile(json jsonData, string fileName)
{
	ofstream jsonFile(fileName);
	jsonFile << jsonData;
	cout << "Файл " << fileName << " успешно создан" << endl;
	jsonFile.close();
}

void printData(vector<ZAP>& intelligence, string fileName)
{
	printCap();

	for (int i = 0; i < intelligence.size(); i++)
		printRecords(intelligence, i);
}

void printCap(bool isCountPersonnel)
{
	if (isCountPersonnel)
		cout
			<< "\n"
			<< left
			<< setw(FIELD_1.size()) << FIELD_1
			<< setw(FIELD_2.size()) << FIELD_2
			<< setw(FIELD_3.size()) << FIELD_3
			<< setw(FIELD_4.size()) << FIELD_4
			<< setw(FIELD_5.size()) << FIELD_5
			<< setw(FIELD_6.size()) << FIELD_6
			<< setw(FIELD_7.size()) << FIELD_7
			<< endl;
	else
		cout
			<< "\n"
			<< left
			<< setw(FIELD_1.size()) << FIELD_1
			<< setw(FIELD_2.size()) << FIELD_2
			<< setw(FIELD_4.size()) << FIELD_4
			<< setw(FIELD_5.size()) << FIELD_5
			<< setw(FIELD_6.size()) << FIELD_6
			<< setw(FIELD_7.size()) << FIELD_7
			<< endl;
}

void requestPlan(vector<ZAP>& intelligence,
                 vector<ZAP>& workIntelligence)
{
	string requestedPlan;
	bool isEnd;

	cout << "\nВведите запрашиваемый процент плана по персоналу. По завершению ввода введите exit:\n";
	while (true)
	{
		cin >> requestedPlan;
		isEnd = requestedPlan == "exit";
		if (isEnd) break;
		findCopyAndPrint(intelligence,
		                             workIntelligence,
		                             stof(requestedPlan)
		);
	}
}

void copy(vector<ZAP>& intelligence,
                      vector<ZAP>& workIntelligence,
                      int k,
                      int i)
{
	string Company;
	int CountPersonnel;
	int Industrial;
	int NonIndustrial;
	float Share;
	float Plan;

	Company = intelligence[i].Company;
	CountPersonnel = intelligence[i].CountPersonnel;
	Industrial = intelligence[i].Industrial;
	NonIndustrial = intelligence[i].NonIndustrial;
	Share = intelligence[i].Share;
	Plan = intelligence[i].Plan;

	workIntelligence.push_back(ZAP{
		Company,
		CountPersonnel,
		Industrial,
		NonIndustrial,
		Share,
		Plan
	});
	workIntelligence[k].Company = intelligence[i].Company;
	workIntelligence[k].CountPersonnel = intelligence[i].
		CountPersonnel;
	workIntelligence[k].Industrial = intelligence[i].Industrial;
	workIntelligence[k].NonIndustrial = intelligence[i].NonIndustrial;
	workIntelligence[k].Share = intelligence[i].Share;
	workIntelligence[k].Plan = intelligence[i].Plan;
}

void findCopyAndPrint(vector<ZAP>& intelligence,
                                  vector<ZAP>& workIntelligence,
                                  float requestedPlan)
{
	bool isFoundCountPersonnel;
	int k = 0,
	    Industrial = 0,
	    NonIndustrial = 0;
	float Share = 0,
	      Plan = 0,
	      minShare = FLT_MAX;
	string minCompany;
	json sum{
		{"Industrial", Industrial},
		{"NonIndustrial", NonIndustrial},
		{"Share", Share},
		{"Plan", Plan}
	};

	for (int i = 0; i < intelligence.size(); i++)
	{
		Plan = intelligence[i].Plan;
		isFoundCountPersonnel = requestedPlan <= Plan;
		if (isFoundCountPersonnel)
		{
			copy(intelligence, workIntelligence, k, i);
			printSelectedPlan(workIntelligence, k, requestedPlan);

			sum["Industrial"] = sum["Industrial"].get<int>() + intelligence[i].Industrial;
			sum["NonIndustrial"] = sum["NonIndustrial"].get<int>() + intelligence[i].NonIndustrial;
			sum["Share"] = sum["Share"].get<float>() + intelligence[i].Share;
			sum["Plan"] = sum["Plan"].get<float>() + intelligence[i].Plan;

			if (minShare > intelligence[i].Share)
			{
				minShare = intelligence[i].Share;
				minCompany = intelligence[i].Company;
			}

			k++;
		}
	}

	if (!k)
	{
		cout << "Данные не найдены\n";
		return;
	}
	printTotals(sum);
	printMinCompany(minCompany);
}

void printSelectedPlan(vector<ZAP>& workIntelligence,
                                 int index,
                                 float requestedPlan)
{
	if (index > 0)
		printRecords(workIntelligence, index, false);
	else
		printFieldsAndRecords(workIntelligence, index, requestedPlan);
}

void printRecords(vector<ZAP>& intelligence, int serialNumber, bool isCountPersonnel)
{
	if (isCountPersonnel)
		cout
			<< setw(FIELD_1.size()) << serialNumber + 1
			<< setw(FIELD_2.size()) << intelligence[serialNumber].Company
			<< setw(FIELD_3.size()) << intelligence[serialNumber].CountPersonnel
			<< setw(FIELD_4.size()) << intelligence[serialNumber].Industrial
			<< setw(FIELD_5.size()) << intelligence[serialNumber].NonIndustrial
			<< setw(FIELD_6.size()) << intelligence[serialNumber].Share
			<< setw(FIELD_7.size()) << intelligence[serialNumber].Plan
			<< endl;
	else
		cout
			<< setw(FIELD_1.size()) << serialNumber + 1
			<< setw(FIELD_2.size()) << intelligence[serialNumber].Company
			<< setw(FIELD_4.size()) << intelligence[serialNumber].Industrial
			<< setw(FIELD_5.size()) << intelligence[serialNumber].NonIndustrial
			<< setw(FIELD_6.size()) << intelligence[serialNumber].Share
			<< setw(FIELD_7.size()) << intelligence[serialNumber].Plan
			<< endl;
}

void printTotals(json sum)
{
	cout
		<< "\n"
		<< left
		<< setw(FIELD_1.size() + FIELD_2.size()) << "Итого:"
		<< setw(FIELD_4.size()) << sum["Industrial"].get<int>()
		<< setw(FIELD_5.size()) << sum["NonIndustrial"].get<int>()
		<< setw(FIELD_6.size()) << sum["Share"].get<float>()
		<< setw(FIELD_7.size()) << sum["Plan"].get<float>()
		<< endl;
}

void printFieldsAndRecords(vector<ZAP>& workIntelligence, int serialNumber, float requestedPlan)
{
	printFields(requestedPlan);
	printRecords(workIntelligence, serialNumber, false);
}

void printFields(float requestedPlan)
{
	cout
		<< "\nСведения о предприятиях,\nв которых выполнение плана по персоналу не менее "
		<< requestedPlan
		<< "%."
		<< endl;
	printCap(false);
}

void printMinCompany(string minCompany)
{
	cout << "\nПредприятие с наименьшей долей непромышленного персонала " << minCompany << endl;
}

using namespace std;
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <fstream>
const string FileName = "MyFile.txt";
struct stClient {
	string AccountNumber, PinCode, ClientName, Phone;
	float AccountBalance;
};
enum enMainMenuOptions {
	eList = 1, eAdd = 2, eFind = 3,
	eDelete = 4, eUpdate = 5, eExit = 6
};
//____________________________________________________________________________________________
stClient ReadClient() {
	stClient cl;
	cout << "Please Enter Account Number: \n";
	getline(cin >> ws, cl.AccountNumber);
	cout << "Please Enter Pin Code: \n";
	getline(cin,cl.PinCode);
	cout << "Please Enter Client Name: \n";
	getline(cin, cl.ClientName);
	cout << "Please Enter Phone: \n";
	getline(cin, cl.Phone);
	cout << "Please Enter Account Balance: \n";
	cin >> cl.AccountBalance;
	return cl;

}
string StoreClientInOneLine(stClient cl,string Delim = "#//#") {
	string Line = "";
	Line += cl.AccountNumber + Delim;
	Line += cl.PinCode + Delim;
	Line += cl.ClientName + Delim;
	Line += cl.Phone + Delim;
	Line += to_string(cl.AccountBalance);
	return Line;
}
vector<string> SplitString(string S1, string Delim = "#//#")
{
	vector<string> vString;
	short pos = 0;
	string sWord;
	while ((pos = S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, pos);
		if (sWord != "")
		{
			vString.push_back(sWord);
		}
		S1.erase(0, pos + Delim.length());
	}
	if (S1 != "")
	{
		vString.push_back(S1);
	}
	return vString;
}
stClient ConvertLineToRecord(vector <string> vSplite) {
	stClient C;
	C.AccountNumber = vSplite[0];
	C.PinCode = vSplite[1];
	C.ClientName = vSplite[2];
	C.Phone = vSplite[3];
	C.AccountBalance = stod(vSplite[4]);
	return C;
}
vector<stClient> LoadClientsFromFile(string Delim = "#//#") {
	ifstream MyFile;
	vector<stClient> vClients;
	vector<string> vWord;
	MyFile.open(FileName, ios::in);
	string Line = "";
	if (MyFile.is_open()) {
		while (getline(MyFile, Line)) {
			if (Line != "") {
				vWord = SplitString(Line);
				stClient Client = ConvertLineToRecord(vWord);
				vClients.push_back(Client);
			}
		}
		MyFile.close();
	}
	return vClients;
}
void AddDataLineToFile(string FileName, string stDataLine) {
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);
	if (MyFile.is_open()) {
		MyFile << stDataLine << endl;
		MyFile.close();
	}
}
//____________________________________________________________________________________________
string ReadString(string Message) {
	string S;
	cout << Message << endl;
	getline(cin >> ws,S);
	return S;
}
//____________________________________________________________________________________________
void PrintHeaderTable(vector<stClient> &vClient) {
	cout << "\t\t\t\tClint List (" << vClient.size() << ") Clients \n";
	cout << "____________________________________________________________________________\n";
	cout << "| Account Number     | Pin Code |  Client Name       | Phone         | Balance  \n";
	cout << "____________________________________________________________________________\n";
}
void PrintClientRecord( stClient Client) {
	cout << "| " << setw(18) << left << Client.AccountNumber;
	cout << "| " << setw(9) << left << Client.PinCode;
	cout << "| " << setw(20) << left << Client.ClientName;
	cout << "| " << setw(14) << left << Client.Phone;
	cout << "| " << setw(10) << left << Client.AccountBalance;
	cout << endl;
}
void ShowAllClients() {
	vector <stClient> vAllClient = LoadClientsFromFile();
	PrintHeaderTable(vAllClient);
	if (vAllClient.empty()) {
		cout << "\t\t\t\t No Clients" << endl;
	}
	else {
		for (stClient &Client : vAllClient) {
			PrintClientRecord(Client);
			cout << endl;
		}
	}
}
//____________________________________________________________________________________________
bool IsClientExist(string AccountNumber, string FileName) {
	vector<stClient> vClients = LoadClientsFromFile();
	for (stClient& C : vClients) {
		if (C.AccountNumber == AccountNumber)
			return true;
	}
	return false;
}
void AddNewClient() {
	char AddMore = 'Y';
	stClient cl;

	do {
		system("cls");
		cout << "--- Add New Client Screen ---\n\n";
		cl.AccountNumber = ReadString("Palese Enter Account Number: \n");
		while (IsClientExist(cl.AccountNumber, FileName)) {
			cout << "\nClient with [" << cl.AccountNumber << "] already exists, Enter another Account Number: ";
			cl.AccountNumber = ReadString("");
		}
		cout << "Enter Pin Code: ";
		getline(cin, cl.PinCode);
		cout << "Enter Name: ";
		getline(cin, cl.ClientName);
		cout << "Enter Phone: ";
		getline(cin, cl.Phone);
		cout << "Enter Account Balance: ";
		cin >> cl.AccountBalance;
		AddDataLineToFile(FileName, StoreClientInOneLine(cl));
		cout << "\nClient Added Successfully. Add more? (Y/N): ";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');
}
//____________________________________________________________________________________________
void FindClientFromAccountNumber(string AccountNumFind) {
	vector<stClient> vAllClients = LoadClientsFromFile();
	vector<stClient> vFoundClient;
	bool Found = false;
	for (stClient& i : vAllClients) {
		if (AccountNumFind == i.AccountNumber) {
			vFoundClient.push_back(i); // هذا من اجل اعطاء حجم الفيكتور الصحيح في دالة الطباعه
			break;
		}
	}
	for (stClient &i : vAllClients) {
		if (AccountNumFind == i.AccountNumber) {
			PrintHeaderTable(vFoundClient);
			PrintClientRecord(i);
			Found = true;
			break;
		}
	}
	if (Found == false)
		cout << "Not Can Be Found This Client\n";
}
//____________________________________________________________________________________________
void SaveClientsToFile(vector<stClient> vClients) {
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	if (MyFile.is_open()) {
		for (stClient& C : vClients) {
			string Line = StoreClientInOneLine(C);
			MyFile << Line << endl;
		}
		MyFile.close();
	}
}
void DeleteClient(string AccNumDel) {
	FindClientFromAccountNumber(AccNumDel);
	char delCli;
	cout << "Are you delete this client ? (Y/N)\n";
	cin >> delCli;
	if (toupper(delCli) == 'Y') {
		vector <stClient> vClients = LoadClientsFromFile();
		vector <stClient> vNew;
		for (stClient& i : vClients) {
			if (AccNumDel != i.AccountNumber) {
				vNew.push_back(i);
			}
		}
		SaveClientsToFile(vNew);
		cout << "\nClient Delete Successfully!\n";
	}
}
//____________________________________________________________________________________________
void UpdateClient(string AccountNumUp) {
	stClient cl;
	FindClientFromAccountNumber(AccountNumUp);
	char delCli;
	cout << "Are you Update Information this client ? (Y/N)\n";
	cin >> delCli;
	if (toupper(delCli) == 'Y') {
		vector <stClient> vClients = LoadClientsFromFile();
		for (stClient& i : vClients) {
			if (AccountNumUp == i.AccountNumber) {
				cout << "Please Enter Pin Code: ";
				getline(cin >> ws, i.PinCode);
				cout << "Please Enter Client Name: ";
				getline(cin, i.ClientName);
				cout << "Please Enter Phone: ";
				getline(cin, i.Phone);
				cout << "Please Enter Account Balance: ";
				cin >> i.AccountBalance;
				break;
			}
		}
		SaveClientsToFile(vClients);
		cout << "\nClient Updated Successfully!\n";
	}
}
//____________________________________________________________________________________________
void PrintManeMinueScreen() {

	system("cls");
	cout << "=========================================================================================\n";
	cout << "\t\t\t\t    Main Menue Screen\n";
	cout << "=========================================================================================\n\n\n\n\n";
}
void ShowMainMenu() {
	system("cls");
	PrintManeMinueScreen();
	cout << "Palese Enter Number Options: \n";
	cout << "[1] Show All Clients\n";
	cout << "[2] Add New Client\n";
	cout << "[3] Find Client\n";
	cout << "[4] Delete Client\n";
	cout << "[5] Update Client\n";
	cout << "[6] Exit\n";
}
void ManueFunction(enMainMenuOptions Option) {
	system("cls");

	switch (Option)
	{
	case enMainMenuOptions::eList:
		ShowAllClients();
		break;

	case enMainMenuOptions::eAdd:
		AddNewClient();
		break;

	case enMainMenuOptions::eFind:
		FindClientFromAccountNumber(ReadString("Please Enter Account Number: "));
		break;

	case enMainMenuOptions::eDelete:
		DeleteClient(ReadString("Please Enter Account Number: "));
		break;

	case enMainMenuOptions::eUpdate:
		UpdateClient(ReadString("Please Enter Account Number: "));
		break;

	case enMainMenuOptions::eExit:
		cout << "\n\n\t\t Goodbye!\n";
		break;
	}
	if (Option != enMainMenuOptions::eExit) {
		cout << "\n\nPress any key to go back to Main Menu...";
		system("pause > 0");
	}
}
int main()
{
	short Op;
	do {
		ShowMainMenu();
		cin >> Op;
		ManueFunction(enMainMenuOptions(Op));
		if (Op == 6)
			break;
	} while (true);
	return 0;
}

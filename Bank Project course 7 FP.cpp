#include<iostream>
#include<iomanip>
#include<vector>
#include<fstream>
#include<conio.h>
#include<cctype>
#include<stirng>
#include"MyLib.h";
using namespace std;
enum enScreens { enShowClientScreen = 1, enAddNewClient = 2, enDeleteClient = 3, enUpdateClient = 4, enFindClient = 5, enExit = 6 };
const string ClientsFileName = "Clients.txt";
struct stClient
{
	string AccountNumber, PinCode, Name, Phone;
	double AccountBalance;
	bool MarkDeleted = false;
};
void MainMenueScreen();
void HoldOnUntilUserInput()
{
	cout << "\n\n";
	cout << "Press any key to go back to the main menu..." << endl;
	_getch();

}
void PrintClientCard(stClient& Client)
{
	cout << "| " << left << setw(18) << Client.AccountNumber;
	cout << "| " << left << setw(12) << Client.PinCode;
	cout << "| " << left << setw(45) << Client.Name;
	cout << "| " << left << setw(15) << Client.Phone;
	cout << "| " << left << setw(20) << Client.AccountBalance << "\n";
}
void ShowClientsOnScreen(string ClientsFileName, vector <stClient>& vClients)
{
	system("cls");
	cout << "\t\t\t\t\t" << "Client List (" << vClients.size() << ") Client(s).\n";
	cout << "_______________________________________________________________________________________________________________________\n\n\n";
	cout << "| " << left << setw(18) << " Account Number  ";
	cout << "| " << left << setw(12) << " Pin Code ";
	cout << "| " << left << setw(45) << " Client Name ";
	cout << "| " << left << setw(15) << " Phone    ";
	cout << "| " << left << setw(20) << " Balance  " << "\n\n";
	cout << "_______________________________________________________________________________________________________________________\n\n\n";

	for (stClient& C : vClients)
	{
		PrintClientCard(C);
	}
	cout << "\n_______________________________________________________________________________________________________________________\n\n\n";
}
stClient ReadNewClient(stClient Client);
string ConvertRecordToLine(stClient Client, string Seperator);
stClient ReadNewClient(stClient Client)
{

	cout << "Enter PinCode? ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin, Client.Name);

	cout << "Enter Phone? ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance? ";
	cin >> Client.AccountBalance;

	return Client;

}
bool FindClientByAccountNumber(vector <stClient> vClients, stClient& Client, string AccountNumber)
{

	for (stClient C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}
string ReadClientAccountNumber()
{
	string AccountNumber;
	cout << "Enter Account Number? ";
	getline(cin >> ws, AccountNumber);
	return AccountNumber;
}
void AddNewClient(vector <stClient>& vClient)
{
	char AddMore = 'y';
	stClient Client;
	bool isClientExisted = false;

	Client.AccountNumber = ReadClientAccountNumber();
	isClientExisted = FindClientByAccountNumber(vClient, Client, Client.AccountNumber);

	do
	{
		if (isClientExisted)
		{
			cout << "\nClient With[" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
			Client.AccountNumber = ReadClientAccountNumber();
		}
		isClientExisted = FindClientByAccountNumber(vClient, Client, Client.AccountNumber);
	} while (isClientExisted);

	Client = ReadNewClient(Client);
	vClient.push_back(Client);

}
void AddNewClientScreen(vector <stClient>& vClient)
{
	system("cls");
	char AddMore = 'y';
	stClient Client;
	bool isClientExisted = false;

	cout << "\n-------------------------------------\n";
	cout << "\tAdd New Client Screen\n";
	cout << "-------------------------------------\n";
	cout << "Adding New Client \n\n";
	do
	{
		AddNewClient(vClient);
		cout << "\n\n Client Added Successfully, do you want to add more client? ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');

}
string ConvertRecordToLine(stClient Client, string Seperator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PinCode + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.Phone + Seperator;
	stClientRecord += to_string(Client.AccountBalance);

	return stClientRecord;
}
void SaveClientsToFile(vector <stClient>& vClient, string ClientsFileName)
{
	fstream MyFile;

	MyFile.open(ClientsFileName, ios::out);

	string DataLine;

	if (MyFile.is_open())
	{
		for (stClient& C : vClient)
		{
			if (C.MarkDeleted == false)
			{
				DataLine = ConvertRecordToLine(C);
				if (DataLine != "")
				{
					MyFile << DataLine << endl;
				}
			}

		}
		MyFile.close();
	}
}
stClient ConvertLineToRecord(vector <string> vRecord)
{
	stClient Client;

	Client.AccountNumber = vRecord[0];
	Client.PinCode = vRecord[1];
	Client.Name = vRecord[2];
	Client.Phone = vRecord[3];
	Client.AccountBalance = stod(vRecord[4]);

	return Client;
}
vector <stClient>  LoadClientsInfoFromFile(string ClientsFileName, string Seperator = "#//#")
{
	fstream MyFile;

	string Line;

	vector <string> vRecord;

	vector <stClient> vClients;

	stClient Client;
	MyFile.open(ClientsFileName, ios::in);

	if (MyFile.is_open())
	{
		while (getline(MyFile, Line))
		{
			if (Line != "")
			{
				vRecord = MyStringLib::vSplitString(Line, Seperator);
				Client = ConvertLineToRecord(vRecord);
				vClients.push_back(Client);
			}
		}
		MyFile.close();
	}
	return vClients;
}
enScreens GetUserMenueChoice()
{
	short Choice = 0;
	while (!(cin >> Choice) || Choice < 1 || Choice>6)
	{
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "Invalide Input , Please try Again :-) ";
	}
	return (enScreens)Choice;
}
void PrintClientCardSeparetly(stClient FoundClient)
{
	cout << "\n\nThe following are The Client Details: \n";
	cout << "-------------------------------------------------\n";
	cout << " Account Number  : " << FoundClient.AccountNumber << "\n";
	cout << " Pin Code        : " << FoundClient.PinCode << "\n";
	cout << " Name            : " << FoundClient.Name << "\n";
	cout << " Phone           : " << FoundClient.Phone << "\n";
	cout << " Account Balance : " << FoundClient.AccountBalance << "\n";
	cout << "-------------------------------------------------\n";

}
bool MarkClientForDeleteByAccountNumber(vector <stClient>& vClients, string AccountNumber)
{
	for (stClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkDeleted = true;
			return true;
		}

	}
	return false;
}
void DeleteClientByAccountNumber(vector <stClient>& vClients, string AccountNumber)
{
	stClient Client;

	char Answer = 'n';

	if (FindClientByAccountNumber(vClients, Client, AccountNumber))
	{
		PrintClientCardSeparetly(Client);
		cout << "\n\nAre you sure you want to delete this client? y/n? ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			MarkClientForDeleteByAccountNumber(vClients, AccountNumber);
			SaveClientsToFile(vClients, ClientsFileName);
			cout << "\n\nClient Deleted Successfully. \n";

			//Refresh 

			vClients = LoadClientsInfoFromFile(ClientsFileName);
		}
	}
	else
	{
		cout << "\n\n Client with Account Number (" << AccountNumber << ") Not Found! \n";
	}
}
void ShowDeleteClientScreen(vector <stClient>& vClients)
{
	system("cls");

	cout << "\n-------------------------------------\n";
	cout << "\tDelete Client Screen\n";
	cout << "-------------------------------------\n\n";

	string AccountNumber = ReadClientAccountNumber();
	DeleteClientByAccountNumber(vClients, AccountNumber);
}
stClient ChangeClientRecord(string AccountNumber)
{
	stClient Client;

	Client.AccountNumber = AccountNumber;

	cout << "\n\n\nEnter PinCode? ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin, Client.Name);

	cout << "Enter Phone? ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance? ";
	cin >> Client.AccountBalance;


	return Client;
}
void UpdateClientByAccountNumber(vector <stClient>& vClients, string AccountNumber)
{
	stClient Client;

	char Answer = 'n';

	if (FindClientByAccountNumber(vClients, Client, AccountNumber))
	{
		PrintClientCardSeparetly(Client);
		cout << "\n\nAre you sure you want to Update this client? y/n? ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			for (stClient& C : vClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			cout << "\n\nClient Updated Successfully. \n";
			SaveClientsToFile(vClients, ClientsFileName);
			//Refresh 
		}
	}
	else
	{
		cout << "\n\n Client with Account Number (" << AccountNumber << ") Not Found! \n";
	}
}
void UpdateClientScreen(vector <stClient>& vClient)
{
	system("cls");
	cout << "\n-------------------------------------\n";
	cout << "\tUpdate Client Screen\n";
	cout << "-------------------------------------\n\n";

	string AccountNumber = ReadClientAccountNumber();
	UpdateClientByAccountNumber(vClient, AccountNumber);
}
void FindClientScreen(vector <stClient>& vClients)
{
	system("cls");

	stClient Client;

	cout << "\n-------------------------------------\n";
	cout << "\tFind Client Screen\n";
	cout << "-------------------------------------\n\n";
	string AccountNumber = ReadClientAccountNumber();

	if (FindClientByAccountNumber(vClients, Client, AccountNumber))
	{
		PrintClientCardSeparetly(Client);
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") Not Found! \n";
	}
}
void ShowExitScreen()
{
	system("cls");
	cout << "---------------------------------\n";
	cout << "\t   Program Ends\n";
	cout << "---------------------------------\n";

}
void ShowScreenBasedOnUserDemand()
{
	cout << "Choose What do you want to do? [1 to 6]? ";

	vector <stClient> vClients;

	vClients = LoadClientsInfoFromFile(ClientsFileName);


	enScreens Choice = GetUserMenueChoice();

	switch (Choice)
	{
	case enScreens::enShowClientScreen:
	{
		ShowClientsOnScreen(ClientsFileName, vClients);
		HoldOnUntilUserInput();
		MainMenueScreen();
	}
	break;
	case enScreens::enAddNewClient:
	{
		AddNewClientScreen(vClients);
		SaveClientsToFile(vClients, ClientsFileName);
		HoldOnUntilUserInput();
		MainMenueScreen();
	}
	break;
	case enScreens::enDeleteClient:
	{
		ShowDeleteClientScreen(vClients);
		HoldOnUntilUserInput();
		MainMenueScreen();
	}
	break;
	case enScreens::enUpdateClient:
	{
		UpdateClientScreen(vClients);
		HoldOnUntilUserInput();
		MainMenueScreen();
	}
	break;
	case enScreens::enFindClient:
	{
		FindClientScreen(vClients);
		HoldOnUntilUserInput();
		MainMenueScreen();
	}
	break;
	case enScreens::enExit:
	{
		ShowExitScreen();
		cout << "\n\n";
		system("Pause");
		return;
	}
	default:
		return;
	}

}
void MainMenueScreen()
{
	system("cls");
	cout << "=========================================\n";
	cout << "\t    Main Menue Screen\n";
	cout << "=========================================\n";
	cout << "\t [1] Show Client Screen. \n";
	cout << "\t [2] Add New Client. \n";
	cout << "\t [3] Delete Client. \n";
	cout << "\t [4] Update Client. \n";
	cout << "\t [5] Find Client. \n";
	cout << "\t [6] Exit. \n";
	cout << "=========================================\n";
	ShowScreenBasedOnUserDemand();
}
int main()
{
	MainMenueScreen();

	return 0;
}

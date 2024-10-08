// Project2_ATM_System.cpp : My Final Solution ... i tried to improve AboHadhoud soluation

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream> 
using namespace std;

/////////    Stuctures    /////////
struct stClientData
{
	string id;
	string pinCode;
	string name;
	string phone;
	double balance;
	bool MarkForDelete = false;
};

/////////      Enums      /////////
enum enATMMenuOption {
	eQucikWithdraw = 1, eNormalWithdraw = 2, eDiposit = 3,
	eCheckBalance = 4, eLogout = 5
};

enum enQuickWithdrawOption {
	e$20 = 1, e$50 = 2, e$100 = 3,
	e$200 = 4, e$400 = 5, e$600 = 6,
	e$800 = 7, e$1000 = 8, eExit = 9
};

///////// Global Veriables  /////////
const string ClientsFileName = "Clients.txt"; 
const string DefaultSeparator = "#//#";

stClientData CurrentClient;

/////////  Some Functions Declaration  /////////
void ShowATMMenu(), Login(), GoBackToATMMenu();

/////////  Generic Functions  /////////
int ReadInt(string Massage)
{
	int num;
	cout << Massage << endl;
	cin >> num;

	return num;
}

double ReadPositiveDouble(string Massage)
{
	double num;
	do
	{
		cout << Massage << endl;
		cin >> num;
	} while (num <= 0);

	return num;
}

string ReadString(string Massage)
{
	string s = "";
	cout << Massage << endl;
	getline(cin >> ws, s);
	return s;
}

bool ValidateNumberBetween(int Number, int MoreThan, int LessThan)
{
	if (Number >= MoreThan && Number <= LessThan)
		return true;
	else
	{
		cout << "\nPlease Enter A Number From " << MoreThan << " To " << LessThan << endl;
		return false;
	}
}

bool Confirm_Y_N(string massage) // generic function to confirm Yes/No Questions
{
	char ConfirmUpdate = 'n';
	cout << endl << massage << endl;
	cin >> ConfirmUpdate;

	return(tolower(ConfirmUpdate) == 'y' ? true : false);
}

vector <string> SplitString(string s, string separator)
{
	//separator is the same as delimeter

	vector <string> vStringWords;

	string word;
	short position = 0;

	while ((position = s.find(separator)) != s.npos)
	{
		word = s.substr(0, position);
		if (word != "")
			vStringWords.push_back(word);

		s.erase(0, position + separator.length());
	}

	if (s != "")
		vStringWords.push_back(s);

	return vStringWords;
}

string ConvertClientDataToLine(stClientData client, string separator = DefaultSeparator)
{
	string line = "";

	line += client.id + separator;
	line += client.pinCode + separator;
	line += client.name + separator;
	line += client.phone + separator;
	line += to_string(client.balance);
	return line;
}

stClientData ConvertClientLineToData(string line, string separator = DefaultSeparator)
{
	vector <string> lineSplitted = SplitString(line, separator);
	stClientData data;

	data.id = lineSplitted[0];
	data.pinCode = lineSplitted[1];
	data.name = lineSplitted[2];
	data.phone = lineSplitted[3];
	data.balance = stod(lineSplitted[4]);

	return data;
}

vector <stClientData> LoadClientsFromFileToVector(string FileName)
{
	fstream clients;
	vector <stClientData> vClients;

	clients.open(ClientsFileName, ios::in);

	if (clients.is_open())
	{
		string line;
		while (getline(clients, line))
		{
			vClients.push_back(ConvertClientLineToData(line));
		}

		clients.close();
	}
	return vClients;
}

void LoadClientsFromVectorToFile(string FileName, vector <stClientData> vClients)
{
	fstream Client;

	Client.open(FileName, ios::out); //overwrite

	if (Client.is_open())
	{
		for (stClientData c : vClients)
		{
			// now we send the vector data to the file while skipping the MarkForDelete data
			if (c.MarkForDelete == false)
				Client << ConvertClientDataToLine(c) << endl;
		}
		Client.close();
	}

}

////////////////////       1.Quick Withdraw        ////////////////////
enQuickWithdrawOption ReadQucikWithdrawOption()
{
	int UserOption;
	cout << "Choose What to Withdraw From [1] to [8]: ";

	do
	{
		cin >> UserOption; 
	} while (ValidateNumberBetween(UserOption, 1, 9) == false);

	return enQuickWithdrawOption(UserOption);
}

int GetWithdrawAmount(enQuickWithdrawOption WithdrawChoice)
{
	switch (WithdrawChoice)
	{
	case e$20:
		return 20;
	case e$50:
		return 50;
	case e$100:
		return 100;
	case e$200:
		return 200;
	case e$400:
		return 400;
	case e$600:
		return 600;
	case e$800:
		return 800;
	case e$1000:
		return 1000;
	case eExit:
		ShowATMMenu();
		return 0;
	}
}

double CalculateNewBalance(double Balance, double AddedAmount)
{
	return Balance + AddedAmount;
}

void QucikWithdraw(vector <stClientData>& vClients)
{

	int WithdrawAmount = GetWithdrawAmount(ReadQucikWithdrawOption());
	int NewBalance = 0;

		
	while (WithdrawAmount > CurrentClient.balance)
	{
		cout << "\nAmount Exceeds the balance, you can withdraw up to " << CurrentClient.balance << "$ only.\n";
		WithdrawAmount = GetWithdrawAmount(ReadQucikWithdrawOption());
	}


	if (Confirm_Y_N("Are You Sure You Want To Perform This Transaction? y/n"))
	{
				
		NewBalance = CalculateNewBalance(CurrentClient.balance, WithdrawAmount * -1);

		for (stClientData& c : vClients)
		{
			if (c.id == CurrentClient.id)
			{
				c.balance = NewBalance;
				break; //quit the for loop cuase no need to continue after we updated the balance
			}
		}
				
		CurrentClient.balance = NewBalance;
		LoadClientsFromVectorToFile(ClientsFileName, vClients);
		cout << "\nWithdraw Done Successfully. New Balance is " << NewBalance << "$\n";
	}
	else
		cout << "\Withdraw Operation is Canceled\n";

}

void PrintQuickWithdrawScreen()
{
	cout << "|=================================|\n";
	cout << "|          Qucik Withdraw         |\n";
	cout << "|=================================|\n";
	cout << "|     [1] 20$       [2] 50$       |\n";
	cout << "|     [3] 100$      [4] 200$      |\n";
	cout << "|     [5] 400$      [6] 600$      |\n";
	cout << "|     [7] 800$      [8] 1000$     |\n";
	cout << "|     [9] Exit                    |\n";
	cout << "|=================================|\n";
	cout << "|Your Balance is $" << left << setw(16) << CurrentClient.balance << "|\n";
	cout << "|=================================|\n";

	vector <stClientData> vClients = LoadClientsFromFileToVector(ClientsFileName);
	
	QucikWithdraw(vClients);
}

////////////////////       2.Normal Withdraw         ////////////////////

int ReadWithdrawAmount()
{
	int WithdrawAmount = 0;
	do
	{
		WithdrawAmount = ReadPositiveDouble("Enter an Amount Mutliple of 5's: ");

	} while (WithdrawAmount % 5 != 0);
	return WithdrawAmount;
}

void NormalWithdraw(vector <stClientData>& vClients)
{
	int NewBalance = 0;
	int WithdrawAmount = ReadWithdrawAmount();

	while (WithdrawAmount > CurrentClient.balance)
	{
		cout << "\nAmount Exceeds the balance, you can withdraw up to " << CurrentClient.balance << "$ only.\n";
		WithdrawAmount = ReadWithdrawAmount();
	}

	if (Confirm_Y_N("Are You Sure You Want To Perform This Transaction? y/n"))
	{

		NewBalance = CalculateNewBalance(CurrentClient.balance, WithdrawAmount * -1);

		for (stClientData& c : vClients)
		{
			if (c.id == CurrentClient.id)
			{
				c.balance = NewBalance;
				break; //quit the for loop cuase no need to continue after we updated the balance
			}
		}

		CurrentClient.balance = NewBalance;
		LoadClientsFromVectorToFile(ClientsFileName, vClients);
		cout << "\nWithdraw Done Successfully. New Balance is " << NewBalance << "$\n";
	}
	else
		cout << "\Withdraw Operation is Canceled\n";

}

void PrintNormalWithdrawScreen()
{
	cout << "|=================================|\n";
	cout << "|         Normal Withdraw         |\n";
	cout << "|=================================|\n";
	cout << "|Your Balance is $" << left << setw(16) << CurrentClient.balance << "|\n";
	cout << "|=================================|\n";

	vector <stClientData> vClients = LoadClientsFromFileToVector(ClientsFileName);

	NormalWithdraw(vClients);
}

////////////////////       3.Deposit         ////////////////////
void Deposit(vector <stClientData>& vClients)
{
	double NewBalance = 0;
	double DepositAmount = 0;


	DepositAmount = ReadPositiveDouble("Enter the Deposit Amount: ");

	if (Confirm_Y_N("Are You Sure You Want To Perform This Transaction? y/n"))
	{

		NewBalance = CalculateNewBalance(CurrentClient.balance, DepositAmount);

		for (stClientData& c : vClients)
		{
			if (c.id == CurrentClient.id)
			{
				c.balance = NewBalance;
				break; //quit the for loop cuase no need to continue after we updated the balance
			}
		}

		CurrentClient.balance = NewBalance;
		LoadClientsFromVectorToFile(ClientsFileName, vClients);
		cout << "\nWithdraw Done Successfully. New Balance is " << NewBalance << "$\n";
	}
	else
		cout << "\Withdraw Operation is Canceled\n";

}

void PrintDepositScreen()
{
	cout << "|=================================|\n";
	cout << "|              Deposit            |\n";
	cout << "|=================================|\n";
	cout << "|Your Balance is $" << left << setw(16) << CurrentClient.balance << "|\n";
	cout << "|=================================|\n";

	vector <stClientData> vClients = LoadClientsFromFileToVector(ClientsFileName);

	Deposit(vClients);
}

////////////////////       4.Check Balance        ////////////////////
void PrintCheckBalanceScreen()
{
	cout << "|=================================|\n";
	cout << "|          Check Balance          |\n";
	cout << "|=================================|\n";
	cout << "|Account Balance: $" << left << setw(15) << CurrentClient.balance << "|\n";
	cout << "|=================================|\n";

}

////////////////////  ATM Menu  ////////////////////
void GoBackToATMMenu()
{
	cout << "Press any key to go back to ATM Main Menu...";
	system("pause>0"); //will pause the program until the user press any key
	ShowATMMenu();
}

void PerformATMMenuOption(enATMMenuOption UserOption)
{
	system("cls");
	switch (UserOption)
	{
	case eQucikWithdraw:
		PrintQuickWithdrawScreen();
		break;
	case eNormalWithdraw:
		PrintNormalWithdrawScreen();
		break;
	case eDiposit:
		PrintDepositScreen();
		break;
	case eCheckBalance:
		PrintCheckBalanceScreen();
		break;
	case eLogout:
		Login();
		return;
	}
	
	GoBackToATMMenu();//this will happen after the 'break', NOT 'return'
}

enATMMenuOption ReadUserOption()
{
	int UserOption;
	cout << "Choose What You Want To Do? (1 - 5)?\n";

	do
	{
		cin >> UserOption; //read the user option ByRef
	} while (ValidateNumberBetween(UserOption, 1, 5) == false);

	return enATMMenuOption(UserOption);
}

void ShowATMMenu()
{
	system("cls");
	cout << "|======================================|\n";
	cout << "|             ATM Main Menu            |\n";
	cout << "|======================================|\n";
	cout << "|Client Name: " << left << setw(25) << CurrentClient.name << "|\n";
	cout << "|======================================|\n";

	cout << "|                                      |\n";
	cout << "|---[1] Quick Withdraw.                |\n";
	cout << "|---[2] Normal Withdraw.               |\n";
	cout << "|---[3] Deposit.                       |\n";
	cout << "|---[4] Check Balance.                 |\n";
	cout << "|---[5] Logout.                        |\n";
	cout << "|                                      |\n";

	cout << "|======================================|\n";

	PerformATMMenuOption(ReadUserOption());
}


//////////////////// Login ////////////////////
bool FindClientByAccountNumAndPINcode(string AcountNum, string PINcode, stClientData& Client)
{

	vector <stClientData> vClients = LoadClientsFromFileToVector(ClientsFileName);

	for (stClientData C : vClients)
	{

		if (C.id == AcountNum && C.pinCode == PINcode)
		{
			Client = C; //Client is ByRef
			return true;
		}

	}
	return false;

}

bool LoadClientInfoToLogin(string AcountNum, string PINcode)
{
	if (FindClientByAccountNumAndPINcode(AcountNum, PINcode, CurrentClient))
		return true;
	else
		return false;
}

void Login()
{
	bool LoginFaild = false;

	string AccountNum, PINcode;
	do
	{
		system("cls");

		cout << "\n---------------------------------";
		cout << "\n           Login Screen          ";
		cout << "\n---------------------------------\n";

		if (LoginFaild)
		{
			cout << "Invlaid Account Number/PIN Code!\n";
		}

		cout << "Enter Account Number: ";
		cin >> AccountNum;

		cout << "Enter PIN Code: ";
		cin >> PINcode;

		LoginFaild = !LoadClientInfoToLogin(AccountNum, PINcode);

	} while (LoginFaild);


	ShowATMMenu();

}

int main()
{
	Login();

	system("pause>0");
	return 0;
}
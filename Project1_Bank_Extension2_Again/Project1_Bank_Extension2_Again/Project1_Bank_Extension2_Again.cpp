// Project1_Bank_Extension2_Again.cpp : My Final Solution ...

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

struct stUserData
{
	string Username;
	string Password;
	int Permissions;
	bool MarkForDelete = false;
};


/////////    Enums    /////////
enum enMainMenuOption {
	eShowClientList = 1, eAddNewClient = 2, eDeleteClient = 3,
	eUpdateClient = 4, eFindClient = 5, eTransactions = 6, 
	eManageUsers = 7, eExit = 8
};

enum enTransactionMenuOption {
	eDeposit = 1, eWithdraw = 2, eTotalBalances = 3, eBackToMainMenu = 4
};

enum enManageUsersMenuOption {
	eShowUsersList = 1, eAddNewUser = 2, eDeleteUser = 3,
	eUpdateUser = 4, eFindUser = 5, eBackToMainMenue = 6
};

enum enMainMenuPermissions {
	pAll = -1, pShowClientList = 1, pAddNewClient = 2, pDeleteClient = 4,
	pUpdateClients = 8, pFindClient = 16, pTransactions = 32, pManageUsers = 64
};

///////// Global Veriables  /////////
const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

const string DefaultSeparator = "#//#";

stUserData CurrentUser;


/////////  Some Functions Declaration  /////////
void ShowMainMenu(); void PrintTransactionScreen(); void PrintManageUsersScreen(); 
void Login(); void ShowAccessDeniedMassage(); void GoBackToMainMenu();
void PrintAddNewClientScreen(bool onlyScreen = false);
void PrintAddNewUserScreen(bool onlyScreen = false);
bool CheckAccessPermission(enMainMenuPermissions Permission);


///////// Generic Functions /////////
int ReadDouble(string Massage)
{
	double num;
	cout << Massage << endl;
	cin >> num;

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

void AddLineToFile(string FileName, string RecordLine)
{
	fstream Myfile;

	Myfile.open(FileName, ios::out | ios::app);

	if (Myfile.is_open())
	{
		Myfile << RecordLine << endl;
		Myfile.close();
	}

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

string ConvertUserDataToLine(stUserData user, string separator = DefaultSeparator)
{
	string line = "";

	line += user.Username + separator;
	line += user.Password + separator;
	line += to_string(user.Permissions);
	return line;
}

stUserData ConvertUserLineToData(string line, string separator = DefaultSeparator)
{
	vector <string> lineSplitted = SplitString(line, separator);
	stUserData data;

	data.Username = lineSplitted[0];
	data.Password = lineSplitted[1];
	data.Permissions = stoi(lineSplitted[2]);

	return data;
}

bool Confirm_Y_N(string massage) // generic function to confirm Yes/No Questions
{
	char ConfirmUpdate = 'n';
	cout << endl << massage << endl;
	cin >> ConfirmUpdate;

	return(tolower(ConfirmUpdate) == 'y' ? true : false);
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

vector <stUserData> LoadUsersFromFileToVector(string FileName)
{
	fstream users;
	vector <stUserData> vUsers;

	users.open(UsersFileName, ios::in);

	if (users.is_open())
	{
		string line;
		while (getline(users, line))
		{
			vUsers.push_back(ConvertUserLineToData(line));
		}

		users.close();
	}
	return vUsers;
}

void LoadUsersFromVectorToFile(string FileName, vector <stUserData> vUsers)
{
	fstream User;

	User.open(FileName, ios::out); //overwrite

	if (User.is_open())
	{
		for (stUserData u : vUsers)
		{
			// now we send the vector data to the file while skipping the MarkForDelete data
			if (u.MarkForDelete == false)
				User << ConvertUserDataToLine(u) << endl;
		}
		User.close();
	}

}

bool FindClientBy_ID(string id, stClientData& Client, vector <stClientData> vClients)
{
	for (stClientData c : vClients)
	{
		if (c.id == id)
		{
			Client = c; //Client is ByRef
			return true;
		}
	}
	return false;
}

bool FindUserBy_Username(string Username, stUserData& User, vector <stUserData> vUsers)
{
	for (stUserData u : vUsers)
	{
		if (u.Username == Username)
		{
			User = u; //User is ByRef
			return true;
		}
	}
	return false;
}


////////////////////  1.Print Client List/Card Functions  ////////////////////
void PrintClientRecord(stClientData client)
{
	cout << "| " << left << setw(14) << client.id;
	cout << "| " << left << setw(9) << client.pinCode;
	cout << "| " << left << setw(39) << client.name;
	cout << "| " << left << setw(11) << client.phone;
	cout << "| " << left << setw(11) << client.balance;
	cout << "| ";
}

void PrintClientsDataList()
{
	if (!CheckAccessPermission(pShowClientList))
	{
		ShowAccessDeniedMassage();
		return;
	}

	vector <stClientData> vClientsData = LoadClientsFromFileToVector(ClientsFileName);

	if ((vClientsData.size() == 0))
		cout << "\nNo Clients Available in The System!\n\n";
	else
	{

		cout << "\t\t\t\t\tClient List: (" << vClientsData.size() << ") Client(s).\n";
		cout << "|===============|==========|========================================|============|============|\n";

		cout << "| " << left << setw(14) << "ID";
		cout << "| " << left << setw(9) << "PIN Code";
		cout << "| " << left << setw(39) << "Client Name";
		cout << "| " << left << setw(11) << "Phone";
		cout << "| " << left << setw(11) << "Balance";
		cout << "| \n";

		cout << "|===============|==========|========================================|============|============|\n";

		for (stClientData data : vClientsData)
		{
			PrintClientRecord(data);
			cout << endl;
		}
		cout << "|===============|==========|========================================|============|============|\n";
	}
}

void PrintClientCard(stClientData Client)
{
	cout << "\n==========| The following are the client details |==========\n";
	cout << "\nID      : " << Client.id;
	cout << "\nPIN Code: " << Client.pinCode;
	cout << "\nName    : " << Client.name;
	cout << "\nPhone   : " << Client.phone;
	cout << "\nBalance : " << Client.balance << "$\n";
	cout << "\n============================================================\n";
}

////////////////////      2.Adding Clients Functions     ////////////////////
bool isIdAvailable(string id, string FileName)
{
	fstream clients;

	clients.open(ClientsFileName, ios::in);

	if (clients.is_open())
	{
		string line;
		stClientData Client;

		while (getline(clients, line))
		{
			Client = ConvertClientLineToData(line);
			if (Client.id == id)
			{
				clients.close();
				return false; //false means the id is NOT available
			}
		}
		clients.close();
	}
	return true; //true means the id is Available
}

stClientData ReadClientData()
{
	stClientData client;


	// Usage of >>std::ws will extract all the whitespace character
	cout << "Enter Client ID: ";
	getline(cin >> ws, client.id);

	while (isIdAvailable(client.id, ClientsFileName) == false)//check if client id is available
	{
		cout << "\nClient With ID Number [" << client.id << "] Already Exists, Enter Another ID:\n";
		getline(cin >> ws, client.id);
	}

	cout << "Enter PIN Code: ";
	getline(cin, client.pinCode);

	cout << "Enter Name: ";
	getline(cin, client.name);

	cout << "Enter Phone Number: ";
	getline(cin, client.phone);

	cout << "Enter Account Balance: ";
	cin >> client.balance;

	return client;
}

void AddNewClient()
{
	stClientData client = ReadClientData(); //getting the info of the new client 

	string line = ConvertClientDataToLine(client); //getting the record line of the new client

	AddLineToFile(ClientsFileName, line); //passing the line to function to add it to file 
}

void AddClients()
{
	char addMore = 'n';

	do
	{
		if (addMore == 'y')
		{
			system("cls");
			PrintAddNewClientScreen(true);//will print the screen only because of 'true'
		}
		cout << "\nAdding New Client...\n\n";

		AddNewClient();

		cout << "\nDo You Want To Add Another Client? \n";
		cin >> addMore;
	} while (tolower(addMore) == 'y');

}

void PrintAddNewClientScreen(bool PrintOnlyScreen)
{
	if (!CheckAccessPermission(pAddNewClient))
	{
		ShowAccessDeniedMassage();
		return;
	}

	bool onlyScreen = PrintOnlyScreen; //the defualt value is false, check declration
	cout << "================================\n";
	cout << "         Add New Client         \n";
	cout << "================================\n";

	if (onlyScreen == false)
		AddClients();
}

////////////////////      3.Delete Clients Functions     ////////////////////
void MarkClientForDelete(string id, vector <stClientData>& vClient)
{
	for (stClientData& c : vClient)//ByRef
	{
		if (c.id == id)
			c.MarkForDelete = true;
	}
}

void DeleteClientBy_ID(string id, vector <stClientData>& vClients)
{
	stClientData Client;

	if (FindClientBy_ID(id, Client, vClients))
	{
		PrintClientCard(Client);

		if (Confirm_Y_N("Are You Sure You Want To Delete This Client?"))
		{
			MarkClientForDelete(id, vClients);

			LoadClientsFromVectorToFile(ClientsFileName, vClients);

			vClients = LoadClientsFromFileToVector(ClientsFileName); //refresh the vector because it is ByRef

			cout << "\nClient Deleted Successfully.\n";
		}
		else
			cout << "\nDeleteing the Client is Canceled\n";

	}
	else
		cout << "\nClient Number [" << id << "] Not Found!\n";

}

void PrintDeleteClientScreen()
{
	if (!CheckAccessPermission(pDeleteClient))
	{
		ShowAccessDeniedMassage();
		return;
	}
	cout << "=================================\n";
	cout << "          Delete Client          \n";
	cout << "=================================\n";

	vector <stClientData> vClients = LoadClientsFromFileToVector(ClientsFileName);

	DeleteClientBy_ID(ReadString("Enter ID Number:"), vClients);
}

////////////////////      4.Update Clients Functions     ////////////////////
stClientData ChangeClientRecord(string id)
{
	stClientData Clients;

	Clients.id = id;

	cout << "\nEnter PIN Code: ";
	getline(cin >> ws, Clients.pinCode);

	cout << "Enter Name: ";
	getline(cin, Clients.name);

	cout << "Enter Phone Number: ";
	getline(cin, Clients.phone);

	cout << "Enter Account Balance: ";
	cin >> Clients.balance;

	return Clients;
}

void UpdateClientBy_ID(string id, vector <stClientData>& vClients)
{
	stClientData Client;

	if (FindClientBy_ID(id, Client, vClients))
	{
		PrintClientCard(Client);

		if (Confirm_Y_N("Are You Sure You Want To Update This Client?"))
		{
			for (stClientData& c : vClients)
			{
				if (c.id == id)
				{
					c = ChangeClientRecord(id);
					break; //quit the for loop cuase no need to continue after we updated the client
				}
			}

			LoadClientsFromVectorToFile(ClientsFileName, vClients);
			cout << "\nClient Updated Successfully.\n";
		}
		else
			cout << "\nUpdating the Client is Canceled\n";

	}
	else
		cout << "\nClient Number [" << id << "] Not Found!\n";

}

void PrintUpdeteClientScreen()
{
	if (!CheckAccessPermission(pUpdateClients))
	{
		ShowAccessDeniedMassage();
		return;
	}

	cout << "=================================\n";
	cout << "          Update Client          \n";
	cout << "=================================\n";

	vector <stClientData> vClients = LoadClientsFromFileToVector(ClientsFileName);
	UpdateClientBy_ID(ReadString("Enter ID Number:"), vClients);
}

////////////////////        5.Find Client Functions      ////////////////////
void FindClientAndPrintIt(string id, vector <stClientData>& vClients)
{
	stClientData Client;
	if (FindClientBy_ID(id, Client, vClients))
	{
		PrintClientCard(Client);
	}
	else
		cout << "\nClient Number [" << id << "] Not Found!\n";
}

void PrintFindClientScreen()
{
	if (!CheckAccessPermission(pFindClient))
	{
		ShowAccessDeniedMassage();
		return;
	}

	cout << "=================================\n";
	cout << "           Find Client           \n";
	cout << "=================================\n";

	vector <stClientData> vClients = LoadClientsFromFileToVector(ClientsFileName);
	FindClientAndPrintIt(ReadString("Enter ID Number:"), vClients);
}

///////// 6.1 Deposit Functions        /////////
double CalculateNewBalance(double Balance, double AddedAmount)
{
	return Balance + AddedAmount;
}

void DepositClientBy_ID(string id, vector <stClientData>& vClients)
{
	stClientData Client;

	if (FindClientBy_ID(id, Client, vClients))
	{
		PrintClientCard(Client);

		double DepositAmount = ReadDouble("Enter Deposit Amount:");
		int NewBalance = 0;

		if (Confirm_Y_N("Are You Sure You Want To Perform This Transaction?"))
		{
			for (stClientData& c : vClients)
			{
				if (c.id == id)
				{
					NewBalance = CalculateNewBalance(Client.balance, DepositAmount);
					c.balance = NewBalance;
					break; //quit the for loop cuase no need to continue after we updated the balance
				}
			}

			LoadClientsFromVectorToFile(ClientsFileName, vClients);
			cout << "\nDeposit Done Successfully. New Balance is " << NewBalance << "$\n";
		}
		else
			cout << "\nDeposit Operation is Canceled\n";

	}
	else
		cout << "\nClient Number [" << id << "] Not Found!\n";

}

void PrintDepositScreen()
{
	cout << "=================================\n";
	cout << "             Deposit             \n";
	cout << "=================================\n";

	vector <stClientData> vClients = LoadClientsFromFileToVector(ClientsFileName);
	DepositClientBy_ID(ReadString("Enter ID Number:"), vClients);
}

///////// 6.2 Withdraw Functions       /////////
void WithdrawClientBy_ID(string id, vector <stClientData>& vClients)
{
	stClientData Client;

	if (FindClientBy_ID(id, Client, vClients))
	{
		PrintClientCard(Client);

		double WithdrawAmount = (ReadDouble("Enter Deposit Amount:"));
		int NewBalance = 0;

		while (WithdrawAmount > Client.balance)
		{
			cout << "\nAmount Exceeds the balance, you can withdraw up to " << Client.balance << "$ only.\n";
			WithdrawAmount = ReadDouble("Enter Deposit Amount:");

		}

		if (Confirm_Y_N("Are You Sure You Want To Perform This Transaction?"))
		{
			for (stClientData& c : vClients)
			{
				if (c.id == id)
				{
					NewBalance = CalculateNewBalance(Client.balance, -1 * (WithdrawAmount)); // * -1 to withdraw
					c.balance = NewBalance;
					break; //quit the for loop cuase no need to continue after we updated the balance
				}
			}

			LoadClientsFromVectorToFile(ClientsFileName, vClients);
			cout << "\nWithdraw Done Successfully. New Balance is " << NewBalance << "$\n";
		}
		else
			cout << "\Withdraw Operation is Canceled\n";

	}
	else
		cout << "\nClient Number [" << id << "] Not Found!\n";

}

void PrintWithdrawScreen()
{
	cout << "=================================\n";
	cout << "             Withdraw            \n";
	cout << "=================================\n";

	vector <stClientData> vClients = LoadClientsFromFileToVector(ClientsFileName);
	WithdrawClientBy_ID(ReadString("Enter ID Number:"), vClients);
}

///////// 6.3 Total Balance Functions  /////////
void PrintClientBalancesRecord(stClientData client)
{
	cout << "| " << left << setw(14) << client.id;
	cout << "| " << left << setw(40) << client.name;
	cout << "| " << left << setw(30) << client.balance;
	cout << "| ";
}

void PrintBalancesList()
{
	vector <stClientData> vClientsData = LoadClientsFromFileToVector(ClientsFileName);
	double TotalBalances = 0; //to calaculate all balances

	if ((vClientsData.size() == 0))
		cout << "\nNo Clients Available in The System!\n\n";
	else
	{

		cout << "\t\t\t\tClient Balances List: (" << vClientsData.size() << ") Client(s).\n";
		cout << "|===============|=========================================|===============================|\n";

		cout << "| " << left << setw(14) << "ID";
		cout << "| " << left << setw(40) << "Client Name";
		cout << "| " << left << setw(30) << "Balance";
		cout << "| \n";

		cout << "|===============|=========================================|===============================|\n";

		for (stClientData data : vClientsData)
		{
			TotalBalances += data.balance; //calaculate all balances
			PrintClientBalancesRecord(data);
			cout << endl;
		}
		cout << "|===============|=========================================|===============================|\n";
	}

	cout << "\t\t\t\tTotal Balances = " << TotalBalances << "$\n";

}

////////////////////       6.Transactions Functions        ////////////////////
void GoBackToTransactionMenu()
{
	cout << "Press any key to go back to Transaction Menu...";
	system("pause>0"); //will pause the program until the user press any key
	PrintTransactionScreen();
}

enTransactionMenuOption ReadUserTransactionOption()
{
	int UserOption;
	cout << "Choose What You Want To Do? (1 - 4)?\n";

	do
	{
		cin >> UserOption; //read the user option ByRef
	} while (ValidateNumberBetween(UserOption, 1, 4) == false);

	return enTransactionMenuOption(UserOption);
}

void PerformTransactionMenuOption(enTransactionMenuOption UserOption)
{
	system("cls");
	switch (UserOption)
	{
	case eDeposit:
		PrintDepositScreen();
		break;
	case eWithdraw:
		PrintWithdrawScreen();
		break;
	case eTotalBalances:
		PrintBalancesList();
		break;
	case eBackToMainMenu:
		ShowMainMenu();
		return;
	}

	GoBackToTransactionMenu();
}

void PrintTransactionScreen()
{
	if (!CheckAccessPermission(pTransactions))
	{
		ShowAccessDeniedMassage();
		GoBackToMainMenu();
		return;
	}

	system("cls");
	cout << "|======================================|\n";
	cout << "|           Transaction Menu           |\n";
	cout << "|======================================|\n";

	cout << "|                                      |\n";
	cout << "|---[1] Deposit.                       |\n";
	cout << "|---[2] Withdraw.                      |\n";
	cout << "|---[3] Total Balances.                |\n";
	cout << "|---[4] Back To Main Menu.             |\n";
	cout << "|                                      |\n";

	cout << "|======================================|\n";

	PerformTransactionMenuOption(ReadUserTransactionOption());
}

///////// 7.0 Permissions Functions /////////
int ReadPermissionsToSet()
{
	int Permissions = 0;
	char Answer = 'n';

	cout << "\nDo You Want To Give This User Full Access? y/n: ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		return -1;

	cout << "\n=== Choose What Access You Want To Give This User ===\n";

	cout << "\nShow Client List Access? y/n: ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += pShowClientList;

	cout << "\nAdd Clients Access? y/n: ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += pAddNewClient;

	cout << "\nDelete Clients Access? y/n: ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += pDeleteClient;

	cout << "\nUpdate Clients Access? y/n: ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += pUpdateClients;

	cout << "\nFind Client Access? y/n: ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += pFindClient;

	cout << "\nTransactions Access? y/n: ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += pTransactions;

	cout << "\nManage Users Access? y/n: ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += pManageUsers;

	return Permissions;

}

bool CheckAccessPermission(enMainMenuPermissions Permission)
{
	if (CurrentUser.Permissions == pAll)
		return true;

	if ((Permission & CurrentUser.Permissions) == Permission)
		return true;
	else
		return false;
}

void ShowAccessDeniedMassage()
{
	cout << "\n-------------------------------------\n";
	cout << "Access Denied!\n";
	cout << "You Don't Have Permission To Do This, \n";
	cout << "Please Contact Your Adminstrator.";
	cout << "\n-------------------------------------\n";
}

///////// 7.1 Users List/Card Functions  /////////

void PrintUserRecord(stUserData user)
{
	cout << "| " << left << setw(20) << user.Username;
	cout << "| " << left << setw(9) << user.Password;
	cout << "| " << left << setw(14) << user.Permissions;
	cout << "| ";
}

void PrintUsersDataList()
{
	vector <stUserData> vUsersData = LoadUsersFromFileToVector(UsersFileName);

	if ((vUsersData.size() == 0))
		cout << "\nNo Users Available in The System!\n\n";
	else
	{

		cout << "\t\t\t\t\tUser List: (" << vUsersData.size() << ") User(s).\n";
		cout << "|=====================|==========|===============|\n";

		cout << "| " << left << setw(20) << "Username";
		cout << "| " << left << setw(9) << "Password";
		cout << "| " << left << setw(14) << "Permissions";
		cout << "| \n";

		cout << "|=====================|==========|===============|\n";

		for (stUserData data : vUsersData)
		{
			PrintUserRecord(data);
			cout << endl;
		}
		cout << "|=====================|==========|===============|\n";
	}
}

void PrintUserCard(stUserData User)
{
	cout << "\n==========|  The following are the User details  |==========\n";
	cout << "\nUsername    : " << User.Username;
	cout << "\nPassword    : " << User.Password;
	cout << "\nPermissions : " << User.Permissions;
	cout << "\n============================================================\n";
}


///////// 7.2 Add User Functions    /////////

bool isUsernameAvailable(string username, string FileName)
{
	fstream users;

	users.open(UsersFileName, ios::in);

	if (users.is_open())
	{
		string line;
		stUserData User;

		while (getline(users, line))
		{
			User = ConvertUserLineToData(line);
			if (User.Username == username)
			{
				users.close();
				return false; //false means the Username is NOT available
			}
		}
		users.close();
	}
	return true; //true means the Username is Available
}

stUserData ReadUserData()
{
	stUserData user;


	// Usage of >>std::ws will extract all the whitespace character
	cout << "Enter Username: ";
	getline(cin >> ws, user.Username);

	while (isUsernameAvailable(user.Username, UsersFileName) == false)//check if username is available
	{
		cout << "\nUser With This Username [" << user.Username << "] Already Exists, Enter Another Username:\n";
		getline(cin >> ws, user.Username);
	}

	cout << "Enter Password: ";
	getline(cin, user.Password);

	user.Permissions = ReadPermissionsToSet(); //read the permissions one by one

	return user;
}

void AddNewUser()
{
	stUserData user = ReadUserData(); //getting the info of the new user 

	string line = ConvertUserDataToLine(user); //getting the record line of the new user

	AddLineToFile(UsersFileName, line); //passing the line to function to add it to file 
}

void AddUsers()
{
	char addMore = 'n';

	do
	{
		if (addMore == 'y')
		{
			system("cls");
			PrintAddNewUserScreen(true); //will print the screen only because of 'true'
		}
		cout << "\nAdding New User...\n\n";

		AddNewUser();

		cout << "\nDo You Want To Add Another User? \n";
		cin >> addMore;
	} while (tolower(addMore) == 'y');

}

void PrintAddNewUserScreen(bool PrintOnlyScreen)
{
	bool onlyScreen = PrintOnlyScreen;
	cout << "================================\n";
	cout << "          Add New User          \n";
	cout << "================================\n";

	if (onlyScreen == false)
		AddUsers();
}

///////// 7.3 Delete User Functions /////////

void MarkUserForDelete(string Username, vector <stUserData>& vUser)
{
	for (stUserData& u : vUser)//ByRef
	{
		if (u.Username == Username)
			u.MarkForDelete = true;
	}
}

void DeleteUserBy_Username(string Username, vector <stUserData>& vUsers)
{
	if (Username == "Admin")
	{
		cout << "\n------You Cannot Delete This User.------\n";
		return;
	}

	stUserData User;

	if (FindUserBy_Username(Username, User, vUsers))
	{

		PrintUserCard(User);

		if (Confirm_Y_N("Are You Sure You Want To Delete This User?"))
		{
			MarkUserForDelete(Username, vUsers);

			LoadUsersFromVectorToFile(UsersFileName, vUsers);

			vUsers = LoadUsersFromFileToVector(UsersFileName); //refresh the vector because it is ByRef

			cout << "\nUser Deleted Successfully.\n";
		}
		else
			cout << "\nDeleteing the User is Canceled\n";

	}
	else
		cout << "\nUsername [" << Username << "] Not Found!\n";

}

void PrintDeleteUserScreen()
{
	cout << "=================================\n";
	cout << "           Delete User           \n";
	cout << "=================================\n";

	vector <stUserData> vUsers = LoadUsersFromFileToVector(UsersFileName);

	DeleteUserBy_Username(ReadString("Enter Username:"), vUsers);
}

///////// 7.4 Update User Functions /////////

stUserData ChangeUserRecord(string Username)
{
	stUserData User;

	User.Username = Username;

	cout << "\nEnter Password: ";
	getline(cin >> ws, User.Password);

	cout << "Enter Permissions: ";
	User.Permissions = ReadPermissionsToSet();


	return User;
}

void UpdateUserBy_Username(string Username, vector <stUserData>& vUsers)
{
	stUserData User;

	if (FindUserBy_Username(Username, User, vUsers))
	{
		PrintUserCard(User);

		if (Confirm_Y_N("Are You Sure You Want To Update This User?"))
		{
			for (stUserData& u : vUsers)
			{
				if (u.Username == Username)
				{
					u = ChangeUserRecord(Username);
					break; //quit the for loop cuase no need to continue after we updated the client
				}
			}

			LoadUsersFromVectorToFile(UsersFileName, vUsers);
			cout << "\nUser Updated Successfully.\n";
		}
		else
			cout << "\nUpdating the User is Canceled\n";

	}
	else
		cout << "\nUsername [" << Username << "] Not Found!\n";

}

void PrintUpdeteUserScreen()
{
	cout << "=================================\n";
	cout << "           Update User           \n";
	cout << "=================================\n";

	vector <stUserData> vUsers = LoadUsersFromFileToVector(UsersFileName);
	UpdateUserBy_Username(ReadString("Enter Username:"), vUsers);
}

///////// 7.5 Find User Functions   /////////

void FindUserAndPrintIt(string Username, vector <stUserData>& vUsers)
{
	stUserData User;
	if (FindUserBy_Username(Username, User, vUsers))
	{
		PrintUserCard(User);
	}
	else
		cout << "\nUsername [" << Username << "] Not Found!\n";
}

void PrintFindUserScreen()
{
	cout << "=================================\n";
	cout << "            Find User            \n";
	cout << "=================================\n";

	vector <stUserData> vUsers = LoadUsersFromFileToVector(UsersFileName);
	FindUserAndPrintIt(ReadString("Enter Username:"), vUsers);
}

////////////////////       7.Manage Users Functions        ////////////////////

void GoBackToManageUsersMenu()
{
	cout << "Press any key to go back to Manage Users Menu...";
	system("pause>0"); //will pause the program until the user press any key
	PrintManageUsersScreen();
}

enManageUsersMenuOption ReadUserManageUsersOption()
{
	int UserOption;
	cout << "Choose What You Want To Do? (1 - 6)?\n";

	do
	{
		cin >> UserOption; //read the user option ByRef
	} while (ValidateNumberBetween(UserOption, 1, 6) == false);

	return enManageUsersMenuOption(UserOption);
}

void PerformManageUsersMenuOption(enManageUsersMenuOption UserOption)
{

	system("cls");
	
	switch (UserOption)
	{
	case eShowUsersList:
		PrintUsersDataList();
		break;
	case eAddNewUser:
		PrintAddNewUserScreen();
		break;
	case eDeleteUser:
		PrintDeleteUserScreen();
		break;
	case eUpdateUser:
		PrintUpdeteUserScreen();
		break;
	case eFindUser:
		PrintFindUserScreen();
		break;
	case eBackToMainMenue:
		ShowMainMenu();
		return;
	}

	GoBackToManageUsersMenu();

}

void PrintManageUsersScreen()
{
	if (!CheckAccessPermission(pManageUsers))
	{
		ShowAccessDeniedMassage();
		GoBackToMainMenu();
		return;
	}

	system("cls");
	cout << "|======================================|\n";
	cout << "|              Manage Users            |\n";
	cout << "|======================================|\n";

	cout << "|                                      |\n";
	cout << "|---[1] Users List.                    |\n";
	cout << "|---[2] Add New User.                  |\n";
	cout << "|---[3] Delete User.                   |\n";
	cout << "|---[4] Update User.                   |\n";
	cout << "|---[5] Find User.                     |\n";
	cout << "|---[6] Back To Main Menu.             |\n";
	cout << "|                                      |\n";

	cout << "|======================================|\n";

	PerformManageUsersMenuOption(ReadUserManageUsersOption());

}

//////////////////// Main Menu ////////////////////
void PrintEndScreen()
{
	cout << "==================================\n";
	cout << "           Program Ends           \n";
	cout << "==================================\n";
}

void GoBackToMainMenu()
{
	cout << "Press any key to go back to Main Menu...";
	system("pause>0"); //will pause the program until the user press any key
	ShowMainMenu();
}

enMainMenuOption ReadUserOption()
{
	int UserOption;
	cout << "Choose What You Want To Do? (1 - 8)?\n";

	do
	{
		cin >> UserOption; //read the user option ByRef
	} while (ValidateNumberBetween(UserOption, 1, 8) == false);

	return enMainMenuOption(UserOption);
}

void PerformMainMenuOption(enMainMenuOption UserOption)
{

	system("cls");
	switch (UserOption)
	{
	case eShowClientList:
		PrintClientsDataList();
		break;
	case eAddNewClient:
		PrintAddNewClientScreen();
		break;
	case eDeleteClient:
		PrintDeleteClientScreen();
		break;
	case eUpdateClient:
		PrintUpdeteClientScreen();
		break;
	case eFindClient:
		PrintFindClientScreen();
		break;
	case eTransactions:
		PrintTransactionScreen();
		return; //look carefully this is not 'break', it is 'return' to get out the whole function
	case eManageUsers:
		PrintManageUsersScreen();
		return;
	case eExit:
		//PrintEndScreen(); //comment out the old func
		Login();
		return; 
	}

	GoBackToMainMenu(); //this will happen after all the 'break', NOT 'return'
}

void ShowMainMenu()
{
	system("cls");
	cout << "|======================================|\n";
	cout << "|              Main Menu               |\n";
	cout << "|======================================|\n";

	cout << "|                                      |\n";
	cout << "|---[1] Show CLient List.              |\n";
	cout << "|---[2] Add New Client.                |\n";
	cout << "|---[3] Delete Client.                 |\n";
	cout << "|---[4] Update Client.                 |\n";
	cout << "|---[5] Find Client.                   |\n";
	cout << "|---[6] Transactions.                  |\n";
	cout << "|---[7] Manage Users.                  |\n";
	cout << "|---[8] Logout.                        |\n";
	cout << "|                                      |\n";

	cout << "|======================================|\n";

	PerformMainMenuOption(ReadUserOption());
}

//////////////////// Login ////////////////////
bool FindUserByUsernameAndPassword(string Username, string Password, stUserData& User)
{

	vector <stUserData> vUsers = LoadUsersFromFileToVector(UsersFileName);

	for (stUserData U : vUsers)
	{

		if (U.Username == Username && U.Password == Password)
		{
			User = U;
			return true;
		}

	}
	return false;

}

bool LoadUserInfoToLogin(string Username, string Password)
{
	if (FindUserByUsernameAndPassword(Username, Password, CurrentUser))
		return true;
	else
		return false;
}

void Login()
{
	bool LoginFaild = false;

	string Username, Password;
	do
	{
		system("cls");

		cout << "\n---------------------------------";
		cout << "\n           Login Screen          ";
		cout << "\n---------------------------------\n";

		if (LoginFaild)
		{
			cout << "Invlaid Username/Password!\n";
		}

		cout << "Enter Username: ";
		cin >> Username;

		cout << "Enter Password: ";
		cin >> Password;

		LoginFaild = !LoadUserInfoToLogin(Username, Password);

	} while (LoginFaild);

	ShowMainMenu();

}

int main()
{
	Login();

	system("pause>0");
	return 0;
}

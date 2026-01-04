#include <iostream>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <string>
#include <time.h>
using namespace std;

//Declare data structure
struct PRODUCT {
	char code[4];
	char name[30];
	double price;
};

//Declare function prototypes
void AlaCarte(int& count, int qty[], double& totalpriceAC, bool flagAC[], PRODUCT alacarte[]);
void Set(double priceSet[][4], double& totalpriceSet, bool flagS[][4], int qtyS[][4]);
char ulang();
double checkout(double totalpriceAC, double totalpriceS, double& totalprice, double& baki,
	double& bayarCash, string& method, double priceScan, char scan, double& grandttl, double& harga);
void receipt(double disc, double totalprice, int qty[], bool flagAC[], int count, PRODUCT alacarte[],
	bool flagS[][4], double priceSet[][4], string setName[][4], int qtyS[][4], ofstream& fout,
	double baki, double bayarCash, string method, double grandttl, double harga);
void validation();
void scanOrder(ifstream& fin, PRODUCT alacarte[], bool flagAC[], bool flagA[][4], int qty[], int qtyS[][4],
	double priceSet[][4], double& priceScan);

int main()
{
	char pilih, scan;
	string SorAC;

	//I/O file variables
	ifstream fin;
	ofstream fout;
	double priceScan = 0;

	//Set variables
	string setName[2][4] = { {"MEE GORENG + TOK ABAH SPECIAL KOKO", "NASI GORENG + TOK ABAH SPECIAL KOKO",
								"LAMB CHOP + TOK ABAH SPECIAL KOKO", "CHICKEN CHOP + TOK ABAH SPECIAL KOKO"},
							{"MEE GORENG + TOK ABAH HOT KOKO", "NASI GORENG + TOK ABAH HOT KOKO",
							"LAMB CHOP + TOK ABAH HOT KOKO", "CHICKEN CHOP + TOK ABAH HOT KOKO"} };
	double priceSet[2][4] = { { 10.50 , 12.00, 12.50, 14.00},
							{9.50, 11.00, 11.50 ,13.00}, };
	double totalpriceSet = 0;
	bool flagS[2][4] = { false };
	int qtyS[2][4] = { 0 };

	//Ala carte variables
	PRODUCT alacarte[14] = { {"001", "MEE GORENG", 5.50},
						{"002", "NASI GORENG", 7.00},
						{"003", "LAMB CHOP", 15.00},
						{"004", "CHICKEN CHOP", 7.50},
						{"005", "SPAGHETTI", 9.00},
						{"006", "CHOCOLATE CAKE", 11.30},
						{"007", "CROFFLE", 7.20},
						{"008", "WAFFLE", 4.00},
						{"009", "PARFAIT", 8.90},
						{"010", "TOK ABAH SPECIAL KOKO", 6.00},
						{"011", "TOK ABAH HOT KOKO", 5.00},
						{"012", "TOK ABAH ICED KOKO", 5.50},
						{"013", "TEH O AIS", 2.00},
						{"014", "TEH O PANAS", 2.50} };
	int count = 0;
	int qty[14] = { 0 };
	bool flagAC[14] = { false };
	double totalpriceAC = 0;

	//Checkout variables
	double totalprice = 0;
	double discount;
	string method;
	double bayarCash = 0;
	double baki = 0;
	double grandttl = 0;
	double harga = 0;

	//Cue greetings
	cout << "\nWelcome to Kedai Tok Abah!\nPress Enter to get started. ";
	getline(cin, SorAC);

	do {
		//Prompt user to input answer of choice
		cout << "\nDo you have an order to scan? (Y/N) : ";
		cin >> scan;

		//If user wants to scan their order,
		if (toupper(scan) == 'Y') {
			//Function to read file content from
			scanOrder(fin, alacarte, flagAC, flagS, qty, qtyS, priceSet, priceScan);

			//Function to retrieve discount
			discount = checkout(totalpriceAC, totalpriceSet, totalprice, baki, bayarCash, method, priceScan, scan, grandttl, harga);

			//Function to write file to
			receipt(discount, totalprice, qty, flagAC, count, alacarte, flagS, priceSet, setName, qtyS, fout, baki, bayarCash, method, grandttl, harga);

			//Exit program
			exit;
		}

		//If user does not want to scan an order,
		else if (toupper(scan) == 'N') {
			do {
				do {
					//Alternate way instead of cin.ignore()
					getline(cin, SorAC);

					//Prompt user to order ala carte or set
					cout << "\nDo you want to order ala carte or set ? : ";
					getline(cin, SorAC);

					//If user wants to order set,
					if (SorAC == "set" || SorAC == "SET") {
						//Function to get set orders
						Set(priceSet, totalpriceSet, flagS, qtyS);
					}

					//If user wants to order ala carte,
					else if (SorAC == "ala carte" || SorAC == "ALA CARTE") {
						//Function to get ala carte orders
						AlaCarte(count, qty, totalpriceAC, flagAC, alacarte);
					}

					//Validation for incorrect input
					else
						cout << "Invalid option. Press enter to try again.";

				} while (SorAC != "set" && SorAC != "SET" && SorAC != "ala carte" && SorAC != "ALA CARTE");

				do {
					//Prompt user whether to make a new order
					pilih = ulang();

					//If user enters N,
					if (toupper(pilih) == 'N') {
						//Proceeds to checkout() function
						break;
					}
					//Validation for incorrect input
					else if (toupper(pilih) != 'Y') {
						cout << "Invalid option. Please enter 'Y' or 'N'. ";
					}

				} while ((toupper(pilih) != 'Y'));
			} while (toupper(pilih) == 'Y');

			//Function to calculate total price and to retrieve discount
			discount = checkout(totalpriceAC, totalpriceSet, totalprice, baki, bayarCash, method, priceScan, scan, grandttl, harga);

			//Funtion to produce receipt
			receipt(discount, totalprice, qty, flagAC, count, alacarte, flagS, priceSet, setName, qtyS, fout, baki, bayarCash, method, grandttl, harga);
		}

		//Validation for incorrect input
		else {
			cout << "Invalid option. Please enter 'Y' or 'N'. \n";
		}

	} while (toupper(scan) != 'Y' && toupper(scan) != 'N');

	//Close files
	fin.close();
	fout.close();
	return 0;
}

void AlaCarte(int& count, int qty[], double& totalpriceAC, bool flagAC[], PRODUCT alacarte[])
{
	//Declare and initialize variables
	int codeAC;
	double harga;
	
	//Ala Carte Menu
	cout << "\n-----------------------------------------------------------------";
	cout << "\n" << "\tCODE" << setw(10) << "ITEM\t" << setw(28) << "PRICE(RM)\n";
	cout << "------------------------------------------------------------------";
	cout << "\n\t001\t MEE GORENG\t\t  RM 5.50\n";
	cout << "\t002\t NASI GORENG\t\t  RM 7.00\n";
	cout << "\t003\t LAMB CHOP\t\t  RM 15.00\n";
	cout << "\t004\t CHICKEN CHOP\t\t  RM 7.50\n";
	cout << "\t005\t SPAGHETTI\t\t  RM 9.00\n";
	cout << "\t006\t CHOCOLATE CAKE\t\t  RM 11.30\n";
	cout << "\t007\t CROFFLE\t\t  RM 7.20\n";
	cout << "\t008\t WAFFLE\t\t\t  RM 4.00\n";
	cout << "\t009\t PARFAIT\t\t  RM 8.90\n";
	cout << "\t010\t TOK ABAH SPECIAL KOKO\t  RM 6.00\n ";
	cout << "\t011\t TOK ABAH HOT KOKO\t  RM 5.00\n ";
	cout << "\t012\t TOK ABAH ICED KOKO\t  RM 5.50\n ";
	cout << "\t013\t TEH O AIS \t\t  RM 2.50\n ";
	cout << "\t014\t TEH O PANAS\t\t  RM 2.00\n ";
	cout << "-----------------------------------------------------------------";

	//Cue instructions for users on how to order and to exit
	cout << "\n\nPlace your order by inserting the item code (Enter 999 to exit) : \n\n";
	do {

		//Users input item code
		cout << "Order " << count + 1 << " : ";
		cin >> codeAC;

		//Program will exit once 999 is entered
		if (codeAC == 999) break;

		//Validation for incorrect input
		if (codeAC < 1 || codeAC > 14) {
			validation();
			continue;
		}

		//Increment count by 1
		count++;

		//For successful orders, 
		if (codeAC != 999) {
			//Total up quantity of user's input code by 1
			qty[codeAC - 1]++;

			//Flag user's input code to true
			flagAC[codeAC - 1] = true;

			//Find price based on user's input code
			harga = alacarte[codeAC - 1].price;

			//Total up at totalpriceAC
			totalpriceAC += harga;
		}

	} while (codeAC != 999);

	//Shows total price for ala carte
	cout << "Total price Ala Carte is RM " << setprecision(2) << fixed << totalpriceAC;
}

void Set(double priceSet[][4], double& totalpriceSet, bool flagS[][4], int qtyS[][4]) {

	//Declare set variables 
	char pilih = '\0';
	char foodcode[4];
	char drinkcode[4];
	int fcode, dcode;
	double harga;

	//Show menu Set
	cout << endl << setw(60) << "OUR LIMITED TIME SET DEAL\n";
	cout << setw(63) << "Save RM 1 for each set purchase!\n";
	cout << "\n-----------------------------------------------------------------------------------------------------";
	cout << endl << setw(25) << "MEE GORENG" << setw(21) << "NASI GORENG"
		<< setw(23) << "CHICKEN CHOP" << setw(15) << "SPAGHETTI";
	cout << endl << setw(21) << "001" << setw(21) << "002" << setw(22) << "003" << setw(17) << "004";
	cout << "\n-----------------------------------------------------------------------------------------------------";
	cout << "\n" << "  TOK ABAH";
	cout << "\nSPECIAL KOKO" << setw(10) << "10.50" << setw(21) << "12.00" << setw(22) << "12.50" << setw(17) << "14.00";
	cout << "\n    010\n\n  TOK ABAH";
	cout << "\n  HOT KOKO" << setw(12) << "9.50" << setw(21) << "11.00" << setw(22) << "11.50" << setw(17) << "13.00";
	cout << "\n    011";
	cout << "\n-----------------------------------------------------------------------------------------------------";

	do {
		do {
			//Enter food and drink code
			cout << "\n\nEnter food code : ";
			cin >> foodcode;
			cout << "Enter drink code : ";
			cin >> drinkcode;

			//Convert to integer to get the food and drink code
			fcode = atoi(foodcode) - 1;
			dcode = atoi(drinkcode) - 10;

			//Validation for incorrect input
			if (fcode < 0 || fcode >= 4 || dcode < 0 || dcode >= 2) {
				validation();
				continue;
			}
		} while ((fcode < 0 || fcode >= 4 || dcode < 0 || dcode >= 2));

		//If correct input,
		//Flag designated code to true
		flagS[dcode][fcode] = true;

		//Total up quantity by 1
		qtyS[dcode][fcode]++;

		//Find price
		harga = priceSet[dcode][fcode];

		//Total up price
		totalpriceSet += harga;

		//Prompt user if they want to add a set order
		cout << "\nDo you want to make another set order? (Y/N) : ";
		do {
			cin >> pilih;
			//If no,
			if (toupper(pilih) == 'N') {
				//Skip and proceed to ulang()
				break;
			}
			//Validation for incorrect input
			else if (toupper(pilih) != 'Y') {
				cout << "Invalid option. Please enter 'Y' or 'N'." << endl;
			}

		} while (toupper(pilih) != 'Y');
	} while (toupper(pilih) == 'Y');

	//Show total price for set
	cout << "Total price Set is RM " << setprecision(2) << fixed << totalpriceSet;
}

//Function to prompt user if they want to make a new order
char ulang() {
	char p;
	cout << "\n\nDo you want to make a new order? (Y/N) : ";
	cin >> p;
	return p;
}

double checkout(double totalpriceAC, double totalpriceS, double& totalprice, double& baki,
	double& bayarCash, string& method, double priceScan, char scan, double& grandttl, double& harga) {

	//Declare variables
	char reedemans, pilihh = '\0';
	string code;
	double disc = 0;

	//grandttl and harga is assigned by selection of whether user scan order or not
	grandttl = harga = toupper(scan) == 'Y' ? priceScan : totalprice = totalpriceS + totalpriceAC;

	//Output total price
	cout << "\nTotal Price : RM " << setprecision(2) << fixed << harga;

	do {
		//Prompt user if there is a code to redeem 
		cout << "\nDo you have a code to redeem? (Y/N) : ";
		cin >> reedemans;

		//Validation for incorrect input
		if (toupper(reedemans) != 'Y' && toupper(reedemans) != 'N')
			cout << "Invalid option.Please enter 'Y' or 'N'.\n";

	} while (toupper(reedemans) != 'Y' && (toupper(reedemans) != 'N'));

	//If user wants to redeem code,
	if (toupper(reedemans) == 'Y') {
		do {
			//Input code
			cout << "\nEnter code : ";
			cin >> code;

			//If code is correct,
			if (code == "LOL") {
				//Discount of 10% is given
				disc = 0.1;
				//Break from if-else 
				break;
			}

			//Validation for invalid redeem code 
			else {
				//Prompt user to enter another code or exit
				cout << "Invalid code. Enter again or exit? (A/E) : ";
				cin >> pilihh;

				//Exit input redeem code
				if (toupper(pilihh) == 'E') break;
			}

			//A to repeat input code
		} while (toupper(pilihh) == 'A');

		//Total price after redeeming code
		grandttl = harga - (harga * disc);
		cout << "Total price is now : RM " << setprecision(2) << fixed << grandttl;
	}

	do {
		//Prompt user to choose a payment method
		cout << "\n\nChoose your payment method (CARD or CASH): ";
		cin >> method;

		//If cash,
		if (method == "CASH" || method == "cash") {
			//Prompt user to give amount
			cout << "Amount you want to give is : RM ";
			do {
				cin >> bayarCash;
				//Show amount user paid
				cout << "\nYou paid RM " << setprecision(2) << fixed << bayarCash;

				//If bayarCash > grandttl,
				if (bayarCash > grandttl) {
					//Calculate balance
					baki = bayarCash - grandttl;
					//Output balance
					cout << "\nYour balance is : RM " << baki;
				}

				//If bayarCash < grandttl
				else {
					//Output insufficient amount
					cout << "\nInsufficient amount! Please give new amount : RM ";
				}
				//Will loop until bayarCash > grandttl
			} while (bayarCash < grandttl);
		}

		//If card,
		else if (method == "CARD" || method == "card")
			cout << "Please tap your card!";

		//Validation for invalid input
		else
			cout << "Invalid option. Please enter CARD or CASH.";

	} while (method != "CASH" && method != "cash" && method != "CARD" && method != "card");
	return disc;
}

void receipt(double disc, double totalprice, int qty[], bool flagAC[], int count, PRODUCT alacarte[],
	bool flagS[][4], double priceSet[][4], string setName[][4], int qtyS[][4], ofstream& fout,
	double baki, double bayarCash, string method, double grandttl, double harga) {

	//Initialize variable
	char pilih = '\0';

	//Open file
	fout.open("receipt.txt");

	//Get current data/time with respect to system.
	time_t now = time(0);
	char nowF[30];
	//Convert it into string.
	ctime_s(nowF, sizeof(nowF), &now);

	//Prompt user whether they want a physical copy of the receipt
	cout << "\n\nDo you want a physical copy of the receipt? (Y/N) : ";

	do {
		cin >> pilih;

		//If No,
		if (toupper(pilih) == 'N') {
			//Skip and only output at terminal
			break;
		}
		//If Yes,
		else if (toupper(pilih) == 'Y') {
			//Output receipt at file
			fout << "------------------------------------------------------------------";
			fout << "\n\t\t\t\t\tKedai Tok Abah";
			fout << "\n------------------------------------------------------------------";
			fout << "\nAddress " << setw(6) << ": jalan ayah saya\nPhone number : 01234567891\n";
			fout << "\nServer : Sarah";
			//Print local date and time.
			fout << setw(53) << nowF;
			fout << "------------------------------------------------------------------\n";
			fout << setw(10) << "Qty" << setw(17) << "Description" << setw(34) << "Cost";
			fout << "\n------------------------------------------------------------------";

			//For loop is used to output the items the user has ordered based on flagAC array
			for (int i = 0; i < 14; i++) {
				//If flagAC of that current index is true,
				if (flagAC[i]) {
					//Output the quantity, name and price
					fout << "\n\t\t" << left << qty[i] << "\t\t" << setw(40) << alacarte[i].name << "RM " << setprecision(2) << fixed << alacarte[i].price * qty[i];
				}
			}

			//For loop is used to output the items the user has ordered based on flagS array
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 4; j++) {
					//If flagS of that current index is true,
					if (flagS[i][j]) {
						//Output the quantity, name and price
						fout << "\n\t\t" << left << qtyS[i][j] << "\t\t" << setw(40) << setName[i][j] << "RM " << setprecision(2) << fixed << priceSet[i][j] * qtyS[i][j];
					}
				}
			}

			fout << "\n------------------------------------------------------------------";

			//Show net total, discount if redeemed and grand total
			fout << "\nNet Total : RM " << harga
				<< "\nDiscount redeemed : " << setprecision(0) << fixed << disc * 100 << "%"
				<< "\nGrand Total : RM " << setprecision(2) << fixed << grandttl;

			//Show cash and balance if method used was cash
			if (method == "cash" || method == "CASH") {
				fout << "\n\nCash : RM " << bayarCash
					<< "\nBalance : RM " << baki;
			}
			//Show card was approved if method is card
			else
				fout << "\n\nCARD : APPROVED";

			//Cue thankyous
			fout << "\n\nThank You for Dining with Us! Please come again!\nUse the code 'LOL' to get 10% off your next order!";
			fout << "\n\nWant to order faster? Just write down the code of the item and\nquantity of said item ;)";
			fout << "\nExample: 010 2\n\t\t 002011 1\n\n";
		}

		//Validation for incorrect input
		else
			cout << "Invalid option. Please enter 'Y' or 'N'. ";

	} while (toupper(pilih) != 'Y');

	//If No, output receipt at terminal
	cout << "\n\n------------------------------------------------------------------";
	cout << "\n\t\t\tKedai Tok Abah";
	cout << "\n------------------------------------------------------------------";
	cout << "\nAddress : jalan ayah saya\nPhone number : 01234567891\n";
	cout << "\nServer : Sarah";
	// print local date and time.
	cout << setw(53) << nowF;
	cout << "------------------------------------------------------------------\n";
	cout << setw(10) << "Qty" << setw(17) << "Description" << setw(34) << "Cost";
	cout << "\n------------------------------------------------------------------";

	//For loop is used to output the items the user has ordered based on flagAC array
	for (int i = 0; i < 14; i++) {
		//If flagAC of that current index is true,
		if (flagAC[i]) {
			//Output the quantity, name and price
			cout << "\n\t" << left << qty[i] << "\t" << setw(40) << alacarte[i].name << "RM " << setprecision(2) << fixed << alacarte[i].price * qty[i];
		}
	}

	//For loop is used to output the items the user has ordered based on flagS array
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			//If flagS of that current index is true,
			if (flagS[i][j]) {
				//Output the quantity, name and price
				cout << "\n\t" << left << qtyS[i][j] << "\t" << setw(40) << setName[i][j] << "RM " << setprecision(2) << fixed << priceSet[i][j] * qtyS[i][j];
			}
		}
	}

	cout << "\n------------------------------------------------------------------";

	//Show net total, discount if redeemed and grand total
	cout << "\nNet Total : RM " << harga
		<< "\nDiscount redeemed : " << setprecision(0) << fixed << disc * 100 << "%"
		<< "\nGrand Total : RM " << setprecision(2) << fixed << grandttl;

	//Show cash and balance if method used was cash
	if (method == "cash" || method == "CASH") {
		cout << "\n\nCash : RM " << bayarCash
			<< "\nBalance : RM " << baki;
	}
	//Show card was approved if method is card
	else
		cout << "\n\nCARD : APPROVED";

	//Cue thankyous
	cout << "\n\nThank You for Dining with Us! Please come again!\nUse the code 'LOL' to get 10% off your next order!";
	cout << "\n\nWant to order faster? Just write down the code of the item and\nquantity of said item. ;)";
	cout << "\nExample: 010 2\n\t 002011 1\n\n";
}

//Validate function
void validation()
{
	cout << "Invalid option!\n";
	cout << "Please insert the correct code.\n";
}

void scanOrder(ifstream& fin, PRODUCT alacarte[], bool flagAC[], bool flagS[][4], int qty[], int qtyS[][4], double priceSet[][4], double& priceScan) {
	//Declare variables
	char code[7], code1[4], code2[4];
	int quantity;
	int i = 0;
	//Open file
	fin.open("order.txt");

	//Testing for opening file
	if (!fin)
		cout << "File open failure";

	do {
		//Read first 2 inputs from file
		fin >> code;
		fin >> quantity;

		//If first input has 3 characters,
		if (strlen(code) == 3) {
			for (int j = 0; j < 14; j++) {
				//Match input to alacarte's array of codes
				if (strcmp(code, alacarte[j].code) == 0) {
					//Flag the designated order to true
					flagAC[j] = true;

					//Total up quantity in qty array
					qty[j] += quantity;

					//Total up prize into priceScan
					priceScan += alacarte[j].price;
				}
			}
		}

		//If first input has 6 characters,
		if (strlen(code) == 6) {
			//Allocate first 3 characters into code1 and the other 3 into code2
			code1[0] = code[0];
			code1[1] = code[1];
			code1[2] = code[2];
			code2[0] = code[3];
			code2[1] = code[4];
			code2[2] = code[5];

			//Convert code1 and code2 into integer
			int fcode = atoi(code1) - 1;
			int dcode = atoi(code2) - 10;

			//Flag the designated set code to true
			flagS[dcode][fcode] = true;

			//Total up quantity in qtyS array
			qtyS[dcode][fcode] += quantity;

			//Total up price into priceScan
			priceScan += priceSet[dcode][fcode];
		}
	} while (fin);
}
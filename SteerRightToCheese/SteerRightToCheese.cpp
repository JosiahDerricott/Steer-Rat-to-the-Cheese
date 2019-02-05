////
// Name: Josiah Derricott
// Section: B
// Program Name: Steer the Rat to the Right Hole
//
// Description: This program starts the player out with a certain number of cash stored as a double.
// The user can then use that cash to bet on which hole the user thinks the randomly placed cheese is, if the
// user is right they get double their money, but if the user is wrong they lose their bet. There is also a
// random encounter that may occur where a stranger will offer to increase the bet with a risk to the user.
////

#include "pch.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

//Function Prototypes 
void printEndOfGame(double playerCash, double amountLost, double amountWon);
void printCurrentAmounts(double playerCash, double amountLost, double amountWon);
void printRules();
void printErrorToConsole(string errorMessage);
void checkIntAndExit(string input, const int& playerCash, const int& amountLost, const int& amountWon);
double handleRandomEncounter(double currentBet, double strangerCashIncrease, const int& playerCash, const int& amountLost, const int& amountWon);
bool isNotDigit(char c);

int main()
{
	cout << "My name: Josiah Derricott\n"
		<< "Program 2: Steer the Rat to the Right Hole\n"
		<< "-----------------------------------------------------\n\n";

	printRules();

	//Player attributes
	double playerCash = 50.0, amountLost = 0.0, amountWon = 0.0, userBetAmount = 0.0;
	int holeGuess = 0;

	//Vars for random number generator
	int numberOfNums = 3, firstNumber = 1;

	//Vars for stranger encounter
	int strangerNumberOfNums = 15, strangerMaxCashAmount = 25, strangerMinCashAmount = 5;
	string userInput = "";

	srand(time(0));

	while (true)
	{
		int cheesePos = (rand() % numberOfNums) + firstNumber; // Randomize cheese position between 1 and 3

		int randomEncounter = (rand() % strangerNumberOfNums) + firstNumber; //Random number generated to determine if the stranger encounter should happen
		double cashIncrease = (rand() % strangerMaxCashAmount) + strangerMinCashAmount; //Random number between the min and max cash increases that the stranger will bet

		printCurrentAmounts(playerCash, amountLost, amountWon);

		cout << "Please enter the amount you would like to bet and your guess seperated by a space: ";

		cin.clear();

		getline(cin, userInput);

		//Start Error Checking

			//Removes all non-digit characters from input string to remove the need for certain error checking
		userInput.erase(remove_if(userInput.begin(), userInput.end(), isNotDigit), userInput.end());

		//Input checking
		checkIntAndExit(userInput, playerCash, amountLost, amountWon);

		if (userInput.length() <= 1)
		{
			printErrorToConsole("You failed to input a second number!");

			continue;
		}

		//Place inputs into integer variables
		userBetAmount = stoi(userInput.substr(0, userInput.length() - 1));
		holeGuess = stoi(string(1, userInput.back()));

		//Hole and Bet Checking
		if (userBetAmount > playerCash)
		{
			printErrorToConsole("You tried to bet more than you have!");

			continue;
		}
		else if (userBetAmount <= 0)
		{
			printErrorToConsole("You must enter a bet greater than zero!");

			continue;
		}
		else if (holeGuess < 1 || holeGuess > 3)
		{
			printErrorToConsole("Your hole guess MUST be between 1 and 3 inclusively!");

			continue;
		}
		//End Error Checking

		if (randomEncounter == strangerNumberOfNums)
		{
			userBetAmount = handleRandomEncounter(userBetAmount, cashIncrease, playerCash, amountLost, amountWon);

			cin.ignore(1000, '\n'); //Used to fix bug of game closing after completing encounter
		}

		if (holeGuess == cheesePos)
		{
			cout << "\n- You found the cheese! -\n\n";

			playerCash += (2 * userBetAmount);
			amountWon += (2 * userBetAmount);
		}
		else
		{
			cout << "\n- You guessed wrong! The cheese was in hole #" << cheesePos << " -\n\n";

			playerCash -= userBetAmount;
			amountLost += userBetAmount;
		}

		if (playerCash <= 0)
		{
			playerCash = 0;

			cout << "\n- You're out of money! -\n\n";

			printEndOfGame(playerCash, amountLost, amountWon);

			break;
		}
	}

	system("pause");

	return 0;
}

void checkIntAndExit(string input, const int& playerCash, const int& amountLost, const int& amountWon)
{
	if (input.empty())
	{
		printEndOfGame(playerCash, amountLost, amountWon);

		system("pause");

		exit(0);
	}
}

//Function that handles random encounter. Either returns the original bet plus the stranger's increase or just the original bet.
double handleRandomEncounter(double currentBet, double strangerCashIncrease, const int& playerCash, const int& amountLost, const int& amountWon)
{
	string input;
	bool hasAcceptedStrangerOffer = false;

	cout << "\n----------------------------------------------" << endl
		<< "A stranger has appeared and has offered to up the bet by $" << strangerCashIncrease << "!" << endl
		<< "If you choose to accept, you can win 2x your bet plus the strangers increase or lose your bet plus the strangers increase!\n\n"
		<< "Do you accept? (0 for no, 1 for yes): ";
	getline(cin, input);

	while (!input.empty())
	{
		bool isValid = true;

		if (input.length() > 1)
		{
			isValid = false;
		}
		else if (!isdigit(input[0]))
		{
			isValid = false;
		}
		else if (stoi(input) > 1 || stoi(input) < 0)
		{
			isValid = false;
		}

		if (isValid)
		{
			hasAcceptedStrangerOffer = stoi(input);
			break;
		}

		printErrorToConsole("You did not enter a valid number!");

		cout << "Do you accept? (0 for no, 1 for yes): ";
		getline(cin, input);
	}

	checkIntAndExit(input, playerCash, amountLost, amountWon);

	if (hasAcceptedStrangerOffer)
	{
		cout << "\nThe stranger smiles at your risk taking abilities.\n\nPress any key to continue..." << endl
			<< "----------------------------------------------" << endl;

		cin.clear();

		return (currentBet + strangerCashIncrease);
	}
	else
	{
		cout << "\nThe stranger looks at you in disappointment and leaves in the blink of an eye.\n\nPress any key to continue..." << endl
			<< "----------------------------------------------" << endl;

		cin.clear();

		return currentBet;
	}
}

//Function to check if a char is not a digit
//Used for the 'remove_if' algorithm
bool isNotDigit(char c) {
	return !(c >= '0' && c <= '9');
}

//Start functions to make printing easier
void printEndOfGame(double playerCash, double amountLost, double amountWon)
{
	int widthAmount = 5;
	char wallChar = '|';

	string outputs[3] = { "Final Player Cash: $", "Amount Lost: $", "Amount Won: $" };

	cout << fixed << setprecision(2) << "\n==============================================" << endl
		<< wallChar << endl
		<< wallChar << setw(widthAmount + outputs[0].length()) << outputs[0] << playerCash << endl
		<< wallChar << setw(widthAmount + outputs[1].length()) << outputs[1] << amountLost << endl
		<< wallChar << setw(widthAmount + outputs[2].length()) << outputs[2] << amountWon << endl
		<< wallChar
		<< "\n==============================================\n" << endl
		<< "\n- Thanks for playing my game! -" << endl;
}

void printCurrentAmounts(double playerCash, double amountLost, double amountWon)
{
	int widthAmount = 5;
	char wallChar = '|';

	string outputs[3] = { "Current Cash: $", "Current Amount Won: $", "Current Amount Lost: $" };

	cout << fixed << setprecision(2) << "==============================================\n"
		<< wallChar << endl
		<< wallChar << setw(widthAmount + outputs[0].length()) << outputs[0] << playerCash << endl
		<< wallChar << setw(widthAmount + outputs[1].length()) << outputs[1] << amountWon << endl
		<< wallChar << setw(widthAmount + outputs[2].length()) << outputs[2] << amountLost << endl
		<< wallChar
		<< "\n==============================================\n" << endl;
}

void printRules()
{
	int widthAmount = 5;
	char wallCharacter = '|';
	string rules[4]{ "- Your goal is to direct the rat to the hole containing the cheese\n", "- You have a 1 in 3 chance of guessing correctly\n",
		"- You must bet at least one dollar on each turn\n", "- You can quit at any time by hitting the return key with nothing typed in" };

	cout << "====================================== RULES =====================================\n"
		<< wallCharacter << setw(widthAmount + rules[0].length()) << rules[0]
		<< wallCharacter << setw(widthAmount + rules[1].length()) << rules[1]
		<< wallCharacter << setw(widthAmount + rules[2].length()) << rules[2]
		<< wallCharacter << setw(widthAmount + rules[3].length()) << rules[3]
		<< "\n==================================================================================\n\n";
}

void printErrorToConsole(string errorMessage)
{
	cout << "\n >> ERROR: " << errorMessage << "\n\n";
}
//End functions to make printing easier
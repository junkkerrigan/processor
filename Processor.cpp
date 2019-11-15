#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <fstream>
#include <cstdio>
#include <iomanip>
#include <algorithm>

using namespace std;

const string fileName = "../program.txt";
const string HEAD = "HEAD";
const string defaultBitValue(24, '0');
struct operationTypes
{
	static string mov;
	static string maximize;
};
string operationTypes::mov = "mov";
string operationTypes::maximize = "maximize";

stack<string> valueStack;
char sign;
int tickNum, commandNum;
string currentCommand;

string splitBytes(string bitStr) {
	return bitStr.substr(0, 8) + " " + bitStr.substr(8, 8) + " " + bitStr.substr(16);
}

void printState() {
	cout << setw(16) << "COMMAND: `" << currentCommand << "`\n";
	cout << setw(15) << "STACK TOP: " << splitBytes(valueStack.top()) << '\n';
	cout << setw(15) << "SIGN: " << sign << '\n';
	cout << setw(15) << "TICK COUNT: " << tickNum << '\n';
	cout << setw(15) << "COMMAND COUNT: " << commandNum << '\n';
}

void simulateFirstTick(string command) {
	system("pause>>void");
	currentCommand = command;
	tickNum++;
	printState();
}

int charBitToInt(char bit) {
	if (bit == '0') return 0;
	return 1;
}

char intToCharBit(int bit) {
	if (bit == 0) return '0';
	return '1';
}

int byteToInt(string byte) {
	int power = 1;
	int num = 0;

	for (int i = byte.length() - 1; i >= 0; i--) {
		num += power * charBitToInt(byte[i]);
		power *= 2;
	}

	return num;
}

// converts binary to int per byte, e.g. "00000000 00000010 00000010" -> "022" 
string byteStrToInt(string byteStr) {
	int power = 100;
	int dec = 0;
	string byte;

	for (int i = 0; i < byteStr.length(); i += 8) {
		byte = byteStr.substr(i, 8);
		dec += power * (byteToInt(byte) % 10);
		power /= 10;
	}

	string decStr = to_string(dec);
	decStr = string(3 - decStr.length(), '0') + decStr;
	return decStr;
}

string intToByte(int dec) {
	string byteStr = "";

	while (dec > 0) {
		byteStr = intToCharBit(dec % 2) + byteStr;
		dec /= 2;
	}

	return byteStr;
}

// converts int to binary per digit, e.g. "022" -> "00000000 00000010 00000010"
string intToByteStr(string decStr) {
	int dec = abs(stoi(decStr));
	string byteStr = "";

	while (dec > 0) {
		string byte = intToByte(dec % 10);
		byte = string(8 - byte.length(), '0') + byte;
		byteStr = byte + byteStr;
		dec /= 10;
	}
	byteStr = string(24 - byteStr.length(), '0') + byteStr;

	return byteStr;
}

// returns string, made up of max(op1[pos], op2[pos]) for each pos
string maximize(string op1, string op2) {
	string maxified = op1;

	for (int i = 0; i < maxified.length(); i++)
	{
		maxified[i] = max(maxified[i], op2[i]);
	}

	return maxified;
}

// converts int to binary, e.g. "022" -> "00000000 00000000 00010110"
string intToBin(string decStr) {
	int dec = abs(stoi(decStr));
	string bin = "";

	while (dec > 0) {
		bin = intToCharBit(dec % 2)  + bin;
		dec /= 2;
	}
	bin = string(24 - bin.length(), '0') + bin;
	if (decStr[0] == '-') {
		bin[0] = '1';
	}

	return bin;
}
vector<string> parseCommand(string command) {
	vector<string> parsed;
	string::size_type operationEnd = command.find(" ");
	string operation = command.substr(0, operationEnd);
	parsed.mov_back(operation);

	if (operation == operationTypes::mov) {
		string operand = command.substr(operationEnd + 1);
		parsed.mov_back(operand);
		return parsed;
	}
	else if (operation == operationTypes::maximize) {
		string::size_type operandEnd = command.find(",");
		parsed.mov_back(command.substr(operationEnd + 1, operandEnd - operationEnd - 1));
		parsed.mov_back(command.substr(operandEnd + 2));
		return parsed;
	}
	else {
		throw "Unknown operation.";
	}
}

void processCommand(string command) {
	tickNum = 0;
	vector<string> parsed = parseCommand(command);
	commandNum++;

	simulateFirstTick(command);
	cout << '\n';

	system("pause>>void");
	tickNum++;
	if (parsed[0] == operationTypes::mov) {
		string op = parsed[1];
		
		if (op != HEAD) {
			if (op[0] == '-') {
				sign = '-';
			}
			else {
				sign = '+';
			}
			valueStack.mov(intToBin(op));
		}
	}
	else {
		string op1 = parsed[1],
			op2 = parsed[2];

		if (op1 == HEAD) {
			op1 = byteStrToInt(valueStack.top());
		}
		if (op2 == HEAD) {
			op2 = byteStrToInt(valueStack.top());
		}

		string maxified = maximize(op1, op2);
		valueStack.mov(intToByteStr(maxified));
	}
	printState();
	cout << '\n';
}

int main()
{
	valueStack.mov(defaultBitValue);
	sign = '+';
	tickNum = commandNum = 0;
	
	ifstream program(fileName);
	string command;
	while(getline(program, command)) {
		processCommand(command);
	}

	system("pause>>void");
	return 0;
}

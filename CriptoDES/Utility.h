#pragma once
#include<string>
#include<iostream>
#include<fstream>

using namespace std;

void convertLetterToBits(char letter, bool* bits) {
	bits[7] = letter & 1 ? true : false;
	bits[6] = letter & 2 ? true : false;
	bits[5] = letter & 4 ? true : false;
	bits[4] = letter & 8 ? true : false;
	bits[3] = letter & 16 ? true : false;
	bits[2] = letter & 32 ? true : false;
	bits[1] = letter & 64 ? true : false;
	bits[0] = letter & 128 ? true : false;
}

void convert8CharsStringToBits(string text, bool* result) {
	char letter;
	for (int i = 0; i < 8; ++i) {
		letter = text[i];
		convertLetterToBits(letter, result + 8 * i);
	}
}

void convertBitsToLetter(bool* bits, char& letter) {
	letter = 0;
	letter += bits[0] ? 128 : 0;
	letter += bits[1] ? 64 : 0;
	letter += bits[2] ? 32 : 0;
	letter += bits[3] ? 16 : 0;
	letter += bits[4] ? 8 : 0;
	letter += bits[5] ? 4 : 0;
	letter += bits[6] ? 2 : 0;
	letter += bits[7] ? 1 : 0;
}

void convertBitsToString(bool* bits, string& result) {
	char letter;
	for (int i = 0; i < 8; ++i) {
		convertBitsToLetter(bits + 8 * i, letter);
		result += letter;
	}
}

void convertSBoxResultToBool(int boxResult, bool* result) {

	result[0] = boxResult & 8 ? true : false;
	result[1] = boxResult & 4 ? true : false;
	result[2] = boxResult & 2 ? true : false;
	result[3] = boxResult & 1 ? true : false;
}

void copyBoolVectorToAnother(bool* source, bool* destination, int length) {
	for (int i = 0; i < length; ++i) {
		destination[i] = source[i];
	}
}

int getBoxElementIndex(bool* B_i) {
	int rowsBefore = 0;
	rowsBefore += B_i[0] ? 2 : 0;
	rowsBefore += B_i[5] ? 1 : 0;

	int columns = 0;
	columns += B_i[1] ? 8 : 0;
	columns += B_i[2] ? 4 : 0;
	columns += B_i[3] ? 2 : 0;
	columns += B_i[4] ? 1 : 0;

	return rowsBefore * 16 + columns;

}

void convertBoolArrayToHexa(bool* bits, string& result) {
	char letter = 0;
	letter += bits[0] ? 8 : 0;
	letter += bits[1] ? 4 : 0;
	letter += bits[2] ? 2 : 0;
	letter += bits[3] ? 1 : 0;

	if (0 <= letter && letter <= 9) {
		letter += '0';
	}
	else {
		letter += ('A' - 10);	
	}
	result += letter;
}

void convertHexaInBoolArray(char letter, bool* bits) {
	if ('0' <= letter && letter <= '9') {
		letter -= '0';
	}
	else {
		letter -= ('A' - 10);
	}
	bits[3] = letter & 1 ? true : false;
	bits[2] = letter & 2 ? true : false;
	bits[1] = letter & 4 ? true : false;
	bits[0] = letter & 8 ? true : false;
}

void printAndWriteTextInHexa(bool* bitCryptotext, char* file) {
	string result;
	for (int i = 0; i < 16; ++i) {
		convertBoolArrayToHexa(bitCryptotext + 4 * i, result);
	}
	cout << result << '\n';

	ofstream out(file, fstream::trunc | fstream::out);
	out << result;
	out.close();
}

void printBoolVector(bool* bits, int length) {
	for (int i = 0; i < length; ++i) {
		cout << bits[i];
		if (i % 4 == 3) {
			cout << ' ';
		}
	}
	cout << '\n';
}

string convertIndexInHexaString(int index) {
	string result;
	int numberOfOnes = 0;

	int firstPart = 0, secondPart = 0;

	if (index & 64) {
		firstPart += 8;
		++numberOfOnes;
	}
	if (index & 32) {
		firstPart += 4;
		++numberOfOnes;
	}
	if (index & 16) {
		firstPart += 2;
		++numberOfOnes;
	}
	if (index & 8) {
		firstPart += 1;
		++numberOfOnes;
	}

	if (index & 4) {
		secondPart += 8;
		++numberOfOnes;
	}
	if (index & 2) {
		secondPart += 4;
		++numberOfOnes;
	}
	if (index & 1) {
		secondPart += 2;
		++numberOfOnes;
	}
	
	if (numberOfOnes % 2 == 0) {
		secondPart += 1;
	}

	char letter;

	if (0 <= firstPart && firstPart <= 9) {
		letter = firstPart + '0';
	} 
	else {
		letter = firstPart - 10 + 'A';
	}

	result += letter;

	if (0 <= secondPart && secondPart <= 9) {
		letter = secondPart + '0';
	}
	else {
		letter = secondPart - 10 + 'A';
	}

	result += letter;
	return result;

}

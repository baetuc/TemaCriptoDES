#pragma once
#include"Utility.h"
#include"KeyGenerator.h"
#include"Permutations.h"
#include<fstream>
#include<string>

#define MESSAGE "message.txt"
#define ENCRYPTED_TEXT "encryptedText.txt"
#define DECRYPTED_TEXT "decryptedText.txt"
using namespace std;

bool message[64];

void initializeMessage() {
	ifstream f(MESSAGE);
	string initialMessage;
	f >> initialMessage;
	f.close();
	convert8CharsStringToBits(initialMessage, message);
}

void initializeHexaMessage(char* text) {
	ifstream f(text);
	string initialMessage;
	f >> initialMessage;
	f.close();
	for (int i = 0; i < 16; ++i) {
		convertHexaInBoolArray(initialMessage[i], message + 4 * i);
	}
}

int* getBoxFromIndex(int index) {
	switch (index) {
	case 1: return S1;
	case 2: return S2;
	case 3: return S3;
	case 4: return S4;
	case 5: return S5;
	case 6: return S6;
	case 7: return S7;
	case 8: return S8;
	default: return 0;
	}
}

void XOR(bool* left, bool* right, bool* result, int length) {
	for (int i = 0; i < length; ++i) {
		result[i] = (left[i] == right[i]) ? false : true;
	}
}

void expandVector(bool* RVector, bool* result) {
	for (int i = 0; i < 48; ++i) {
		result[i] = RVector[E[i] - 1];
	}
}

void f(bool* RVector, int round, bool* finalResult) {
	bool expandedVector[48];
	expandVector(RVector, expandedVector); // we apply the E permutation 
	bool xoredVector[48];
	XOR(expandedVector, permutationKeys[round - 1], xoredVector, 48); // we xor the result with K_n

	bool SBoxesResult[32];

	for (int i = 0; i < 8; ++i) {
		int* SBox = getBoxFromIndex(i + 1);
		bool SBoxRes[4];
		convertSBoxResultToBool(SBox[getBoxElementIndex(xoredVector + 6 * i)], SBoxRes);
		copyBoolVectorToAnother(SBoxRes, SBoxesResult + 4 * i, 4);

	}
	// We now have S_1S_2...S_8. All we have to do is applying the last permutation.

	for (int i = 0; i < 32; ++i) {
		finalResult[i] = SBoxesResult[P[i] - 1];
	}

}

void encryptMessage(char* inputFile, char* outputFile, char* keyFile) {
	//initializeMessage();
	generatePermutationKeys(keyFile);
	initializeHexaMessage(inputFile);

	bool firstPermuttedMessage[64];
	bool LVector[32];
	bool RVector[32];

	// We apply the IP permutation to the message M

	for (int i = 0; i < 64; ++i) {
		firstPermuttedMessage[i] = message[IP[i] - 1];
	}

	// We now break the permutted message in two halves
	for (int i = 0; i < 32; ++i) {
		LVector[i] = firstPermuttedMessage[i];
		RVector[i] = firstPermuttedMessage[i + 32];
	}

	bool currentLVector[32];
	bool currentRVector[32];

	for (int i = 1; i < 17; ++i) {
		// We have 16 rounds on which we process the RVector and LVector, based on the formulae: 
		// L_n = R_(n-1) and R_n = L_(n-1) + f(R_(n-1), K_(n-1)), where + defines the XOR operation

		for (int j = 0; j < 32; ++j) {
			currentLVector[j] = RVector[j];
		}
		/*
		cout << 'L' << i << ": ";
		printBoolVector(currentLVector, 32);
		*/
		bool fResult[32];

		f(RVector, i, fResult);

		XOR(LVector, fResult, currentRVector, 32);

		/*
		cout << "R" << i << ": ";
		printBoolVector(currentRVector, 32);
		*/

		copyBoolVectorToAnother(currentLVector, LVector, 32);
		copyBoolVectorToAnother(currentRVector, RVector, 32);
	}
	// We now apply IP_1 to R_16 L_16

	bool bitCryptotext[64];

	for (int i = 0; i < 64; ++i) {
		int elementIndex = IP_1[i] - 1;

		if (elementIndex < 32) {
			bitCryptotext[i] = currentRVector[elementIndex];
		}
		else {
			bitCryptotext[i] = currentLVector[elementIndex - 32];
		}
	}

	printAndWriteTextInHexa(bitCryptotext, outputFile);
}

void decryptMessage(char* inputFile, char* outputFile, char* keyFile) {
	//initializeMessage();
	generatePermutationKeys(keyFile);
	initializeHexaMessage(inputFile);

	bool firstPermuttedMessage[64];
	bool LVector[32];
	bool RVector[32];

	// We apply the IP permutation to the message M

	for (int i = 0; i < 64; ++i) {
		firstPermuttedMessage[i] = message[IP[i] - 1];
	}

	// We now break the permutted message in two halves
	for (int i = 0; i < 32; ++i) {
		LVector[i] = firstPermuttedMessage[i];
		RVector[i] = firstPermuttedMessage[i + 32];
	}

	bool currentLVector[32];
	bool currentRVector[32];

	for (int i = 1; i < 17; ++i) {
		// We have 16 rounds on which we process the RVector and LVector, based on the formulae: 
		// L_n = R_(n-1) and R_n = L_(n-1) + f(R_(n-1), K_(17-n)), where + defines the XOR operation

		for (int j = 0; j < 32; ++j) {
			currentLVector[j] = RVector[j];
		}
		/*
		cout << 'L' << i << ": ";
		printBoolVector(currentLVector, 32);
		*/
		bool fResult[32];

		f(RVector, 17 - i, fResult);

		XOR(LVector, fResult, currentRVector, 32);

		/*
		cout << "R" << i << ": ";
		printBoolVector(currentRVector, 32);
		*/

		copyBoolVectorToAnother(currentLVector, LVector, 32);
		copyBoolVectorToAnother(currentRVector, RVector, 32);
	}
	// We now apply IP_1 to R_16 L_16

	bool bitCryptotext[64];

	for (int i = 0; i < 64; ++i) {
		int elementIndex = IP_1[i] - 1;

		if (elementIndex < 32) {
			bitCryptotext[i] = currentRVector[elementIndex];
		}
		else {
			bitCryptotext[i] = currentLVector[elementIndex - 32];
		}
	}

	printAndWriteTextInHexa(bitCryptotext, outputFile);



}

void encryptSimpleDES() {
	encryptMessage(MESSAGE, ENCRYPTED_TEXT, KEY);
}

void decryptSimpleDES() {
	decryptMessage(ENCRYPTED_TEXT, DECRYPTED_TEXT, KEY);
}
#pragma once
#include"Permutations.h"
#include"Utility.h"
#include<fstream>
#include<iostream>
#include<string>

#define KEY "key.txt"

using namespace std;

bool key[64];
bool firstPermutationKey[56];
bool permutationKeys[16][48];

void initializeKey() {
	ifstream in(KEY);
	char letter;
	for (int i = 0; i < 64; ++i) {
		in >> letter;
		key[i] = (letter == '0') ? false : true;
	}
}

void initializeHexaKey(char* keyFile) {
	ifstream in(keyFile);
	char letter;
	for (int i = 0; i < 16; ++i) {
		in >> letter;
		if ('0' <= letter && letter <= '9') {
			letter -= '0';
		}
		else {
			letter -= ('A' - 10);
		}
		convertSBoxResultToBool(letter, key + 4 * i);
	}
}

void initializeFirstPermutationKey(char* keyFile) {
	//initializeKey();
	initializeHexaKey(keyFile);
	for (int i = 0; i < 56; ++i) {
		firstPermutationKey[i] = key[PC1[i] - 1];
	}
}

void shiftLeftWithOnePosition(bool* actualArray) {
	bool firstElement = actualArray[0];
	for (int i = 0; i < 27; ++i) {
		actualArray[i] = actualArray[i + 1];
	}
	actualArray[27] = firstElement;
}

void shiftLeftWithTwoPositions(bool* actualArray) {
	bool firstElement = actualArray[0];
	bool secondElement = actualArray[1];
	for (int i = 0; i < 27; ++i) {
		actualArray[i] = actualArray[i + 2];
	}
	actualArray[26] = firstElement;
	actualArray[27] = secondElement;
}

void printKeys() {
	for (int i = 0; i < 16; ++i) {
		cout << "K" << i + 1 << ": ";
		for (int j = 0; j < 48; ++j) {
			cout << permutationKeys[i][j];
			if (j % 6 == 5) {
				cout << ' ';
			}
		}
		cout << '\n';
	}
}

void generatePermutationKeys(char* keyFile) {
	initializeFirstPermutationKey(keyFile);
	bool CVector[28];
	bool DVector[28];

	// We construct the C0 and D0 arrays
	for (int i = 0; i < 28; ++i) {
		CVector[i] = firstPermutationKey[i];
	}

	for (int i = 0; i < 28; ++i) {
		DVector[i] = firstPermutationKey[i + 28];
	}

	// We now start constructing C_n and D_n, based on C_(n-1) and D_(n-1)

	for (int i = 1; i < 17; ++i) {

		// In the rounds 1, 2, 9 and 16, C_n and D_n are constructed from C_(n-1) and D_(n-1) ony by applying one left shift
		if (i == 1 || i == 2 || i == 9 || i == 16) {
			shiftLeftWithOnePosition(CVector);
			shiftLeftWithOnePosition(DVector);
		}
		else {
			shiftLeftWithTwoPositions(CVector);
			shiftLeftWithTwoPositions(DVector);
		}

		// We now have C_n and D_n and start constructing K_n, by aplying the PC-2 permutation

		for (int j = 0; j < 48; ++j) {
			int elementIndex = PC2[j];
			// select the wanted element from the concatenated array C_nD_n
			if (elementIndex < 29) {
				permutationKeys[i - 1][j] = CVector[elementIndex - 1];
			}
			else {
				permutationKeys[i - 1][j] = DVector[elementIndex - 29];
			}
		}

	}
	//printKeys();
}

#pragma once
#include"EncryptionDecryption.h"
#include<stdio.h>
#include<string>
#include<map>

#define DOUBLE_DES_FIRST_KEY "key.txt"
#define DOUBLE_DES_SECOND_KEY "secondKey.txt"
#define FOUND_DOUBLE_DES_KEY "found2DESKey.txt"
#define DOUBLE_DES_MESSAGE_CRYPTOTEXT_PAIR "2DESMessageCryptotext.txt"
#define DOUBLE_DES_MESSAGE_AFTER_FIRST_ENCRYPTION "auxiliarCryptotext.txt"
#define DOUBLE_DES_TEMPORARY_FILE_AFTER_SECOND_ENCRYPTION "temporaryOutput.txt"
#define DOUBLE_DES_TEMPORARY_MESSAGE "temporaryMessage.txt"
#define DOUBLE_DES_TEMPORARY_CRYPTOTEXT "temporaryCryptotext.txt"
#define DOUBLE_DES_TEMPORARY_FILE "temporaryEncryptedOrDecryptedFile.txt"

#define DOUBLE_DES_CURRENT_KEY "temporaryKey.txt"

using namespace std;


void encryptDoubleDES() {
	encryptMessage(MESSAGE, DOUBLE_DES_MESSAGE_AFTER_FIRST_ENCRYPTION, DOUBLE_DES_FIRST_KEY);
	encryptMessage(DOUBLE_DES_MESSAGE_AFTER_FIRST_ENCRYPTION, DOUBLE_DES_TEMPORARY_FILE_AFTER_SECOND_ENCRYPTION,
		DOUBLE_DES_SECOND_KEY);

	remove(DOUBLE_DES_MESSAGE_AFTER_FIRST_ENCRYPTION);

	ifstream in(MESSAGE);
	ofstream out(DOUBLE_DES_MESSAGE_CRYPTOTEXT_PAIR, fstream::trunc | fstream::out);

	char hexaLetter;
	for (int i = 0; i < 16; ++i) {
		in >> hexaLetter;
		out << hexaLetter;
	}
	out << ' ';

	in.close();

	ifstream res(DOUBLE_DES_TEMPORARY_FILE_AFTER_SECOND_ENCRYPTION);
	for (int i = 0; i < 16; ++i) {
		res >> hexaLetter;
		out << hexaLetter;
	}
	res.close();

	remove(DOUBLE_DES_TEMPORARY_FILE_AFTER_SECOND_ENCRYPTION);
	
}

void breakDoubleDES() {
	map<string, string> lookupTable;

	// First we iterate through all the keys (note that we don't know only the last byte of the two keys) and decrypt 
	// the known text with every one of it.

	// The keys will be of form 01 01 01 01 01 01 01 xx (in Hexa), so we will have to iterate through 2^7=128 possibilities
	// (not so bad).

	string unchangedPart = "01010101010101";
	string currentKey;

	ifstream in(DOUBLE_DES_MESSAGE_CRYPTOTEXT_PAIR);
	string read;
	in >> read;

	ofstream writeMessage(DOUBLE_DES_TEMPORARY_MESSAGE);
	writeMessage << read;
	writeMessage.close();

	in >> read;
	ofstream writeText(DOUBLE_DES_TEMPORARY_CRYPTOTEXT);
	writeText << read;
	writeText.close();
	in.close();

	for (int i = 0; i < 128; ++i) {
		currentKey = unchangedPart;
		currentKey += convertIndexInHexaString(i);
		ofstream out(DOUBLE_DES_CURRENT_KEY, fstream::trunc | fstream::out);
		out << currentKey;
		out.close();
		 
		// Encrypt the given message with the current key
		encryptMessage(DOUBLE_DES_TEMPORARY_MESSAGE, DOUBLE_DES_TEMPORARY_FILE, DOUBLE_DES_CURRENT_KEY);
		string encryptedMessage;
		ifstream readMessage(DOUBLE_DES_TEMPORARY_FILE);
		readMessage >> encryptedMessage;
		readMessage.close();

		lookupTable[encryptedMessage] = currentKey;
	}

	// We have a hashmap with all the possible cryptotext than can result out of the message.
	// Now, all we have to do is to decrypt the given cryptotext with consecutive keys, until we have a collision in the map

	bool foundKeys = false; 
	string firstKey, secondKey;

	for (int i = 0; i < 128 && !foundKeys; ++i) {
		currentKey = unchangedPart;
		currentKey += convertIndexInHexaString(i);
		ofstream out(DOUBLE_DES_CURRENT_KEY, fstream::trunc | fstream::out);
		out << currentKey;
		out.close();

		// Decrypt the given message with the current key
		decryptMessage(DOUBLE_DES_TEMPORARY_CRYPTOTEXT, DOUBLE_DES_TEMPORARY_FILE, DOUBLE_DES_CURRENT_KEY);
		string decryptedCryptotext;
		ifstream readCryptotext(DOUBLE_DES_TEMPORARY_FILE);
		readCryptotext >> decryptedCryptotext;
		readCryptotext.close();

		if (lookupTable.find(decryptedCryptotext) != lookupTable.end()) {
			foundKeys = true;
			firstKey = lookupTable[decryptedCryptotext];
			secondKey = currentKey;
		}
	}

	ofstream writeKeys(FOUND_DOUBLE_DES_KEY, fstream::trunc | fstream::out );
	writeKeys << firstKey << " " << secondKey;
	writeKeys.close();

	remove(DOUBLE_DES_CURRENT_KEY);
	remove(DOUBLE_DES_TEMPORARY_CRYPTOTEXT);
	remove(DOUBLE_DES_TEMPORARY_MESSAGE);
	remove(DOUBLE_DES_TEMPORARY_FILE);

}
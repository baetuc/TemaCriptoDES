#include<iostream>
#include"KeyGenerator.h"
#include"Utility.h"
#include"EncryptionDecryption.h"
#include"2DES.h"

using namespace std;

int main() {
	int DESOption;
	cout << "Please enter 1 for simple DES or 2 for 2-DES: ";
	cin >> DESOption;
	if (DESOption == 1) {
		cout << "Please enter 1 for encrypting or 2 for decrypting: ";
		cin >> DESOption;
		if (DESOption == 1) {
			encryptSimpleDES();
		} 
		else if (DESOption == 2) {
			decryptSimpleDES();
		}
		else {
			cout << "You entered an invalid command. Goodye!\n";
		}
	} 
	else if (DESOption == 2) {
		// 2-DES branch
		cout << "Please enter 1 for encrypting with 2-DES or 2 for breaking 2-DES: ";
		cin >> DESOption;
		if (DESOption == 1) {
			encryptDoubleDES();
		}
		else if (DESOption == 2) {
			breakDoubleDES();
		}
		else {
			cout << "You entered an invalid command. Goodye!\n";
		}

	} 
	else {
		cout << "You entered an invalid command. Goodbye!\n";
	}
	return 0;
}
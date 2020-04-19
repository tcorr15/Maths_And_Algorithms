#include <iostream>
#include <fstream>
#include <string>

using namespace std;

static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" //the base64 characters available for later use
										"abcdefghijklmnopqrstuvwxyz"
										"0123456789+/";

int base64_count = 6, curr_base64_value = 0, sizeOfString;//global variables to allow bit shifting from 8 bits to 4 bits
string leftover;

int printBase64(int current, int previous, int x) {//prints out the Base64 character based on the  values passed in. Including the unshifted leftover values from the previous ASCII value to the new ASCII value and the number of bits each need to shift
	int temp = current >> x;
	previous = previous << (8 - x);
	cout << base64_chars[temp + previous];
	current = current - (temp << x);
	if (x == 6) {
		cout << base64_chars[current];
		return 0;
	}
	else return current;
}

void base64_Creator(string line) { //Based on the lines passed through from the chosen text file, will split the letters into 3 groups (24 bits) to provide an even split of bits from 8 into 6. It will conver the values into 6 bits from 8 bits and carry foward the remaining bits into the next bit process. If there isn't enough ASCII letters to group into three bits then = signs will be printed where appropriate
	line = leftover + line;
	sizeOfString = line.size();
	for (int i = 0; i < (sizeOfString - (sizeOfString%3)); i += 3) {//groups the values where three letters are available and process the ASCII letters into Base64
		int asciiCharacter;
		asciiCharacter = (int)line[i];
		int previous = printBase64(asciiCharacter, 0, 2);
		asciiCharacter = (int)line[i + 1];
		previous = printBase64(asciiCharacter, previous, 4);
		asciiCharacter = (int)line[i + 2];
		previous = printBase64(asciiCharacter, previous, 6);
	}
	leftover = "";
	for (int i = sizeOfString % 3; i > 0; i--) { //adds the remaining letters to the leftover string to be carried onto the next loop
		leftover += line[sizeOfString - i];
	}
}

void methodCall() {//Requests for a file location. Reads the valid file and runs the base64 converting method
	string ascii_Input = "", line, file_Location;
	while (true) {//Will run until a valid file path has been provided. When provided then will read the path and process the ASCII values into base64
		cout << "Enter File Location of : " << endl;
		cin >> file_Location;
		ifstream myfile(file_Location, ios::in || ios::binary);
		if (myfile.is_open()) //runs the file only if the file is open
		{
			base64_count = 6, leftover = "";
			while (getline(myfile, line))//loop running the base 64 encoder for each line in the text file
				base64_Creator(line);
			myfile.close();
			if ((sizeOfString % 3) == 2) { //Will process the last 2 remaining ASCII letters (where there isn't enough letters to group into three) and then prints and = sign
				int asciiCharacter = (int)line[sizeOfString - 2];
				int previous = printBase64(asciiCharacter, 0, 2);
				asciiCharacter = (int)line[sizeOfString - 1];
				previous = printBase64(asciiCharacter, previous, 4);
				cout << "=" << endl;
			}
			if ((sizeOfString % 3) == 1) {//Will process the last remaining ASCII letter (where there isn't enough letters to group into three) and then prints two = signs (==)
				int asciiCharacter = (int)line[sizeOfString - 1];
				int previous = printBase64(asciiCharacter, 0, 2);
				cout << "==" << endl;
			}
			break;
		}
		else cout << "Unable to open file" << endl;
	}
}

int main() {//The main loop, will begin by requesting a file location with text to be encoded
	cout << "This program is designed to convert a set of text from a text file into base 64 for email\n" << endl;
	cout << "------------------------------------------\n\n";
	bool isLoop = true;
	while (isLoop) {//runs a loop until the user wants to leave the program
		methodCall();
		cout << "\n\n------------------------------------------\n\n";
		string confirmation;
		while (true){//request loop for a different file 
			cout << "Would you like to try a different file? (y/n)" << endl;
			cin >> confirmation;
			if (confirmation == "n") {
				isLoop = false;
				break;
			}
			else if (confirmation == "y") break;
			else cout << "Unknown response" << endl;
		}
	}
}
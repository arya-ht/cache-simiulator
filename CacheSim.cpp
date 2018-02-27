#include <iostream>
#include <string>
#include <cmath>
#include <fstream>

/*
* Cache simulator
* GROUP MEMBERS:
* Kaif Dauwa
* Arya HajiTaheri
* Vinh  Lam
*/

using namespace std;
string convert(char);
int convertBinaryToDecimal(long);
int convertStringToInt(string);
void storetag(int numindex, int associativity, int replacementpolicy, string wordtag, string **cache, string **cachePos);
void updateCache(int index, int block, int associativity, int replacementpoliciy, string **cachePos);
void loadtag(int numindex, int associativity, int replacementpolicy, string wordtag, string **cache, string **cachePos);

float miss = 0, hit = 0;
int main() {
	ifstream tracefile;

	const int cachesize = 32768; //32KB cache size
	int blocksize,
		associativity,
		replacement,
		writepolicy,
		sets, blocks,
		addresssize,
		tag,
		indexsize,
		offsetsize,
		numtag,
		numindex,
		numoffset;

	string **cache, **cachePos;

	string word,
		filename,
		binary = "",
		operation,
		wordtag,
		wordindex,
		wordoffset;
	cout << "/*\n * COSC 2440 - GROUP 7\n * FINAL PROJECT : 3 - Cache simulator\n * GROUP MEMBERS :\n * Kaif Dauwa\n * Arya HajiTaheri\n * Vinh  Lam\n */\n\n";

	bool wronginput = true; //taking the parameters from users
	while (wronginput) {
		cout << "Enter your block/line size (32 or 64): ";
		cin >> blocksize;
		if (blocksize == 32) {
			wronginput = false;
		}
		if (blocksize == 64) {
			wronginput = false;
		}
	}
	wronginput = true;
	while (wronginput) {
		cout << "Enter 1 for 1-way associativity. Enter 2 for 2-way associativity: ";
		cin >> associativity;
		if (associativity == 1) {
			wronginput = false;
		}
		if (associativity == 2) {
			wronginput = false;
		}
	}
	wronginput = true;
	while (wronginput) {
		cout << "Enter 1 for FIFO replacement, or 2 for LRU replacement: ";
		cin >> replacement;
		if (replacement == 1) {
			wronginput = false;
		}
		if (replacement == 2) {
			wronginput = false;
		}
	}
	wronginput = true;

	while (wronginput) {
		cout << "Enter 1 for Write-Back policy, or 2 for Write-Through policy: ";
		cin >> writepolicy;
		if (writepolicy == 1) {
			wronginput = false;
		}
		if (writepolicy == 2) {
			wronginput = false;
		}
	}
	wronginput = true;

	while (wronginput) {
		cout << "\nEnter the trace file name: ";
		cin >> filename;
		tracefile.open(filename);
		if (tracefile.is_open() == true) {
			wronginput = false;
		}

		cout << "\nInvalid file";
	}

	cout << "\n                  CACHE TABLE                \n";
	sets = cachesize / (blocksize * associativity); 	// equations used to fined the number of bits needed for each cache section
	blocks = cachesize / blocksize;
	addresssize = 32;
	indexsize = log2(sets);
	offsetsize = log2(blocksize);
	tag = addresssize - indexsize - offsetsize;

	cache = new string*[sets];	//creating new 2d array to store new cache
	for (int i = 0; i < sets; i++) {
		cache[i] = new string[associativity];
	}
	for (int i = 0; i < sets; i++) {//creating new 2d array to store new cache address position in replacement policy
		for (int j = 0; j < associativity; j++) {
			cache[i][j] = "0";
			//cout << cache[i][j];
		}
	}
	cachePos = new string *[sets];// create a 2d array to store the position of each cache
	for (int i = 0; i < sets; i++) {
		cachePos[i] = new string[associativity];
	}
	for (int i = 0; i < sets; i++) {
		for (int j = 0; j < associativity; j++) {// initial cache element order
			if (j == 0) {
				cachePos[i][j] = "1";
			}
			else {
				cachePos[i][j] = "2";
			}
			//cout << cache[i][j];
		}
	}
	while (tracefile >> word) { //read data from main memory to cache
		if (word == "s") {
			operation = word;
		}
		else if (word == "l") {
			operation = word;
		}
		else if (word.substr(0, 2) == "0x") {
			for (int i = 2; i < word.length(); i++) {
				binary = binary + convert(word[i]);
			}

			//cout << binary << endl;
			//separate binary bits into tag +index+offset
			wordtag = binary.substr(0, tag);
			wordindex = binary.substr(tag, indexsize);
			wordoffset = binary.substr(indexsize + tag, offsetsize);

			//cout << "TAG: " << wordtag << " INDEX: " << wordindex << " OFFSET: " << wordoffset << endl;
			numindex = convertStringToInt(wordindex);
			numindex = convertBinaryToDecimal(numindex);
			//cout << numindex << endl;

			if (operation == "s") {
				storetag(numindex, associativity, replacement, wordtag, cache, cachePos);
			}
			if (operation == "l") {
				loadtag(numindex, associativity, replacement, wordtag, cache, cachePos);
			}
		}
		binary = "";
	}
	// print final cache table
	for (int i = 0; i < sets; i++) {
		cout << "INDEX " << i << ": ";
		for (int j = 0; j < associativity; j++) {
			if (j == 1) {
				cout << "| ";
			}
			if (cache[i][j] == "0") {
				cout << "       EMPTY      " << " ";
			}
			else {
				cout << cache[i][j] << " ";
			}
		}
		cout << endl;
		switch (associativity)
		{
		case(2):
			cout << "==================================================\n";
			break;
		case(1):
			cout << "============================\n";
			break;
		}
	}
	// print final hit miss ratio
	cout << "TOTAL HIT:  " << hit << endl;
	cout << "TOTAL MISS: " << miss << endl;
	cout << "ACCURACY:   " << (hit / (hit + miss)) * 100 << "%" << endl;

	system("pause");
	return 0;
}

string convert(char c) {

	//convert from base 16 to base 2
	switch (c) {
	case '0':
		return "0000"; break;
	case '1':
		return "0001"; break;
	case '2':
		return "0010"; break;
	case '3':
		return "0011"; break;
	case '4':
		return "0100"; break;
	case '5':
		return "0101"; break;
	case '6':
		return "0110"; break;
	case '7':
		return "0111"; break;
	case '8':
		return "1000"; break;
	case '9':
		return "1001"; break;
	case 'a':
		return "1010"; break;
	case 'b':
		return "1011"; break;
	case 'c':
		return "1100"; break;
	case 'd':
		return "1101"; break;
	case 'e':
		return "1110"; break;
	case 'f':
		return "1111"; break;
	}
}
int convertBinaryToDecimal(long n)
{
	int decimalNumber = 0, i = 0, remainder;
	while (n != 0)
	{
		remainder = n % 10;
		n /= 10;
		decimalNumber += remainder*pow(2, i);
		++i;
	}
	return decimalNumber;
}
int convertStringToInt(string word) {
	int num = atoi(word.c_str());
	return num;
}
void storetag(int numindex, int associativity, int replacementpolicy, string wordtag, string **cache, string **cachePos) {/// this function stores the data to cache lines 

	if (associativity == 1) {// direct map
		if (cache[numindex][0] != wordtag) {
			cache[numindex][0] = wordtag;
			miss++;
		}
		else {
			hit++;
		}
	}
	else { // set associative 2
		if (cache[numindex][0] == "0") { // if first element is empty, replace with cache line
			cache[numindex][0] = wordtag;
			miss++;
			updateCache(numindex, 0, 2, replacementpolicy, cachePos);

		}
		else if (cache[numindex][0] != "0" && cache[numindex][1] == "0") { // if the second element is empty, replace with cache line

			cache[numindex][1] = wordtag;
			miss++;
			updateCache(numindex, 1, 2, replacementpolicy, cachePos);
		}
		else { // if both are full, replace according to replacement policy
			if (cachePos[numindex][0] == "1") {
				if (cache[numindex][0] != wordtag) {
					cache[numindex][0] = wordtag;
					miss++;
				}
				else {
					hit++;
				}
				updateCache(numindex, 0, 2, replacementpolicy, cachePos);
			}
			else {
				if (cache[numindex][1] != wordtag) {
					cache[numindex][1] = wordtag;
					miss++;
				}
				else {
					hit++;
				}
				updateCache(numindex, 1, 2, replacementpolicy, cachePos);
			}
		}
	}
}
void updateCache(int index, int block, int associativity, int replacementpoliciy, string **cachePos) {/// this function updates the cache block with replacement values
	if (block == 0) {//assigning values for FIFO and LRU
		cachePos[index][block] = "1";
		cachePos[index][1] = "2";

	}
	else if (block == 1) {
		cachePos[index][block] = "1";
		cachePos[index][0] = "2";
	}
}
void loadtag(int numindex, int associativity, int replacementpolicy, string wordtag, string **cache, string **cachePos) {/// similar store, but does not replace elements. only updates hit/miss and replacement values
	if (associativity == 1) { // direct map
		if (cache[numindex][0] == wordtag) {
			hit++;
		}
		else {
			miss++;
		}
	}
	else {
		if (cache[numindex][0] == wordtag) {
			if (replacementpolicy == 2) {//LRU
				updateCache(numindex, 0, 2, replacementpolicy, cachePos);
			}
			hit++;
		}
		else if (cache[numindex][1] == wordtag) {
			if (replacementpolicy == 2) {//LRU
				updateCache(numindex, 0, 2, replacementpolicy, cachePos);
			}
			hit++;
		}
		else {
			miss++;
		}
	}
}
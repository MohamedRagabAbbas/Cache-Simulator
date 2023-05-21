#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;


int numberLines, cacheSize, blockSize, numberCycles, numMiss = 0, numHit = 0, numAccess = 0;
int numberLinesBits, blockSizeBits;
string fileName;
vector<int>accessSequence;
// blockSize= cacheSize/numberLines;

void print2DVector(vector<vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (const auto& element : row) {
            cout << setw(5) << element;
        }
        cout << endl;
    }
    cout<< endl << endl<< endl;
}

void print(vector<string>& cache) {
    for (int i = 0; i < cache.size(); i++)
        cout << cache[i] << endl;
}

void printCache(string binary, int lineBits, int blockBits, bool hit) {
    
    cout << setw(5) << binary.substr(0, 32 - lineBits - blockBits) << setw(10)
        << binary.substr(32 - lineBits - blockBits, lineBits) << setw(10)
        << binary.substr(32 - blockBits);
    string b = hit ? "hit" : "miss";
    cout << setw(10) << b << endl<< endl;
}
string intToBinaryString(int number) {
    if (number == 0) {
        return string(32, '0');
    }
    string binary;
    while (number > 0) {
        binary = to_string(number % 2) + binary;
        number /= 2;
    }
    // Extend the binary string to 32 bits
    binary = string(32 - binary.length(), '0') + binary;
    return binary;
}

int binaryToDecimal(const string& binaryString) {
    int decimal = 0;
    int power = 1;
    int length = binaryString.length();
    for (int i = length - 1; i >= 0; --i) {
        if (binaryString[i] == '1') {
            decimal += power;
        }
        power *= 2;
    }
    return decimal;
}


void readIntegersFromFile(string& filename) 
{
    ifstream file(filename);
    string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            accessSequence.push_back(stoi(line));
        }
        file.close();
    }
    else {
        cout << "Unable to open the file: " << filename << std::endl;
    }
}
void cacheManger()
{
    // taking the user's inputs 
    cout << "Welcome to the cache simulator.....";
    cout << "Please enter the cache size : ";
    cin >> cacheSize;
    cout << "Please enter the number of lines in the cache : ";
    cin >> numberLines;
    blockSize = cacheSize / numberLines;
    cout << "Please enter the number of cycles (it must be between 1 and 10 ): ";
    cin >> numberCycles;
    cout << "Please enter the file name that contains access aequence: ";
    cin >> fileName;
    
    
    // reading the file that contains  that the used just entered
    readIntegersFromFile(fileName);
    // creating the cache table
    vector<string> cache(numberLines,"");
    string binary;
    numberLinesBits = log2(numberLines);
    blockSizeBits = log2(blockSize);
    int i, j;
    cout << setw(15) << "Tag" << setw(25) << "Line Number" << setw(10) << "Offset"<< setw(10) <<"Hit/Miss"<< endl;
    for(int i=0;i< accessSequence.size();i++)
    {
       // cout << accessSequence[i] << endl;
        binary = intToBinaryString(accessSequence[i]);
        int lineNumber = binaryToDecimal(binary.substr(32 - numberLinesBits - blockSizeBits, (32 - blockSizeBits)));
        if (cache[lineNumber % numberLines] == "")
        {
            numAccess++;
            numMiss++;
        }
        else
        {
            numAccess++;
            numHit++;
        }
        printCache(binary, numberLinesBits, blockSizeBits, cache[lineNumber % numberLines] != "");
        cache[lineNumber % numberLines] = binary;
    }

    cout << " the number of hits is : " << numHit << endl;
    cout << " the number of Miss is : " << numMiss << endl;
    cout << " the number of Access is : " << numAccess << endl;
    cout << " the hit ratio is : " << ((numHit * 1.0) / (numAccess * 1.0))*100 <<"%" << endl;
    cout << " the miss ratio is : " << ((numMiss * 1.0)/(numAccess *1.0)) * 100 << "%" << endl;
}

int main()
{
    cacheManger();

    // test  b ---> d and d-->b
    //cout << binaryToDecimal(intToBinaryString(87));
}

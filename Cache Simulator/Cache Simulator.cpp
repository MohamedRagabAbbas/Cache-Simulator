#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;


int numberLines, cacheSize, blockSize, numberCycles, numMiss = 0, numHit = 0, numAccess = 0;
int numberLinesBits, blockSizeBits;
string fileName;
vector<int>accessSequenceInstruction;
vector<int>accessSequenceData;
// blockSize= cacheSize/numberLines;
bool isValid(int num) 
{
    double loga = log2(num);
    int floored = floor(loga);
    return (loga - floored == 0);
}
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
        cout << setw(6) <<i<<" -    "<< cache[i] << endl;
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
            char label = line.back();
            line.pop_back();
            if (label == 'd') {
                accessSequenceData.push_back(stoi(line));
            }
            else if (label == 'i') {
                accessSequenceInstruction.push_back(stoi(line));
            }
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
    while (!isValid(cacheSize))
    {
        cout << "The number must be power of 2....." << endl;
        cin >> cacheSize;
    }
    cout << "Please enter the number of lines in the cache : ";
    cin >> numberLines;
    while (!isValid(numberLines))
    {
        cout << "The number must be power of 2....." << endl;
        cin >> numberLines;
    }
    blockSize = cacheSize / numberLines;
    cout << "Please enter the number of cycles (it must be between 1 and 10 ): ";
    cin >> numberCycles;
    while (numberCycles < 0 && numberCycles > 11)
    {
        cout << "the number must be between 1 and 10 inclusive." << endl;
        cin >> numberCycles;
    }
    cout << "Please enter the file name that contains access aequence: ";
    cin >> fileName;
    int x;
    cout << "Which cache mapping type do you want?\n";
    cout << "Enter d for direct mapping, f for full associativity, or s for m-way set associativity: ";
    char op;
    cin >> op;
    if (op == 's')
    {
        cout << "Enter m : ";
        cin >> x;
        while (!isValid(x))
        {
            cout << "The number must be power of 2....." << endl;
            cin >> x;
        }
    }
    else
    {
        if (op == 'f')
            x = numberLines;
        else x = 1;
    }
    // reading the file that contains  that the used just entered
    readIntegersFromFile(fileName);
    // creating the cache table
    vector<string> cacheData(numberLines,"");
    vector<string> cacheInstruction(numberLines, "");
    string binary;
    numberLinesBits = log2(numberLines);
    blockSizeBits = log2(blockSize);
    int i, j;
    cout << "Instruction Cache:\n";
    cout << setw(15) << "Tag" << setw(25) << "Line Number" << setw(10) << "Offset"<< setw(10) <<"Hit/Miss"<< endl;

    numberLines /= x;
    for(int i=0;i< accessSequenceInstruction.size();i++)
    {
       // cout << accessSequence[i] << endl;
        binary = intToBinaryString(accessSequenceInstruction[i]);
        int lineNumber = binaryToDecimal(binary.substr(32 - numberLinesBits - blockSizeBits, numberLinesBits));//binaryToDecimal(binary.substr(32 - numberLinesBits - blockSizeBits, (32 - blockSizeBits)));
        //cout << lineNumber << endl;
        bool test = 0;
        bool hit = 0;
        int k;
        int r;
        for (int j = 0; j < x; j++)
        {
            numAccess++;
            if (cacheInstruction[(lineNumber % numberLines) * x + j] != "" && binaryToDecimal(cacheInstruction[(lineNumber % numberLines) * x + j]) == accessSequenceInstruction[i])
            {
                numHit++;
                hit = 1;
                break;
            }
            else {
                numMiss++; 
            }
        }

        if (!hit) {
            for (int j = 0; j < x; j++)
            {
                if (cacheInstruction[(lineNumber % numberLines) * x + j] == "")
                {
                    test = 1;
                    k = j;
                    break;
                }
            }
            if (test)
                cacheInstruction[(lineNumber % numberLines) * x + k] = binary;
            else
            {
                r = rand() % x;

                cacheInstruction[(lineNumber % numberLines) * x + r] = binary;
            }
        }
        printCache(binary, numberLinesBits, blockSizeBits, hit);

        cout << endl << endl << "====================" << endl;
        cout << "Instruction Cache:\n";
        print(cacheInstruction);
        cout << endl << endl << "====================" << endl;
    }
    cout << "Data Cache:\n";
    cout << setw(15) << "Tag" << setw(25) << "Line Number" << setw(10) << "Offset" << setw(10) << "Hit/Miss" << endl;

    numberLines /= x;
    for (int i = 0; i < accessSequenceData.size(); i++)
    {
        // cout << accessSequence[i] << endl;
        binary = intToBinaryString(accessSequenceData[i]);
        int lineNumber = binaryToDecimal(binary.substr(32 - numberLinesBits - blockSizeBits, numberLinesBits));//binaryToDecimal(binary.substr(32 - numberLinesBits - blockSizeBits, (32 - blockSizeBits)));
        //cout << lineNumber << endl;
        bool test = 0;
        bool hit = 0;
        int k;
        int r;
        for (int j = 0; j < x; j++)
        {
            numAccess++;
            if (cacheData[(lineNumber % numberLines) * x + j] != "" && binaryToDecimal(cacheData[(lineNumber % numberLines) * x + j]) == accessSequenceData[i])
            {
                numHit++;
                hit = 1;
                break;
            }
            else {
                numMiss++;
            }
        }

        if (!hit) {
            for (int j = 0; j < x; j++)
            {
                if (cacheData[(lineNumber % numberLines) * x + j] == "")
                {
                    test = 1;
                    k = j;
                    break;
                }
            }
            if (test)
                cacheData[(lineNumber % numberLines) * x + k] = binary;
            else
            {
                r = rand() % x;

                cacheData[(lineNumber % numberLines) * x + r] = binary;
            }
        }
        printCache(binary, numberLinesBits, blockSizeBits, hit);

        cout << endl << endl << "====================" << endl;
        cout << "Data Cache:\n";
        print(cacheData);
        cout << endl << endl << "====================" << endl;
    }
    cout << " the number of hits is : " << numHit << endl;
    cout << " the number of Miss is : " << numMiss << endl;
    cout << " the number of Access is : " << numAccess << endl;
    cout << " the hit ratio is : " << ((numHit * 1.0) / (numAccess * 1.0))*100 <<"%" << endl;
    cout << " the miss ratio is : " << ((numMiss * 1.0)/(numAccess *1.0)) * 100 << "%" << endl;
    cout << " the AMAT is: " << numberCycles + ((numMiss * 1.0) / numAccess) * 100 << "\n";
    cout << "Instruction Cache:\n";
    print(cacheInstruction);
    cout << "Data Cache:\n";
    print(cacheData);
}

int main()
{
    srand(time(NULL));
    cacheManger();

    // test  b ---> d and d-->b
    //cout << binaryToDecimal(intToBinaryString(87));
}

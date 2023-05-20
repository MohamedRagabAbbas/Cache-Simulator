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

string intToBinaryString(int number,int bits) {
    if (number == 0) {
        return "0";
    }
    string binary;
    while (number > 0) {
        binary = to_string(number % 2) + binary;
        number /= 2;
    }
   /* while (bits < binary.length())
        binary = '0' + binary;*/
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
    cout << "Welcome to the cache simulator....." << endl;
    cout << "Please enter the cache size : " << endl;
    cin >> cacheSize;
    cout << "Please enter the number of lines in the cache : " << endl;
    cin >> numberLines;
    blockSize = cacheSize / numberLines;
    cout << "Please enter the number of cycles (it must be between 1 and 10 ): " << endl;
    cin >> numberCycles;
    cout << "Please enter the file name that contains access aequence: " << endl;
    cin >> fileName;
    
    
    // reading the file that contains  that the used just entered
    readIntegersFromFile(fileName);
    // creating the cache table
    vector<vector<int>> cache(numberLines, vector<int>(blockSize,0));
    string binary;
    numberLinesBits = log2(numberLines);
    blockSizeBits = log2(blockSize);
    int i, j;
    print2DVector(cache);
    while (accessSequence.size() > 0)
    {

        i = accessSequence[0] % numberLines;
        j = (accessSequence[0] / numberLines) % blockSize;

       //cout << " i : " << i << "   j : " << j << endl;

       if (cache[i][j] == accessSequence[0])
       {
           numAccess++;
           numHit++;
       }
       else
       {
           numAccess++;
           numMiss++;
           cache[i][j] = accessSequence[0];
       }
      // cout << accessSequence[0] << endl;
       print2DVector(cache);
       accessSequence.erase(accessSequence.begin(), accessSequence.begin() + 1);
       //cout << "sds" << endl;
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

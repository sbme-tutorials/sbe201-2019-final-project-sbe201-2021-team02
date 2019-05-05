#include <iostream>
#include "huffman.hpp"
using namespace std;

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cout << "Usage:\n\t huffmanCoding inputfile outputfile" << endl;
		exit(1);
	}
    huffman h;
    h.in_file_name= argv[1];
    vector<string> code= h.encode();
	for (int i = 0; i < code.size(); i++)
    {
        cout<< code[i];
    }
    
	return 0;
}
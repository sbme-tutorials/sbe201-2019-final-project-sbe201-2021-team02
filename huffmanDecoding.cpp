#include <iostream>
#include "huffman.hpp"
using namespace std;

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cout << "Please insert the compressed file you want to decompress" << endl;
		exit(1);
	}
	huffman h;
	h.in_file_name = argv[1];
	h.backToOriginal();

	return 0;
}
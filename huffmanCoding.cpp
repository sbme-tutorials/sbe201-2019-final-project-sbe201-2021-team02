#include <iostream>
#include "huffman.hpp"
using namespace std;

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cout << "Please insert the file you want to compress" << endl;
		exit(1);
	}
	huffman h;
	h.in_file_name = argv[1];
	string code = h.encode();
	h.printTotxt(h.compress(code));

	return 0;
}
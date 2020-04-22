#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <string>
#include <queue>
#include <vector>
#include <fstream>
#include <map>
#include <bitset>
using namespace std;

struct huffman_node
{
	char id;
	int freq; //frequency of the character
	huffman_node *left;
	huffman_node *right;

	huffman_node()
	{ //constructor for the initialization of the pointers
		left = right = NULL;
	}
};
typedef huffman_node *node_ptr;

class huffman
{

protected:
	ifstream in_file;	//declaration of an input file that I will read from
	ofstream out_file;	//declaration of an output file that I will write into
	node_ptr root;		//I will use it as the root of the huffman tree
	char id;

	class compare
	{ //an object funtion to set comparing rule of priority queue
	public:
		bool operator()(const node_ptr &c1, const node_ptr &c2) const
		{
			return c1->freq > c2->freq;
		}
	};
	priority_queue<node_ptr, vector<node_ptr>, compare> pq; //priority queue of frequency from high to low
	map<char, int> charFreq;	//map of characters and frequencies
	map<char, string> charCode; //map of characters and huffman codes

public:
	string in_file_name, out_file_name;
	void create_pq()							
	{ //make a priority queue using the frequencies of the characters in the file
		in_file.open(in_file_name, ios::in);	//open the text file I want to compress
		in_file.get(id);						//get the first character
		while (!in_file.eof())					//iterate till the end of the file
		{
			charFreq[id]++;						// compute the frequencies of the characters and store it in the map
			in_file.get(id);
		}
		in_file.close();
		for (auto &[k, v] : charFreq)			//the making of the priority queue using the frequency map
		{
			node_ptr temp = new huffman_node;
			temp->id = k;
			temp->freq = v;
			pq.push(temp);
		}
	}

	node_ptr create_huffman_tree(priority_queue<node_ptr, vector<node_ptr>, compare> &q)
	{
		priority_queue<node_ptr, vector<node_ptr>, compare> temp(q);
		while (temp.size() > 1)
		{ //create the huffman tree with highest frequecy characher being leaf from bottom to top
			root = new huffman_node;
			root->freq = 0;
			root->left = temp.top();
			root->freq += temp.top()->freq;
			temp.pop();
			root->right = temp.top();
			root->freq += temp.top()->freq;
			temp.pop();
			temp.push(root);
		}
		return temp.top();
	}

	void traverse(node_ptr node, string code = "")
	{ //traverse over the tree to compute the huffman codes of the characters
		if (node->left == NULL && node->right == NULL)
		{
			charCode[node->id] = code;	//after reaching a leaf store the character in a map with its corresponding code
		}
		else
		{
			traverse(node->left, code + '0');
			traverse(node->right, code + '1');
		}
	}

	void calculate_huffman_codes()
	{ //passing the root of the huffman tree to the traverse function to calculate the codes
		traverse(root, "");
	}

	string encode()
	{ //encode the text file into one string of huffman code
		string encode;
		create_pq();
		create_huffman_tree(pq);
		calculate_huffman_codes();	//getting the codes of the characters
		in_file.open(in_file_name, ios::in);
		in_file.get(id);
		while (!in_file.eof())	//iterate over the text file and convert every character into its corresponding code
		{
			encode += charCode[id]; //insert the code of the character into the encode string
			in_file.get(id);	//get the next character
		}
		in_file.close();

		return encode;
	}

	string compress(const string code)
	{	//compress the encode string which I have got from the previous function using a bitset to reduce its size
		string compressed;
		string str;				
		str.clear();
		for (auto c : code)	//iterate over the encode string
		{
			str.push_back(c);
			if (str.size() == 8)	// takes 8 bits at a time and insert them into the bitset to compress them	
			{ //convert binary to decimal and then convert that decimal to a character using the ASCII table
				char chr = bitset<8>(str).to_ulong();	
				compressed.push_back(chr);
				str.clear();		//clear the string to push another 8 bits into it 
			}						//repeat the process till the end of the encode string
		}
		return compressed;			//return the compressed string which I will print into the encoded file
	}								//and that will be the compressed output file from the compressing process

	void printTotxt(const string compressed)
	{ //print the compressed string into a text file
		out_file.open("encoded.txt");						//arbitrary name for the file I will print into
		map<char, int>::iterator it = charFreq.begin();		//iterator for iteration over the frequency map
		for (it; it != charFreq.end(); it++)
		{ //print the frequency map into the output file for using it in the decompressing process
			out_file << it->first;
			out_file << '%';								//seperator between the character and its frequency
			out_file << it->second;
			out_file << '^';								//seperator between the frequenct and the next frequency
		}

		out_file << '$';									//seperator between the map and the compressed encode string

		for (int i = 0; i < compressed.size(); i++)
		{ //print the compressed encode string into the output file
			out_file << compressed[i];
		}
		out_file.close();
	}
};
#endif

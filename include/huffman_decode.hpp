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

	map<char, int> getMap()
	{ //consruct the frequency map again from the compressed file to use it in the decomprssion process
		map<char, int> freqMAp;
		string v;
		char k;
		in_file.open(in_file_name, ios::in);

		while (id != '$')		//terminate after reading the seperator between the map and the encode string
		{
			in_file.get(id);
			if (id == '$')
				break;			//just to make sure I will really terminate when I see this sign :)

			if (id == '%')		//continue if you see this seperator
				continue;
			else
				k = id;			// if you didn't see this '%' store the character in k

			in_file.get(id);
			in_file.get(id);
			while (id >= '0' && id <= '9')	
			{ //make sure that the next characters are only the frequency of the previous character
				if (id == '%' || id == '^')
					break;					//To make sure I will print only the frequency

				v += id;					//store the frequency of the charater k in v
				in_file.get(id);
			}
			int freq = 0;
			if (v.size() > 0)
			{
				freq = stoi(v);							  // casting the frequency from string to integers
				freqMAp.insert(pair<char, int>(k, freq)); //insert the charater with its corresponding frequency in the map
			}
			v.clear();			//clear the frequency from the v string to make it ready to store the next frequency			
		}
		in_file.close();

		return freqMAp;
	}

	node_ptr recreate_huffman_tree()
	{ /*use the map I have got from the previous function to recreate the huffman tree 
	  using the same logic used in the compression process*/
		priority_queue<node_ptr, vector<node_ptr>, compare> q;
		map<char, int> freqMap = getMap();
		for (auto &[k, v] : freqMap)
		{ 
			node_ptr temp = new huffman_node;
			temp->id = k;
			temp->freq = v;
			q.push(temp);
		}
		return create_huffman_tree(q);
	}

	string decode()
	{ //read the compressed encode file and convert it into the original encoded one "the huffman code of the original file"
		string str;
		str.clear();
		in_file.open(in_file_name, ios::in);
		in_file.get(id);
		while (id != '$')
		{ // iterate to avoid reading the map
			in_file.get(id);
		}
		in_file.get(id); 			//read the first character of the compressed code

		while (!in_file.eof())
		{
			in_file.get(id);
			bitset<8> chr = id;
			str += chr.to_string();	//convert the every character into its original binary code
		}
		return str;					//return the original huffman code
	}

	void decompress(node_ptr node, string code)
	{ /*iterate over the code I have got from the previous function 
	  and convert every code into its corresponding character to reach finally to the original text file*/
		out_file.open("decoded.txt");

		int i = 0;
		node_ptr current = node;
		while (i < code.size())
		{
			if (current->left == NULL && current->right == NULL)
			{ //print the character if you reach to a leaf and reset the current pointer to the root
				out_file << current->id;
				current = node;
			}
			if (code[i] == '1') //traverse over the huffman tree till reaching a leaf
				current = current->right;
			else if (code[i] == '0')
				current = current->left;

			++i;
		}
		out_file.close();
	}

	void backToOriginal()
	{ //recreate the huffman tree and decompressing the compressed file using the previous functions
		node_ptr temp;
		temp = recreate_huffman_tree();
		decompress(temp, decode());
	}
};

#endif

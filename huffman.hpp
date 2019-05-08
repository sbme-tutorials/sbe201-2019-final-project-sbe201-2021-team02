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
	{
		left = right = NULL;
	}
};
typedef huffman_node *node_ptr;

class huffman
{

protected:
	ifstream in_file;
	ofstream out_file;
	node_ptr root;
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
	map<char, int> charFreq;
	map<char, string> charCode;

public:
	string in_file_name, out_file_name;
	void create_pq()
	{
		in_file.open(in_file_name, ios::in);
		in_file.get(id);
		while (!in_file.eof())
		{
			charFreq[id]++;
			in_file.get(id);
		}
		in_file.close();
		for (auto &[k, v] : charFreq)
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
	{
		if (node->left == NULL && node->right == NULL)
		{
			charCode[node->id] = code;
		}
		else
		{
			traverse(node->left, code + '0');
			traverse(node->right, code + '1');
		}
	}

	void calculate_huffman_codes()
	{
		traverse(root, "");
	}

	string encode()
	{
		string encode;
		create_pq();
		typedef huffman_node *node_ptr;
		create_huffman_tree(pq);
		calculate_huffman_codes();
		in_file.open(in_file_name, ios::in);
		in_file.get(id);
		while (!in_file.eof())
		{
			encode += charCode[id];
			in_file.get(id);
		}
		in_file.close();

		return encode;
	}

	string compress(const string code)
	{
		string compressed;
		string str;
		str.clear();
		for (auto c : code)
		{
			str.push_back(c);
			if (str.size() == 8)
			{
				char chr = bitset<8>(str).to_ulong();
				compressed.push_back(chr);
				str.clear();
			}
		}
		return compressed;
	}

	void printTotxt(const string compressed)
	{
		out_file.open("encoded.txt");
		map<char, int>::iterator it = charFreq.begin();
		for (it; it != charFreq.end(); it++)
		{
			out_file << it->first;
			out_file << '%';
			out_file << it->second;
			out_file << '^';
		}

		out_file << '$';

		for (int i = 0; i < compressed.size(); i++)
		{
			out_file << compressed[i];
		}
		out_file.close();
	}

	map<char, int> getMap()
	{
		map<char, int> freqMAp;
		string v;
		char k;
		in_file.open(in_file_name, ios::in);

		while (id != '$')
		{
			in_file.get(id);
			if (id == '$')
				break;

			if (id == '%')
				continue;
			else
				k = id;

			in_file.get(id);
			in_file.get(id);
			while (id >= '0' && id <= '9')
			{
				if (id == '%' || id == '^')
					break;

				v += id;
				in_file.get(id);
			}
			int freq = 0;
			if (v.size() > 0)
			{
				freq = stoi(v);
				freqMAp.insert(pair<char, int>(k, freq));
			}
			v.clear();
		}
		in_file.close();

		return freqMAp;
	}

	node_ptr recalculate_huffman_codes()
	{
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
	{
		string str;
		str.clear();
		in_file.open(in_file_name, ios::in);
		in_file.get(id);
		while (id != '$')
		{
			in_file.get(id);
		}
		in_file.get(id);

		while (!in_file.eof())
		{
			in_file.get(id);
			bitset<8> chr = id;
			str += chr.to_string();
		}
		return str;
	}

	void decompress(node_ptr node, string code)
	{
		out_file.open("decoded.txt");

		int i = 0;
		node_ptr current = node;
		while (i < code.size())
		{
			if (current->left == NULL && current->right == NULL)
			{
				out_file << current->id;
				current = node;
			}
			if (code[i] == '1')
				current = current->right;
			else if (code[i] == '0')
				current = current->left;

			++i;
		}
		out_file.close();
	}

	void backToOriginal()
	{
		node_ptr temp;
		temp = recalculate_huffman_codes();
		decompress(temp, decode());
	}
};

#endif

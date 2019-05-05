#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <string>
#include <queue>
#include <vector>
#include <fstream>
#include <map>
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
	node_ptr child, parent, root;
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

	node_ptr create_huffman_tree()
	{
		priority_queue<node_ptr, vector<node_ptr>, compare> temp(pq);
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

	vector<string> encode()
	{
		vector<string> encode;
		create_pq();
		create_huffman_tree();
		calculate_huffman_codes();
		in_file.open(in_file_name, ios::in);
		in_file.get(id);
		while (!in_file.eof())
		{
			encode.push_back(charCode[id]);
			in_file.get(id);
		}
		in_file.close();

		return encode;
	}
};

#endif

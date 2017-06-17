//
// Created by YZH on 2017/5/11.
//
#ifndef GUARD_TrieTree_h
#define GUARD_TrieTree_h

#include <map>
#include <string>
#include <vector>
#include "DLeftCountingBloomFilter.h"
#include "CountingBloomFilter.h"
#include "BKDRHash.h"
#include <unordered_map>

using std::unordered_map;
using std::string;
using std::vector;

class Node
{
public:

	Node();
	~Node();

	Node* search(const char&);	//查找存储c的子结点
	void addChild(const char&, Node*);
	void setIpList(const string&, const string&);
	BKDR_Hash* getIpList();
	unordered_map<int, DLeftCountingBloomFilter*>* getDLeftList();
	unordered_map<int, CountingBloomFilter*>* getCBFList();
	void addDLeft(int,const string&);
	void removeDLeft(int,const string&);
	void addCBF(int,const string&);
	void removeCBF(int,const string&);

private:

	unordered_map<char, Node*> childs;	//子结点表
	BKDR_Hash ipList;
	unordered_map<int, DLeftCountingBloomFilter*> dleftList;
	unordered_map<int, CountingBloomFilter*> cbfList;
};

class TrieTree
{
public:

	TrieTree();
	TrieTree(Node *);
	~TrieTree();
	Node* searchNode(const string&);

private:

	Node *root;

};
//获得后缀
string getPostfix(const string&);
//获得前缀
string getPrefix(const string&);
TrieTree createTrieTree(const unordered_map<string, string>&);
int getLength(const string&);
void showMemoryInfo(void);
#endif 

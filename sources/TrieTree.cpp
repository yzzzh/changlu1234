//
// Created by YZH on 2017/5/11.
//

#include <algorithm>
#include <unordered_map>
#include "../headers/TrieTree.h"
#include <windows.h>
#include <psapi.h>
#pragma comment(lib,"psapi.lib")
using std::unordered_map;
using std::string;
using std::vector;
using std::unordered_map;
using std::find_if;
using std::cout;
using std::endl;

Node::Node() {}

Node::~Node() {}

Node* Node::search(const char& c)
{
    unordered_map<char, Node*>::iterator i;
    i = childs.find(c);
    if (i == childs.end())
    {
        return NULL;
    }
    else
    {
        return i->second;
    }
}

void Node::addChild(const char& c, Node* child)
{
    childs[c] = child;
}
void Node::setIpList(const string &prefix, const string& ip)
{
    ipList.insert(prefix,ip);

}

BKDR_Hash* Node::getIpList()
{
    return &ipList;
}


unordered_map<int, DLeftCountingBloomFilter*>* Node::getDLeftList()
{
    return &dleftList;
}
unordered_map<int, CountingBloomFilter*>* Node::getCBFList()
{
    return &cbfList;
}

void Node::addDLeft(int i,const string& str)
{
    if (dleftList[i] == NULL)
        dleftList[i] = new DLeftCountingBloomFilter();
    (*dleftList[i]).add(str);
}

void Node::removeDLeft(int i,const string& str)
{
    if (dleftList[i] == NULL)
        dleftList[i] = new DLeftCountingBloomFilter();
    (*dleftList[i]).remove(str);
}

void Node::addCBF(int i,const string& str)
{
    if (cbfList[i] == NULL)
        cbfList[i] = new CountingBloomFilter();
    (*cbfList[i]).add(str);
}

void Node::removeCBF(int i,const string& str)
{
    if (cbfList[i] == NULL)
        cbfList[i] = new CountingBloomFilter();
    (*cbfList[i]).remove(str);
}

TrieTree::TrieTree() {}

TrieTree::TrieTree(Node *node) : root(node) {}

TrieTree::~TrieTree() {}

Node* TrieTree::searchNode(const string& name)
{
    Node* temp1, *temp2;
    temp1 = root;
    for (char c : name)
    {
        temp2 = (*temp1).search(c);
        if (!temp2)
        {
            return NULL;
        }
        temp1 = temp2;
    }
    return temp1;
}
string getPostfix(const string& url)
{
    string::size_type begin;
    begin = url.find_last_of(".");
    return url.substr(begin + 1);
}

string getPrefix(const string& url)
{
    string::size_type begin;
    begin = url.find_last_of(".");
    return url.substr(0, begin);
}

TrieTree createTrieTree(const unordered_map<string, string>& routerlist)
{
    Node *root = new Node(), *temp1, *temp2;
    string topName;
    string prefix;
    unordered_map<string, string>::const_iterator mit;
    for (mit = routerlist.begin(); mit != routerlist.end(); mit++)
    {
        temp1 = root;
        topName = getPostfix(mit->first);//获取顶级域名
        prefix = getPrefix(mit->first);
        for (char c : topName)
        {
            temp2 = (*temp1).search(c);
            if (!temp2)
            {
                temp2 = new Node();
                (*temp1).addChild(c, temp2);
            }
            temp1 = temp2;
        }
        int length = getLength(prefix);
        temp1->setIpList(prefix, mit->second);
        if (length >= 3)
        {
            temp1->addDLeft(3,prefix);
            temp1->addCBF(3,prefix);
        }
        else
        {
            temp1->addDLeft(length,prefix);
            temp1->addCBF(length,prefix);
        }
    }
    return TrieTree(root);
}

int getLength(const string& s)
{
    int sum = 0;
    char ch = '.';
    for (char c : s)
    {
        if (c == ch)
            sum++;
    }
    return ++sum;
}

//void showMemoryInfo(void)
//{
//    HANDLE handle = GetCurrentProcess();
//    PROCESS_MEMORY_COUNTERS pmc;
//    GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
//    cout << "Memory Use:" << pmc.WorkingSetSize / 1024.0f << "KB/" << pmc.PeakWorkingSetSize / 1024.0f << "KB, Virtual Memory Use:" << pmc.PagefileUsage / 1024.0f << "KB/" << pmc.PeakPagefileUsage / 1024.0f << "KB" << endl;
//}
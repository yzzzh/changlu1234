//
// Created by YZH on 2017/5/11.
//
#pragma once
#ifndef GUARD_Routerlist_h
#define GUARD_Routerlist_h

#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <map>
#include <algorithm>

#include "TrieTree.h"

using std::list;
using std::string;
using std::ifstream;
using std::getline;
using std::unordered_map;
using namespace std;

bool compare(string elem1, string elem2)
{
    return getPostfix(elem1) < getPostfix(elem2);
}

unordered_map<string, string> createRouterlist(ifstream& infile)
{
    unordered_map<string, string> routerlist;
    string url;
    string ip;
    while (getline(infile, url))
    {
        getline(infile, ip);
        routerlist[url] = ip;
    }
    return routerlist;
}


#endif

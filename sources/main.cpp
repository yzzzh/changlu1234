#include <iostream>
#include <fstream>
#include <map>
#include <list>
#include <time.h>
#include <unordered_map>
#include <windows.h>
#include <stdio.h>
#include <psapi.h>
#include "../headers/TrieTree.h"
#include "../headers/Routerlist.h"
#include "../headers/DLeftCountingBloomFilter.h"
#include "../headers/BKDRHash.h"


using std::unordered_map;
using std::cout;
using std::endl;
using std::ifstream;
using std::list;
using std::unordered_map;
using std::iterator;

string path_name_data("D:\\C++_Project\\changlu\\data.txt");
string path_name_diffdata("D:\\C++_Project\\changlu\\diffdata.txt");

unordered_map<string, string> routerlist;
TrieTree trietree;
vector<string> urls;
vector<string> prefixs;
vector<string> postfixs;
ofstream res;

int test_hash();
int test_trie();
int test_dleft();
int test_cbf();
void init();
void test_singleEfficiency();
void write_data();
void test_shorten();
string shorten(string&);
void create_hash_list(string&);
void create_trietree();
void test(int, double);
void generate_data(int, double);

int TEST_NUM = 100000;
string PREFIX = "aaa.bbb.ccc.ddd.eee";
string POSTFIX = "com";
string path_resultdata = "D:\\C++_Project\\changlu\\resultdata.txt";

int main()
{
    res.open(path_resultdata);

    //cin>>TEST_NUM;
    init();
    test(TEST_NUM, 0);

    //生成不同命中率和总量的数据
    //cout << "------------------命中率：0%------------------"<< endl;
    //for (int num = 1000; num <= 100000; num += 2000)
    //{
    //	//total = num;
    //	test(num, 0.0);
    //}
    //cout << "------------------命中率：100%------------------" << endl;
    //for (int num = 1000; num <= 100000; num += 2000)
    //{
    //	//total = num;
    //	test(num, 1.0);
    //}

    res.close();
}

void init()
{
    //建立路由表和树
    create_hash_list(path_name_data);
    create_trietree();

    cout << "路由表建立完成！" << endl;
}

void test(int num, double rate)
{
    //generate_data(num, rate);

    //test_singleEfficiency();

    test_shorten();

}

void create_hash_list(string& path)
{
    //根据原有数据构建Trie树和hash表

    ifstream infile;
    infile.open(path);
    routerlist.clear();
    routerlist = createRouterlist(infile);
    infile.close();
}

void create_trietree()
{
    trietree = createTrieTree(routerlist);
}

int test_dleft()
{
    clock_t start, end;

    cout << "Dleft : " << endl;

    start = clock();
    //int n = 0, m = 0;
    //dleft_target = trie_target;
    for (int i = 0; i < (int) urls.size(); i++)
    {
        //m++;
        string prefix = prefixs[i];
        string postfix = postfixs[i];
        Node* node = trietree.searchNode(postfix);
        if (node != NULL)
        {
            //域名退化,
            unordered_map<int, DLeftCountingBloomFilter*>* dleftList = node->getDLeftList();
            BKDR_Hash* list = node->getIpList();
            int length = getLength(prefix);
            while (length > 0)
            {
                int index = length >= 3 ? 3 : length;
                DLeftCountingBloomFilter* dleft = (*dleftList)[index];
                if (dleft==NULL || !(*dleft).isContain(prefix))
                {
                    prefix = shorten(prefix);
                }
                else
                {
                    (*list)[prefix];
                    break;
                }
                length--;
            }
        }
    }

    end = clock();
    //dleft_target = abs(dleft_target);

    cout << end - start << endl;
    //cout << dleft_target << endl;
    //cout << n << endl;
    //cout << m << endl << endl;

    return end - start;
}

int test_cbf()
{
    clock_t start, end;

    cout << "cbf : " << endl;

    start = clock();
    //int n = 0, m = 0;
    //cbf_target = trie_target;
    for (int i = 0; i < (int)urls.size(); i++)
    {
        //m++;
        string prefix = prefixs[i];
        string postfix = postfixs[i];
        Node* node = trietree.searchNode(postfix);
        if (node != NULL)
        {
            //域名退化,
            unordered_map<int, CountingBloomFilter*>* cbfList = node->getCBFList();
            BKDR_Hash* list = (node->getIpList());
            int length = getLength(prefix);
            while (length > 0)
            {
                int index = length >= 3 ? 3 : length;
                CountingBloomFilter* cbf = (*cbfList)[index];
                if (cbf==NULL || !(*cbf).isContain(prefix))
                {
                    prefix = shorten(prefix);
                }
                else
                {
                    (*list)[prefix];
                    break;
                }
                length--;
            }
        }
    }

    end = clock();
    //cbf_target = abs(cbf_target);

    cout << end - start << endl;
    //cout << cbf_target << endl;
    //cout << n << endl;
    //cout << m << endl << endl;

    return end - start;
}

int test_trie()
{
    clock_t start, end;

    cout << "trie : " << endl;

    start = clock();
    //int n = 0, m = 0;
    //trie_target = 0;
    for (int i = 0; i< (int)urls.size(); i++)
    {
        //m++;
        string prefix = prefixs[i];
        string postfix = postfixs[i];
        Node* node = trietree.searchNode(postfix);
        if (node != NULL)
        {
            int length = getLength(prefix);
            while (length>0)
            {
                //	n++;
                //trie_target++;
                if ((*(node->getIpList()))[prefix] == "")
                    prefix = shorten(prefix);
                else
                    break;
                length--;
                //(*(node->getIpList()))[prefix];
            }
        }
    }

    end = clock();
    //trie_target = n;
    cout << end - start << endl;
    //cout << n << endl;
    //cout << m << endl << endl;


    return end - start;
}

int test_hash()
{
    clock_t start, end;
    int m = 0, n = 0;
    cout << "hash : " << endl;

    start = clock();
    for (string url : urls)
    {
        m++;
        if (routerlist[url] != "") {
            n++;
        }
    }

    end = clock();
    cout << end - start << endl << endl;
    cout << n << endl;
    cout << m << endl << endl;
    return end - start;
}

//三种东西的单次访问时间
void test_singleEfficiency()
{
    clock_t start, end;
    Node* node = trietree.searchNode(POSTFIX);
    BKDR_Hash* list = node->getIpList();
    unordered_map<int, CountingBloomFilter*>* cbfList = node->getCBFList();
    unordered_map<int, DLeftCountingBloomFilter*>* dleftList = node->getDLeftList();
    CountingBloomFilter* cbf = (*cbfList)[3];
    DLeftCountingBloomFilter* dleft = (*dleftList)[3];

    res << "单次访问时间" << endl;
    res << "类型 总时间 访问次数"<< endl;
    unordered_map<string,string> map;
    //-----------------------------------------------
    for(int k = 100000;k <=500000;k+=100000)
    {
        //cout << "trie:" << endl;
        start = clock();
        for (int i = 0; i < k; i++)
            (*list)[PREFIX];
        end = clock();
        res << "trie " << end - start << " " << k << endl;

        start = clock();
        for (int i = 0; i < k; i++)
            (map)[PREFIX];
        end = clock();
        //cout << end - start << endl;
        res << "map " << end - start << " " << k << endl;
        //-----------------------------------------------
        //cout << "cbf:" << endl;
        start = clock();
        for (int i = 0; i < k; i++)
            (*cbf).isContain(PREFIX);
        end = clock();
        //cout << end - start << endl;
        res << "cbf " << end - start << " " << k << endl;
        //-----------------------------------------------
        //cout << "dleft:" << endl;
        start = clock();
        for (int i = 0; i < k; i++)
            (*dleft).isContain(PREFIX);
        end = clock();
        //cout << end - start << endl;
        res << "dleft " << end - start << " " << k << endl<<endl;
    }
}

//不同退化次数下的效率
void test_shorten()
{
    clock_t start, end;
    res << "类型 退化次数 总时间 查询次数"<< endl ;
    //------------------------------
    //cout << "trie : " << endl;

    int k = 0;
    cin>>k ;
    for(int j=0;j<3;j++) {
        start = clock();
//        cout << "trie" <<endl;
        for (int i = 0; i < TEST_NUM; i++) {
            string pre = PREFIX;
            Node *node = trietree.searchNode(POSTFIX);
            BKDR_Hash* list = node->getIpList();
            if (node) {
                int length = getLength(pre);
                while (length > 0) {
                    if ((*(list))[pre] == "") {
                        pre = shorten(pre);
//                        cout << "false"<<endl;
                    }
                    else {
//                        cout << "true" << endl;
                        break;
                    }
                    length--;
                }
            }
        }

        end = clock();
        res << "trie " << k << " " << end - start << " " << TEST_NUM << endl;
        //cout << end - start << endl;
        //---------------------------------
        //cout << "cbf : " << endl;

        start = clock();
//        cout <<"cbf"<<endl;
        for (int i = 0; i < TEST_NUM; i++) {
            Node *node = trietree.searchNode(POSTFIX);
            string pre = PREFIX;
            if (node) {
                unordered_map<int, CountingBloomFilter *> *cbfList = node->getCBFList();
                BKDR_Hash *list = (node->getIpList());
                int length = getLength(pre);
                while (length > 0) {
                    int index = length >= 3 ? 3 : length;
                    CountingBloomFilter *cbf = (*cbfList)[index];
                    if(cbf)
                    {
                        if (!(*cbf).isContain(pre))
                        {
                            pre = shorten(pre);
//                            cout<<"false"<<endl;
                            //cout <<pre<<endl;
                        }
                        else
                        {
                            (*list)[pre];
//                            cout<<"true"<<endl;
                            break;
                        }
                    }
                    length--;
                }
            }
        }
        end = clock();
        res << "cbf " << k << " " << end - start << " " << TEST_NUM << endl;
        //cout << end - start << endl;
        //---------------------------------------
        //cout << "Dleft : " << endl;

        start = clock();
//        cout<<"dleft"<<endl;
        for (int i = 0; i < TEST_NUM; i++) {
            Node *node = trietree.searchNode(POSTFIX);
            string pre = PREFIX;
            if (node) {
                unordered_map<int, DLeftCountingBloomFilter *> *dleftList = node->getDLeftList();
                BKDR_Hash *list = node->getIpList();
                int length = getLength(pre);
                while (length > 0)
                {
                    int index = length >= 3 ? 3 : length;
                    DLeftCountingBloomFilter *dleft = (*dleftList)[index];
                    if(dleft) {
                        if (!(*dleft).isContain(pre))
                        {
                            pre = shorten(pre);
//                            cout<<"false"<<endl;
                            //cout <<pre<<endl;
                        }
                        else
                        {
                            (*list)[pre];
//                            cout<<"true"<<endl;
                            break;
                        }
                    }
                    length--;
                }
            }
        }
        end = clock();
        res << "dleft " << k << " " << end - start << " " << TEST_NUM << endl;
    }
}

//不同数据量不同命中率所跑的时间，写入文件
void write_data(int num,int rate)
{
    int r = 0;

    r += test_trie();
    r += test_trie();
    r += test_trie();
    res << "trie " << num << " " << rate << " " << r/3 <<endl;

    r = 0;
    r += test_cbf();
    r += test_cbf();
    r += test_cbf();
    res << "cbf " << num << " " << rate << " " << r/3 << endl;

    r = 0;
    r += test_dleft();
    r += test_dleft();
    r += test_dleft();
    res << "dleft " << num << " " << rate << " " << r/3 << endl;
}

//产生测试数据
void generate_data(int count_total_data, double rate) {
    string url;
    string ip;

    ifstream data;
    urls.clear();
    prefixs.clear();
    postfixs.clear();

    data.open(path_name_data);
    int count_data = (int)(count_total_data * rate);
    int counter_data = 1;
    while (getline(data, url)) {
        if (counter_data > count_data)
            break;
        urls.push_back(url);
        prefixs.push_back(getPrefix(url));
        postfixs.push_back(getPostfix(url));
        getline(data, ip);
        counter_data++;
    }
    data.close();

    ifstream diff_data;
    diff_data.open(path_name_diffdata);
    int count_diffdata = count_total_data - count_data;
    int counter_diffdata = 1;
    while (getline(diff_data, url)) {
        if (counter_diffdata > count_diffdata)
            break;
        urls.push_back(url);
        prefixs.push_back(getPrefix(url));
        postfixs.push_back(getPostfix(url));
        getline(diff_data, ip);
        counter_diffdata++;
    }
    diff_data.close();
}

//域名退化
string shorten(string &str)
{
    string::size_type begin;
    begin = str.find_first_of(".");
    if (begin == string::npos)
        return "";
    return str.substr(begin+1);
}

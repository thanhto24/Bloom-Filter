#include "header.h"

int hash_1(string s)
{
    ll int hash = 0;
    for (int i = 0; i < s.size(); i++)
    {
        hash = (hash + ((int)s[i]));
        hash = hash % MAX_SIZE;
    }
    return hash;
}

int hash_2(string s)
{
    ll int hash = 1;
    for (int i = 0; i < s.size(); i++)
    {
        hash = hash + pow(19, i) * s[i];
        hash = hash % MAX_SIZE;
    }
    return hash % MAX_SIZE;
}

int hash_3(string s)
{
    ll int hash = 7;
    for (int i = 0; i < s.size(); i++)
    {
        hash = (hash * 31 + s[i]) % MAX_SIZE;
    }
    return hash % MAX_SIZE;
}

int hash_4(string s)
{
    ll int hash = 3;
    int p = 7;
    for (int i = 0; i < s.size(); i++)
    {
        hash += hash * 7 + s[0] * pow(p, i);
        hash = hash % MAX_SIZE;
    }
    return hash;
}

bool isContain(bool *bitArray, string str)
{
    int index_1 = hash_1(str);
    int index_2 = hash_2(str);
    int index_3 = hash_3(str);
    int index_4 = hash_4(str);
    return (bitArray[index_1] && bitArray[index_2] && bitArray[index_3] && bitArray[index_4]);
}

void insertToBit(bool *bitArray, string str)
{
    int index_1 = hash_1(str);
    int index_2 = hash_2(str);
    int index_3 = hash_3(str);
    int index_4 = hash_4(str);
    bitArray[index_1] = bitArray[index_2] = bitArray[index_3] = bitArray[index_4] = true;
    return;
}

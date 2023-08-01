#include "header.h"

int hash_1(string str)
{
    return str.length() % MAX_SIZE;
}

int hash_2(string str)
{
    int hash = 3;
    for (int i = 0; i < str.length(); i++)
    {
        hash *= (int)str[i];
        hash = hash % MAX_SIZE;
    }
    return hash % MAX_SIZE;
}

int hash_3(string str)
{
    int hash = 0;
    for (int i = 0; i < str.length(); i++)
    {
        hash += int(str[i]) ^ i;
        hash %= MAX_SIZE;
    }
    return hash % MAX_SIZE;
}

int hash_4(string str)
{
    return (hash_1(str) + hash_2(str) * hash_2(str) + hash_3(str) * 13) % MAX_SIZE;
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


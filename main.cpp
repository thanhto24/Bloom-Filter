#include "operations.cpp"

int main()
{
    bool *usernameBit = buildUsernameBit();
    bool *weakPassBit = buildWeakPassBit();
    process(usernameBit, weakPassBit);
    delete[] usernameBit;
    delete[] weakPassBit;
    return 0;
}

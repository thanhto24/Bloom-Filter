#include <iostream>
#include <cstring>
#include <conio.h>
#include <fstream>
#include <vector>

using namespace std;

const int MAX_SIZE = 1000;

int hash_1(string str)
{
    return ((int)(str[0]) + (int)(str[str.length() - 1]) + (int)(str[(str.length() - 1) / 2])) % MAX_SIZE;
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

bool *buildUsernameBit()
{
    bool *bitArray = new bool[MAX_SIZE]{false};
    ifstream fin("Signup.txt");
    if (!fin)
        return bitArray;
    while (!fin.eof())
    {
        string username, tmp;
        fin >> username;
        fin.ignore();
        insertToBit(bitArray, username);
        fin >> tmp;
        fin.ignore();
    }
    fin.close();
    return bitArray;
}

bool *buildWeakPassBit()
{
    bool *bitArray = new bool[MAX_SIZE]{false};
    ifstream fin("WeakPass.txt");
    if (!fin)
        return bitArray;
    while (!fin.eof())
    {
        string weakPass;
        fin >> weakPass;
        insertToBit(bitArray, weakPass);
    }
    fin.close();
    return bitArray;
}

bool validUsername(string s)
{
    return true;
}

bool validPassword(string s)
{
    return true;
}

void process();

void backToMenu()
{
    cout << "Press any key to back to menu!\n";
    char ch;
    ch = getch();
    return process();
}

void store(string username, string pass)
{
    ofstream fout("SignUp.txt", ios::app);
    if (!fout)
    {
        cout << "Error: Can not open file!\n";
        return;
    }
    fout << username << ' ' << pass << endl;
    fout.close();
    return;
}

void storeFail(string username, string pass)
{
    ofstream fout("Fail.txt", ios::app);
    if (!fout)
    {
        cout << "Error: Can not open file!\n";
        return;
    }
    fout << username << ' ' << pass << endl;
    fout.close();
    return;
}

void registration(int type, bool usernameBit[], bool weakPassBit[])
{
    cout << "Registration\n";
    string username = "", pass = "";
    cout << "Username: ";
    getline(cin, username);
    cout << "Password: ";
    getline(cin, pass);
    if (validUsername(username) && validPassword(pass))
    {
        if (isContain(usernameBit, username))
            cout << "Username was exists\n";
        else
        {
            insertToBit(usernameBit, username);
            store(username, pass);
        }
    }
    else if (type == 2)
        storeFail(username, pass);
    if (!validUsername(username))
        cout << "Unvalid username!\n";
    if (!validPassword(pass))
        cout << "Unvalid password!\n";
    if (type == 1)
    {
        cout << "Press (L) to Register again or the others to back to menu!\n";
        char ch = getch();
        if (ch == 'L' || ch == 'l')
            registration(1, usernameBit, weakPassBit);
        else
            process();
    }
}

void multipleRegistrations(bool usernameBit[], bool weakPassBit[])
{
    cout << "Multiple registrations\n";
    cout << "Enter number of accounts you want to register: ";
    int num;
    cin >> num;
    cin.ignore();
    for (int i = 0; i < num; i++)
    {
        cout << "Account " << i + 1 << ' ';
        registration(2, usernameBit, weakPassBit);
    }
    backToMenu();
}

void changePassword(string username, string pass, bool usernameBit[], bool weakPassBit[]);

void login(int type, bool usernameBit[], bool weakPassBit[])
{
    cout << "Login\n";
    string username = "", pass = "";
    cout << "Username: ";
    getline(cin, username);
    cout << "Password: ";
    getline(cin, pass);
    if (!validUsername(username))
        cout << "Unvalid username!\n";
    if (!validPassword(pass))
        cout << "Unvalid password!\n";
    if (validUsername(username) && validPassword(pass))
    {
        if (!isContain(usernameBit, username))
            cout << "Username does not found!";
        else
        {
            ifstream fin("SignUp.txt");
            if (!fin)
            {
                cout << "Error: Can not open file!\n";
                return;
            }
            string tmpUsername = "", tmpPass = "";
            while (!fin.eof())
            {
                getline(fin, tmpUsername, ' ');
                getline(fin, tmpPass, '\n');
                if (tmpUsername == "")
                    break;
                if (username == tmpUsername && pass == tmpPass)
                {
                    fin.close();
                    if (type == 1)
                    {
                        cout << "Login sucessfully!\nPress (1) to Change Password | Press (2) to back to menu\n";
                        while (1)
                        {
                            char ch = getch();
                            if (ch == '1')
                                return changePassword(username, pass, usernameBit, weakPassBit);
                            if (ch == '2')
                                return process(usernameBit, weakPassBit);
                            cout << "Wrong input!\n";
                        }
                    }
                    else
                        return changePassword(username, pass, usernameBit, weakPassBit);
                    return;
                }
            }
            fin.close();
            cout << "Wrong username or password!\n";
        }
    }
    cout << "Press (L) to Login again or the others to back to menu!\n";
    char ch = getch();
    if (ch == 'L' || ch == 'l')
        login(1, usernameBit, weakPassBit);
    else
        process(usernameBit, weakPassBit);
}

void changePassword(string username, string pass, bool usernameBit[], bool weakPassBit[])
{
    if (username == "" || pass == "")
    {
        cout << "Please login first!\n";
        login(2, usernameBit, weakPassBit);
    }
    ifstream fin("SignUp.txt");
    if (!fin)
    {
        cout << "Error: Can not open file!\n";
        return;
    }
    vector<pair<string, string>> account;
    account.resize(0);
    string tmpUsername = "", tmpPass = "", newPass = "";
    while (!fin.eof())
    {
        getline(fin, tmpUsername, ' ');
        getline(fin, tmpPass, '\n');
        if (tmpUsername == "")
            break;
        if (tmpUsername == username && tmpPass == pass)
        {
            cout << "Input new password: ";
            getline(cin, newPass);
            while (!validPassword(newPass) || isContain(weakPassBit, newPass))
            {
                cout << "Unvalid Password, please input valid password: ";
                getline(cin, newPass);
            }
            cout << "Your Password has been changed!\n";
            tmpPass = newPass;
        }
        account.push_back({tmpUsername, tmpPass});
    }
    fin.close();
    for (int i = 0; i < account.size(); i++)
        cout << account[i].first << ' ' << account[i].second << endl;
    ofstream fout("SignUp.txt");
    if (!fout)
    {
        cout << "Error: Can not open file!\n";
        return;
    }
    for (int i = 0; i < account.size(); i++)
        fout << account[i].first << ' ' << account[i].second << endl;
    fout.close();
    backToMenu();
}

void process(bool usernameBit[], bool weakPassBit[])
{
    system("cls");
    string bar[5];
    cout << "Use arrow to move\n";
    bar[0] = "  Registration";
    bar[1] = "  Multiple registrations";
    bar[2] = "  Log-in";
    bar[3] = "  Change Password";
    bar[4] = "  Exit";
    bar[0][0] = '>';
    for (int i = 0; i < 5; i++)
        cout << bar[i] << endl;
    int x = 0;
    char key;
    while (true)
    {
        if (kbhit())
        {
            key = getch();
            if (key == 72)
                x--;
            if (key == 80)
                x++;
            if (key == 13)
            {
                switch (x)
                {
                case 0:
                    registration(1, usernameBit, weakPassBit);
                    break;
                case 1:
                    multipleRegistrations(usernameBit, weakPassBit);
                    break;
                case 2:
                    login(1, usernameBit, weakPassBit);
                    break;
                case 3:
                    changePassword("", "", usernameBit, weakPassBit);
                    break;
                case 4:
                    system("cls");
                    cout << "Exit successfully!";
                    exit(0);
                    break;
                default:
                    break;
                }
            }
            if (x == 5)
                x = 0;
            if (x == -1)
                x = 4;
            system("cls");
            cout << "Use arrow to move\n";
            bar[x][0] = '>';
            for (int i = 0; i < 5; i++)
                cout << bar[i] << endl;
            bar[x][0] = ' ';
        }
    }
}

int main()
{
    bool *usernameBit = buildUsernameBit();
    bool *weakPassBit = buildWeakPassBit();
    process(usernameBit, weakPassBit);
    delete[] usernameBit;
    delete[] weakPassBit;
    return 0;
}

#include "bloomFilter.cpp"

bool *buildUsernameBit()
{
    bool *bitArray = new bool[MAX_SIZE]{false};
    ifstream fin("UserData.txt");
    if (!fin)
        return bitArray;
    while (!fin.eof())
    {
        string username = "", pass = "";
        fin >> username;
        fin.ignore();
        if (username == "")
            break;
        fin >> pass;
        fin.ignore();
        insertToBit(bitArray, username);
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
        string weakPass = "";
        fin >> weakPass;
        if (weakPass == "")
            break;
        insertToBit(bitArray, weakPass);
    }
    fin.close();
    return bitArray;
}

bool validUsername(string username, bool bitArr[])
{
    if (username.length() <= 5 || username.length() >= 10)
    {
        cout << "Username must longer than 5 and shorter than 10!\n";
        return false;
    }
    if (username.find(' ') != string::npos)
    {
        cout << "Username must not contain spaces!\n";
        return false;
    }
    if (isContain(bitArr, username))
    {
        cout << "Username must not be the same as any registered Username!\n";
        return false;
    }
    return true;
}

bool checkPass(string pass)
{
    bool hasUpperCase = false;
    bool hasLowerCase = false;
    bool hasDigit = false;
    bool hasSpecialChar = false;

    for (char c : pass)
    {
        if (isupper(c))
            hasUpperCase = true;
        if (islower(c))
            hasLowerCase = true;
        if (isdigit(c))
            hasDigit = true;
        if (ispunct(c))
            hasSpecialChar = true;
        if (hasUpperCase && hasLowerCase && hasDigit && hasSpecialChar)
            return true;
    }
    return false;
}

bool validPassword(string pass, string username, bool bitArr[])
{
    if (pass.length() <= 10 || pass.length() >= 20)
    {
        cout << "Password must longer than 10 and shorter than 20!\n";
        return false;
    }
    if (pass.find(' ') != string::npos)
    {
        cout << "Password must not contain spaces!\n";
        return false;
    }
    if (pass == username)
    {
        cout << "Password cannot be the same as username!\n";
        return false;
    }
    if (!checkPass(pass))
    {
        cout << "Password must include uppercase, lowercase, numbers and special characters!\n";
        return false;
    }
    if (isContain(bitArr, pass))
    {
        cout << "Your password is weak!\n";
        return false;
    }
    return true;
}

void process(bool usernameBit[], bool weakPassBit[]);

void backToMenu(bool usernameBit[], bool weakPassBit[])
{
    cout << "Press any key to back to menu!\n";
    char ch;
    ch = getch();
    return process(usernameBit, weakPassBit);
}

void store(string username, string pass)
{
    ofstream fout("UserData.txt", ios::app);
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
    bool flag = false;
    cout << "Registration\n";
    string username = "", pass = "";
    cout << "Username: ";
    getline(cin, username);
    cout << "Password: ";
    getline(cin, pass);
    if (validUsername(username, usernameBit))
    {
        if (validPassword(pass, username, weakPassBit))
        {
            insertToBit(usernameBit, username);
            store(username, pass);
            cout << "Register Successfully!\n";
        }
        else
        {
            if (type == 2)
            {
                storeFail(username, pass);
                flag = true;
            }
        }
    }
    else
    {
        if (type == 2 && !flag)
            storeFail(username, pass);
    }
    if (type == 1)
    {
        cout << "Press (R) to Register again or the others to back to menu!\n";
        char ch = getch();
        if (ch == 'R' || ch == 'r')
            registration(1, usernameBit, weakPassBit);
        else
            process(usernameBit, weakPassBit);
    }
}

void multipleRegistrations(bool usernameBit[], bool weakPassBit[])
{
    ifstream fin("SignUp.txt");
    if (!fin)
        return;
    int cnt = 0, dem = 0;
    while (!fin.eof())
    {
        dem++;
        string username = "", pass = "";
        getline(fin, username, ' ');
        getline(fin, pass, '\n');
        if (username == "")
            break;
        cout << "Regisstration " << dem << "\nUsername: " << username << endl
             << "Password: " << pass << endl;
        if (validUsername(username, usernameBit))
        {
            if (validPassword(pass, username, weakPassBit))
            {
                insertToBit(usernameBit, username);
                store(username, pass);
                cout << "Register Successfully!\n";
                cnt++;
            }
            else
                storeFail(username, pass);
        }
        else
            storeFail(username, pass);
    }
    fin.close();
    cout << cnt << " account were registed successfully!\n";
    backToMenu(usernameBit, weakPassBit);
}

void changePassword(string username, string pass, bool usernameBit[], bool weakPassBit[]);

void login(int type, bool usernameBit[], bool weakPassBit[])
{
    bool flag = false;
    cout << "Login\n";
    string username = "", pass = "";
    cout << "Username: ";
    getline(cin, username);
    cout << "Password: ";
    getline(cin, pass);
    if (!isContain(usernameBit, username))
    {
        flag = true;
        cout << "Username does not found!\n";
    }
    ifstream fin("UserData.txt");
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
    if (!flag)
        cout << "Wrong Password!\n";
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
    ifstream fin("UserData.txt");
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
            while (!validPassword(newPass, username, weakPassBit))
            {
                cout << "Please input valid password: ";
                getline(cin, newPass);
            }
            cout << "Your Password has been changed!\n";
            tmpPass = newPass;
        }
        account.push_back({tmpUsername, tmpPass});
    }
    fin.close();
    ofstream fout("UserData.txt");
    if (!fout)
    {
        cout << "Error: Can not open file!\n";
        return;
    }
    for (int i = 0; i < account.size(); i++)
        fout << account[i].first << ' ' << account[i].second << endl;
    fout.close();
    backToMenu(usernameBit, weakPassBit);
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


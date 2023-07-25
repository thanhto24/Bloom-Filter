#include <iostream>
#include <cstring>
#include <conio.h>
#include <fstream>
#include <vector>

using namespace std;

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

void registration(int type)
{
    cout << "Registration\n";
    string username = "", pass = "";
    cout << "Username: ";
    getline(cin, username);
    cout << "Password: ";
    getline(cin, pass);
    if (validUsername(username) && validPassword(pass))
        store(username, pass);
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
            registration(1);
        else
            process();
    }
}

void multipleRegistrations()
{
    cout << "Multiple registrations\n";
    cout << "Enter number of accounts you want to register: ";
    int num;
    cin >> num;
    cin.ignore();
    for (int i = 0; i < num; i++)
    {
        cout << "Account " << i + 1 << ' ';
        registration(2);
    }
    backToMenu();
}

void changePassword(string username, string pass);

void login(int type)
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
                            return changePassword(username, pass);
                        if (ch == '2')
                            return process();
                        cout << "Wrong input!\n";
                    }
                }
                else
                    return changePassword(username, pass);
                return;
            }
        }
        fin.close();
        cout << "Wrong username or password!\n";
    }
    cout << "Press (L) to Login again or the others to back to menu!\n";
    char ch = getch();
    if (ch == 'L' || ch == 'l')
        login(1);
    else
        process();
}

void changePassword(string username, string pass)
{
    if (username == "" || pass == "")
    {
        cout << "Please login first!\n";
        login(2);
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
            while (!validPassword(newPass))
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

void process()
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
                    registration(1);
                    break;
                case 1:
                    multipleRegistrations();
                    break;
                case 2:
                    login(1);
                    break;
                case 3:
                    changePassword("", "");
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
    process();
    return 0;
}
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <stdio.h>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
using namespace std;

struct firstFollowResult{
    map<char, set<char>> firstResult, followResult;
    vector<vector<char>> Productions;
    set<char> Terminals; 
    unordered_set<char> Variables;
};

void customDelimeter(string s, vector<char> &res, char delimeter)
{
    int en = 0;
    int len = s.length();
    while (en < len)
    {
        res.push_back(s[en]);
        en++;
        en++;
    }
}

void findFirst(char nonTerminal, vector<vector<char>> &productions, map<char, set<char>> &first)
{
    for (int i = 0; i < productions.size(); i++)
    {
        int j = 1;
        if (productions[i][0] == nonTerminal)
        {
            if (productions[i][1] >= 'A' && productions[i][1] <= 'Z')
            {
                findFirst(productions[i][1], productions, first);
                for(auto x : first[productions[i][j]]){
                    if(x == '#'){
                        j++;
                        if(islower(productions[i][j])){
                            first[nonTerminal].insert(productions[i][j]);
                        }
                        if(productions[i][j] != nonTerminal && isupper(productions[i][j])){
                            findFirst(productions[i][j], productions, first);
                            first[nonTerminal].insert(first[productions[i][j]].begin(), first[productions[i][j]].end());
                        }
                    }
                    else{
                        first[nonTerminal].insert(first[productions[i][1]].begin(), first[productions[i][1]].end());
                        if(j != productions[i].size() - 1){
                            first[nonTerminal].erase('#');
                        }
                        break;
                    }
                }
                // first[nonTerminal].insert(first[productions[i][1]].begin(), first[productions[i][1]].end());
            }
            else
            {
                first[nonTerminal].insert(productions[i][1]);
            }
        }
    }
}

void findFollow(char nonTerminal, vector<vector<char>> &productions, map<char, set<char>> &follow, map<char, set<char>> &first) {
    for (int i = 0; i < productions.size(); i++) 
    {
        for (int j = 1; j < productions[i].size(); j++) 
        {
            if (productions[i][j] == nonTerminal) 
            {
                if (j == productions[i].size() - 1) 
                {
                    if (productions[i][0] != nonTerminal) 
                    {
                        if(follow[productions[i][0]].empty())
                        {
                            findFollow(productions[i][0], productions, follow, first);
                        }
                        follow[nonTerminal].insert(follow[productions[i][0]].begin(), follow[productions[i][0]].end());
                    }
                } 
                else 
                {
                    int k = j + 1;
                    if(!islower(productions[i][k]))
                    {
                        set<char> firstSet = first[productions[i][k]];
                        while (firstSet.count('#') > 0 && k < productions[i].size() - 1) 
                        {
                            k++;
                            firstSet.erase('#');
                            if(!islower(productions[i][k]))
                            {
                                firstSet.insert(first[productions[i][k]].begin(), first[productions[i][k]].end());
                            }
                            else{
                                follow[nonTerminal].insert(productions[i][k]);
                            }
                        }
                        if (firstSet.count('#') > 0 && k == productions[i].size() - 1) 
                        {
                            firstSet.erase('#');
                            firstSet.insert(follow[productions[i][0]].begin(), follow[productions[i][0]].end());
                        }
                        if (firstSet.count('#') > 0 && productions[i][0] != nonTerminal) 
                        {
                            if(follow[productions[i][0]].empty())
                            {
                                findFollow(productions[i][0], productions, follow, first);
                            }
                            firstSet.insert(follow[productions[i][0]].begin(), follow[productions[i][0]].end());
                        }
                        follow[nonTerminal].insert(firstSet.begin(), firstSet.end());
                    }
                    else{
                        follow[nonTerminal].insert(productions[i][k]);
                    }
                }
            }
        }
    }
}

struct firstFollowResult firstAndFollow()
{
    vector<vector<char>> productions;
    set<char> terminals; 
    unordered_set<char> variables;
    
    fstream in;
    in.open("89_P6_grammar.txt", ios::in);

    if (!in)
    {
        cerr << "File not found" << endl;
    }

    string str;
    while (getline(in, str))
    {
        vector<char> temp;
        customDelimeter(str, temp, ' ');
        productions.push_back(temp);
    }

    // storing variables and terminals separately
    for (int i = 0; i < productions.size(); i++)
    {
        variables.insert(productions[i][0]);
        for (int j = 0; j < productions[i].size(); j++)
        {
            if(productions[i][j] >= 'a' && productions[i][j] <= 'z')
                terminals.insert(productions[i][j]);
        }
    }

    map<char, set<char>> first;
    for(int i = 0; i < productions.size(); i++)
    {
        findFirst(productions[i][0], productions, first);
    }

    // insert terminals' firsts
    for(auto t : terminals){
        first[t].insert(t);
    }

    //First of non-terminals
    for(auto x: first)
    {
        cout << "first("<< x.first << ") : ";
        for(auto y: x.second)
        {
            cout << y << " ";
        }
        cout << endl;
    }

    map<char, set<char>> follow;
    follow[productions[0][0]].insert('$');

    for(int i = 0; i < productions.size(); i++)
    {
        findFollow(productions[i][0], productions, follow, first);
    }

    for(auto x: follow){
        for(auto it: x.second){
            if(it == '#'){
                follow[x.first].erase(it);
                break;
            }
        }
    }

    //Follow of non-terminals
    cout << endl << endl;
    for(auto x: follow)
    {
        cout << "follow(" << x.first << ") : ";
        for(auto y: x.second)
        {
            cout << y << " ";
        }
        cout << endl;
    }

    struct firstFollowResult data;
    data.firstResult = first;
    data.followResult = follow;
    data.Productions = productions;
    data.Terminals = terminals;
    data.Variables = variables;
   
    return data;
}
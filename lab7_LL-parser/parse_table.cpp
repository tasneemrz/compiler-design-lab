#include "first_and_follow.cpp"
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <map>
#include <set>
using namespace std;

struct Result{
    map<char, map<char, set<int>>> pt;
    vector<vector<char>> prods;
};

void print_parse_table(map<char, map<char, set<int>>> parsingTable){
    cout << "\n********** PARSE TABLE **********\n" << endl;
    for(auto outer_pair : parsingTable){
        for(auto inner_pair : outer_pair.second){
            cout << "\t " << inner_pair.first;
        }
        break;
    }
    cout << endl << endl;
    for (auto outer_pair : parsingTable)
    {
        char var = outer_pair.first;
        cout << var << "\t";
        for (auto inner_pair : outer_pair.second)
        {
            char ter = inner_pair.first;
            for(auto val : inner_pair.second)
            {
                cout << val << ",";
            }
            cout << "\b " << "\t";
        }
        cout << endl << endl;
    }
}

void check_and_clear_neg_1(map<char, map<char, set<int>>>& parsingTable, char v, char t){
    for(auto x : parsingTable[v][t]){
        if(x == -1){
            parsingTable[v][t].clear();
        }
    }
}

void parse_table(map<char, map<char, set<int>>>& parsingTable, map<char, set<char>> first, map<char, set<char>> follow, vector<vector<char>> productions){
        
    // inserting values into parsing table
    for(int i = 0; i < productions.size(); i++){
        char v = productions[i][0];
        char z = productions[i][1];

        if(z == '#'){   // if # in production then do follow of variable
            for (auto t : follow[v])
            {
                check_and_clear_neg_1(parsingTable, v, t);
                parsingTable[v][t].insert(i);
            }
            continue;
        }

        for(auto t : first[z]){
            if(t != '#'){   
                check_and_clear_neg_1(parsingTable, v, t);
                parsingTable[v][t].insert(i);
            }
            else{   // if # in first of a variable then iterate through follow of the variable
                for (auto t : follow[z])
                {
                    check_and_clear_neg_1(parsingTable, v, t);
                    parsingTable[v][t].insert(i);
                }
            }
        }
    }
}

struct Result ParseTable(){
    struct firstFollowResult res = firstAndFollow();
    map<char, set<char>> first = res.firstResult, follow = res.followResult;
    vector<vector<char>> productions = res.Productions;
    set<char> terminals = res.Terminals; 
    unordered_set<char> variables = res.Variables;
    map<char, map<char, set<int>>> parsingTable;
    
    // terminals set does not have $ so insert it
    terminals.insert('$');

    // inserting -1 to all values to form the table
    for (auto v : variables){
        for (auto t : terminals)
            parsingTable[v][t].insert(-1);
    }
    
    parse_table(parsingTable, first, follow, productions);
    // print_parse_table(parsingTable);

    struct Result r;
    r.pt = parsingTable;
    r.prods = productions;

    return r;
}
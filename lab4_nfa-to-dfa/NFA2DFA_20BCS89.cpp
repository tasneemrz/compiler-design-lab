#include <bits/stdc++.h>
using namespace std;

vector<int> toBinary(int n)
{
    vector<int> num;
    while (n > 0)
    {
        num.push_back(n % 2);
        n = n / 2;
    }
    return num;
}

int main()
{
    int initial_state;
    unordered_map<int, int> final_states;

    int nfa[50][50][50]; // state,input,transitions
    memset(nfa, -2, 50 * 50 * 50 * sizeof(nfa[0][0][0]));
    ifstream f;

    f.open("nfa.txt", ios::in);

    if (f.is_open())
    {
        string str;
        int r = 0, c, t; // c->input symbols , t->transitions
        f >> initial_state;
        getline(f, str);
        getline(f, str);

        for (int i = 0; i < str.size(); i++)
        {
            if (str[i] == ' ')
                continue;
            final_states[str[i] - '0'] = 2;
        }

        while (getline(f, str))
        {
            c = 0, t = 0;
            for (int i = 0; i < str.size();)
            {
                if (str[i] == '-')
                {
                    nfa[r][c][t] = -1;
                    i += 2;
                    continue;
                }
                if (str[i] == ' ')
                {
                    i++;
                    t = 0;
                    c++;
                    continue;
                }
                if (str[i] == ',')
                {
                    t++;
                    i++;
                    continue;
                }

                nfa[r][c][t] = str[i++] - '0';
            }
            r++;
        }

        queue<int> process_q; // keeps track of states that are to be processed
        map<int, int> already; // already map keeps track of the states that are already processed

        already[pow(2, initial_state)] = 1; // adds binary representation of initial state
        process_q.push(pow(2, initial_state)); // adds binary form of initial state to process_q

        int dfa[100][100];
        set<int> finalstate;

        while (process_q.size() > 0)
        {
            int curr_state = process_q.front();
            process_q.pop();
            vector<int> check;
            vector<int> bin = toBinary(curr_state);

            for (int i = 0; i < bin.size(); i++)
            {
                if (bin[i] == 1)
                    check.push_back(i);  // to identify which states in the NFA are included in the current DFA state being processed.
            }

            for (int i = 0; i <= c; i++)  // iterates through all possible input symbols
            {
                int trans = 0;
                map<int, int> arr;
                for (int w = 0; w < check.size(); w++) // iterating through all states in nfa present in check
                {
                    if (final_states[check[w]] == 2)
                    {
                        finalstate.insert(curr_state);
                    }
                    for (int k = 0; nfa[check[w]][i][k] != -16843010 && nfa[check[w]][i][k] != -1; k++)
                    {
                        if (arr[nfa[check[w]][i][k]] == 0)
                        {
                            trans += pow(2, nfa[check[w]][i][k]);
                            arr[nfa[check[w]][i][k]] = 1;
                        }
                    }
                }
                if (trans != 0 && already[trans] == 0)
                {
                    process_q.push(trans);
                    already[trans] = 1;
                }
                dfa[curr_state][i] = (trans == 0) ? -1 : trans;
            }
        }

        map<int, int> refine;
        int q = 0;
        for (int i = 1; i < pow(2, r); i++)
        {
            if (already[i] == 1)
            {

                refine[i] = q++;
            }
        }
        refine[-1] = -1;

        cout << "\n----------DFA----------" << endl;

        cout << "Initial state: ";
        cout << initial_state << '\n';

        cout << "Final states: ";
        for (auto it = finalstate.begin(); it != finalstate.end(); it++)
            cout << refine[*it] << " ";
        cout << '\n' << endl;

        for (int i = 0; i < pow(2, r); i++)
        {
            if (already[i] == 1)
            {
                for (int j = 0; j <= c; j++)
                {
                    cout << refine[dfa[i][j]] << '\t';
                }
                cout << "\n";
            }
        }

    }
    else
        cout << "Not Open" << endl;

    return 0;
}
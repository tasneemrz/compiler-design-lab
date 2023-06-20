#include<iostream>
#include <algorithm>
#include<fstream>
#include<vector>
#include<string>
using namespace std;

void display(vector<int> final_states, int current_state){
    vector<int>::iterator it;
    it = find(final_states.begin(), final_states.end(), current_state);
    cout << "final state : q" << current_state << endl;
    if(it != final_states.end()){
        cout << "accepted" << endl;
    }else{
        cout << "not accepted" << endl;
    }
}

int main()
{
    fstream dfaFile;
    dfaFile.open("20bcs089_dfa.txt", ios::in);

    string initial_state_line, final_states_line, transition_line, num, input;
    vector<vector<int>> transition_table;
    vector<int> final_states;
    int initial_state, string_length, c;

    if(dfaFile.is_open()){
        getline(dfaFile, initial_state_line, '\n');
        initial_state = stoi(initial_state_line);

        getline(dfaFile, final_states_line, '\n');
        for(int i = 0; i < final_states_line.length()-1; i++){
            if(final_states_line[i] != ' '){
                num = final_states_line[i];
                final_states.push_back(stoi(num));
            }
        }

        while(getline(dfaFile, transition_line, '\n')){
            vector<int> transition;
            for(int i = 0; i < transition_line.length(); i++){
                if(transition_line[i] != ' ' && transition_line[i] != 13){
                    if(transition_line[i] == '-'){
                        num = transition_line[i];
                        i++;
                        num = num + transition_line[i];
                        transition.push_back(stoi(num));
                    }
                    else{
                        num = transition_line[i];
                        transition.push_back(stoi(num));
                    }
                }
            }
            transition_table.push_back(transition);
        }
        dfaFile.close();
    }else{
        cout << "unable to open file" << endl;
    }

    while(true){
        cout << "\nEnter the input string : ";
        cin >> input;
        int current_state = initial_state;

        string_length = input.length();
        if(string_length == 1 && input[0] == 'e'){
            c = 1;
        }else{
            c = 2;
        }

        switch (c)
        {
        case 1:
            display(final_states, initial_state);
            break;
        
        case 2:
            for (int i = 0; i < input.length(); i++)
            {
                cout << "q" << current_state << " -> ";
                if(input[i] == '0'){
                    if(transition_table[current_state][0] != -1)
                        current_state = transition_table[current_state][0];
                }
                if(input[i] == '1'){
                    if(transition_table[current_state][1] != -1){
                        current_state = transition_table[current_state][1];
                    }
                }
            }
            cout << "q" << current_state << endl;
            display(final_states, current_state);
            break;
        
        default:
            cout << "length of string not accepted" << endl;
            break;
        }
    }
    return 0;
}
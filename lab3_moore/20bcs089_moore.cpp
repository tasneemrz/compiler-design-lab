#include<iostream>
#include<algorithm>
#include<fstream>
#include<vector>
#include<string>
using namespace std;

int getIndex(vector<char> v, char k)
{
	int index = 0;
    auto it = find(v.begin(), v.end(), k);
    if (it != v.end()) 
    {
        index = it - v.begin();
    }
	return index;
}

int main()
{
	fstream mooreFile;
	mooreFile.open("20bcs089_moore.txt", ios::in);

	string initial_state_line, line, num, input_string;
	int initial_state, current_state;
	vector<vector<int>> transition_table;

	if(mooreFile.is_open()){
		getline(mooreFile, initial_state_line);
		initial_state = stoi(initial_state_line);

		while(getline(mooreFile, line)){
			vector<int> row;
			for (int i = 0; i < line.length(); i++)
			{
				if(line[i] != ' ' && int(line[i]) != 13)
				{
                    num = line[i];
					if(line[i] == '-'){
                        i++;
                        num = num + line[i];
                        row.push_back(stoi(num));
                    }
					else if(char(line[i]) >= 'A' && char(line[i]) <= 'Z'){
                    	row.push_back(int(line[i])); 
                    }
					else{
                    	row.push_back(stoi(num)); 
                    }
				}
			}
			transition_table.push_back(row);
		}
		mooreFile.close();
	}else{
		cout << "unable to open file" << endl;
	}

	while(true){
		int c, string_length, output_var_index;
		string output_string;
		vector<int> states_traversed;
		vector<char> terminals;

		current_state = initial_state;
		states_traversed.push_back(initial_state);
		output_var_index = transition_table[0].size() - 1;
		output_string += char(transition_table[current_state][output_var_index]);

		cout << "\nEnter input string : ";
		getline(cin, input_string);
		string_length = input_string.length();

		for (int i = 0; i < string_length; i++){
			int j;
			for (j = 0; j < i; j++){
				if (input_string[i] == input_string[j])
               		break;
			}
			if(i == j){
				terminals.push_back(input_string[i]);
			}
		}
		sort(terminals.begin(), terminals.end());

		if(string_length == 1 && input_string == " " || string_length == 0) c = 1;
		else c = 2;

		switch (c)
		{
		case 1:
			cout << "q" << current_state << endl;
			cout << "output : " << char(transition_table[0][output_var_index]) << endl;
			break;

		case 2:
			for (int i = 0; i < string_length; i++){
				int j = getIndex(terminals, input_string[i]);
				if(transition_table[current_state][j] == -1){
					break;
				}
				current_state = transition_table[current_state][j];
				states_traversed.push_back(current_state);
				output_string += char(transition_table[current_state][output_var_index]);
			}

			cout << "q" << states_traversed[0];
			for (int i = 1; i < states_traversed.size(); i++){
				cout << " -> " << "q" << states_traversed[i];
			}
			cout << "\noutput string : " << output_string << endl;
			break;

		default:
			cout << "string not accepted" << endl;
			break;
		}
	}
	return 0;
}
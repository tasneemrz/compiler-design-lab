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
        index =  distance(v.begin(), it);
    }
	return index;
}

int main()
{
	fstream mealyFile;
	mealyFile.open("20bcs089_mealy.txt", ios::in);

	string initial_state_line, line, num, input_string;
	int initial_state, current_state;
	vector<vector<int>> transition_table;

	if(mealyFile.is_open()){
		getline(mealyFile, initial_state_line);
		initial_state = stoi(initial_state_line);

		while(getline(mealyFile, line)){
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
		mealyFile.close();
	}else{
		cout << "unable to open file" << endl;
	}

	while(true){
		int c, string_length;
		string output_string, sorted_str;
		vector<int> states_traversed;
		vector<char> terminals;

		cout << "\nEnter input string : ";
		getline(cin, input_string);
		string_length = input_string.length();

		sorted_str = input_string;
		sort(sorted_str.begin(), sorted_str.end());
		for (int i = 0; i < sorted_str.length(); i++){
			int j;
			for (j = 0; j < i; j++){
				if (sorted_str[i] == sorted_str[j])
               		break;
			}
			if(i == j){
				terminals.push_back(sorted_str[i]);
				terminals.push_back('/');
			}
		}

		current_state = initial_state;
		states_traversed.push_back(initial_state);

		if(string_length == 1 && input_string == " " || string_length == 0) c = 1;
		else c = 2;

		switch (c)
		{
		case 1:
			cout << "epsilon not acceptable" << endl;
			break;

		case 2:
			for (int i = 0; i < string_length; i++)
			{
				int j = getIndex(terminals, input_string[i]);
				if(transition_table[current_state][j] == -1){
					break;
				}
				output_string += char(transition_table[current_state][j + 1]);
				current_state = transition_table[current_state][j];
				states_traversed.push_back(current_state);
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
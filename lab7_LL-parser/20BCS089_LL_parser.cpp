#include <iostream>
#include <stack>
#include <set>
#include "parse_table.cpp"
using namespace std;

int main()
{
	struct Result res = ParseTable();
	map<char, map<char, set<int>>> parse_table = res.pt;
	vector<vector<char>> productions = res.prods;
	stack<char> Stack;
	stack<char> input;
	string str;
	char start_sym = productions[0][0];

	print_parse_table(parse_table);

	while(true){
		cout << "\nEnter string : ";
		cin >> str;

		// push start symbol on to stack
		Stack.push('$');
		Stack.push(start_sym);

		// push the input string onto input stack
		input.push('$');
		for (int i = str.length() - 1; i >= 0; i--)
		{
			input.push(str[i]);
		}

		while (input.top())
		{
			// if both stack tops match
			if (Stack.top() == input.top() && input.top() != '$') 
			{
				input.pop();
				Stack.pop();
			}

			// both stack and input stack become empty
			else if (Stack.top() == '$' && input.top() == '$')
			{
				cout << "String is accepted" << endl;
				input.pop();
				Stack.pop();
				break;
			}

			// stack becomes empty but input stack is not empty
			else if (Stack.top() == '$' && input.top() != '$')
			{
				cout << "String not accepted" << endl;
				break;
			}

			// if they top of both stacks do not match
			else if (Stack.top() != input.top()) 
			{
				// get the production
				set<int> value = parse_table[Stack.top()][input.top()];
				auto prod_num = value.begin();

				if (*prod_num == -1){
					cout << "error" << endl;
					break;
				}

				// pop the symbol at the top and replace with its production symbols
				Stack.pop();
				for (int j = productions[*prod_num].size() - 1; j >= 1; j--)
				{
					if (productions[*prod_num][j] != '#')
					{
						Stack.push(productions[*prod_num][j]);
					}
				}
			}
		}
	}

	return 0;
}
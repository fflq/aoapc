/*
 * uva 11988 - Broken Keyboard
 * flq - Tue Apr 24 11:39:21 CST 2018
 * note:
 * *1. vecotr will "Time limit", list is ok
 */
#include <iostream>
//#include <vector>
#include <list>

using namespace std ;

enum flag_e
{
	BEGIN = 1, 
	END,
} ;


//vector<char> vec ;
list<char> vec ;
int main()
{
#ifdef LOCAL
	freopen ("in", "r", stdin) ;
#endif 

	string line ;
	int pos, flag, if_first_begin = 0 ;
	//vector<char>::iterator it_in ;
	list<char>::iterator it_in ;
	while (cin >> line)
	{
		flag = END ;
		for (auto ch : line)
		{
			if (ch == '[')	
			{
				flag = BEGIN ;
				it_in = vec.begin() ;
			}
			else if (ch == ']')	flag = END ;
			else	
			{
				// avoid erro perator for other conditions
				if (vec.empty())	vec.push_back(ch) ;
				// insert end
				else if (END == flag)	vec.push_back(ch) ;
				// insert before
				else	
				{
					it_in = vec.insert(it_in, ch) ;
					++ it_in ;
				}
			}
		}

		for (auto it : vec)	cout << it ; cout << endl ;

		vec.clear () ;
	}

	return 0 ;
}

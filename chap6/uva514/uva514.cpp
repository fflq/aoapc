/*
 * uva 514 - Rails 
 * flq - Sat Apr 21 17:01:28 CST 2018 
 */

#include <iostream>
#include <queue>
#include <stack>
using namespace std ;

// handle one block data
void handle(int n, queue<int> &want_queue)
{		
	int number, index = 1 ;
	stack<int> station_stack ;

	while (!want_queue.empty())
	{
		// want_queue is B
		number = want_queue.front() ;
		want_queue.pop () ;

		// index is A
		while (index < number)	station_stack.push(index ++) ;

		// A hasnot, see stack
		if (index != number)	// find from stack
		{
			// the top of station_stack is what B wanted
			if (number == station_stack.top())	station_stack.pop() ;
			// A hasnot, and stack's top isnot what B wanted, means no reaonable
			else	
			{
				cout << "No" << endl ;
				return ;
			}
		}
		// A has it, A->B, no need to stack
		else	++ index ;
	}

	cout << "Yes" << endl ;
}

int main()
{
#ifdef LOCAL
	freopen ("in", "r", stdin) ;
#endif 

	int n ;
	while (true)
	{
		cin >> n ;
		if (0 == n)	break ;

		int number ;
		while (true)
		{
			queue<int> want_queue ;
			for (int i = 0; i < n; ++ i)
			{
				cin >> number ;
				if (0 == number)	goto next_block ;

				want_queue.push (number) ;
			}
			
			handle (n, want_queue) ;
		}

next_block :
		cout << endl ;
	}

	return 0 ;
}

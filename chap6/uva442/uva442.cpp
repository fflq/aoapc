/*
 * uva 442 - Matrix Chain Multiplication
 * flq - Mon Apr 23 14:55:11 CST 2018
 */
#include <iostream>
#include <queue>
#include <stack>

using namespace std ;

struct matrix_t
{
	int nrows ;
	int ncolumns ;
} ;

char g_name = 'a' ;

char matrix_multiply(matrix_t *&mats, const char name1, const char name2, int &score)
{
	if (mats[name1].ncolumns != mats[name2].nrows)	return -1 ;

	score += (mats[name1].nrows) * (mats[name1].ncolumns) * (mats[name2].ncolumns) ;
	mats[g_name].nrows = mats[name1].nrows ; 
	mats[g_name].ncolumns = mats[name2].ncolumns ; 

	return g_name++ ;
}

// the capital/bingger letter means source
// the lower letter means name after multiple, for temporay
void handle_ebnf(string &line, matrix_t *mats)
{
	stack<char> expr_stack ;
	char name, name1, name2 ;
	int score = 0 ;

	if (1 == line.length())	score = 0 ;

	for (auto c : line)
	{
		if (')' != c)	expr_stack.push(c) ;
		else	// c == ')'
		{
			// the order for name1 and name2 is due to stack's FIFO
			name2 = expr_stack.top() ;
			expr_stack.pop () ;
			name1 = expr_stack.top() ;
			expr_stack.pop () ;

			if (-1 == (name = matrix_multiply(mats, name1, name2, score)))
			{
				cout << "error" << endl ;
				return ;
			}
			expr_stack.pop () ;	// pop '('

			expr_stack.push (name) ;

		}
	}

end :
	cout << score <<  endl ;
}

int main()
{
#ifdef LOCAL
	freopen ("in", "r", stdin) ;
#endif 

	matrix_t mats[256] ;
	string line ;
	int matrix_nums ;
	char name ;
	while (cin >> matrix_nums)
	{
		while (matrix_nums --)
		{
			cin >> name ; 
			cin >> mats[name].nrows >> mats[name].ncolumns ;

#ifdef LOCAL
#endif
		}
		
		g_name = 'a' ;	// for multiply temporty name
		while (cin >> line)	handle_ebnf(line, mats) ;
	}

	return 0 ;
}

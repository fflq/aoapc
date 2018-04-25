/*
* title: uva 679 - 
* author: flq[flqnerve@163.com] - Wed Apr 25 15:21:59 CST 2018
* note:
*  *1. 模拟n球路径会超时，这里参考书上的面运算规律：
*	 ball_nums奇数的都是从root左边走L的，
*	 同理把L看作root，则从L走的ball_nums/2个中的奇数都是从L的左边走的
*	 循环直至depth-1次(因为进入root那步不分左右)
*/

#include <iostream>

using namespace std ;

int main()
{
#ifdef LOCAL
	freopen ("in.uva", "r", stdin) ;
#endif

	int num, depth, ball_nums, id ;
	while (cin >> num && (-1 != num))
	{
		while (num --)
		{
			cin >> depth >> ball_nums ;

			id = 1 ;
			// ball_nums奇数的都是从root左边走L的，
			// 同理把L看作root，则从L走的ball_nums/2个中的奇数都是从L的左边走的
			// 循环直至depth次
			-- depth ;	// 因为第一层root不分Lroot，Rroot
			while (depth --)
			{
				if (ball_nums%2)	id*=2, ball_nums=(ball_nums+1)/2 ; 	// left
				else	id=2*id+1, ball_nums/=2 ;
			}
			cout << id << endl ;
		}
	}

	return 0 ;
}


/*
* title: uva 122 - Trees on the level  
* author: flq[flqnerve@163.com] - Thu Apr 26 14:10:36 CST 2018
* note:
*  *1. 不能用数组存完全树，题目说最多256个节点，不是指完全树
*  若256是个单链，数组就需要2^256-1个节点
*  *2. 用链表
*	**1. 先排序s，然后建树，建树过程中可检查路径点是否存在或重复建
*	**2. 层次输出树
*/

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std ;

typedef struct tree
{
	struct tree *lchild, *rchild ;
	int v ;
	tree(int v):v(v),lchild(NULL),rchild(NULL) { }
	tree():v(0),lchild(NULL),rchild(NULL) { }
} tree_t, *p_tree_t ;

inline void get_n_s(string &str, int &n, string &s)
{
	size_t pos1 = str.find_first_of(',') ;
	size_t pos2 = str.find_first_of(')') ;

	string n_str = str.substr(1, pos1-1) ;
	n = atoi(n_str.c_str()) ;

	s = str.substr(pos1+1, pos2-pos1-1) ;
}

// 按s的长度排序
bool sort_func(pair<int, string> a, pair<int, string> b)
{
	return ((a.second).length() < (b.second).length()) ;
}

// 遍历树，附加，输出节点和删除释放
void traverse_tree(p_tree_t &proot, int if_output=0, int if_delete=1)
{
	if (!proot)	return ;

	p_tree_t p ;
	queue<p_tree_t> qe ;
	qe.push (proot) ;

	while (!qe.empty())
	{
		p = qe.front() ; qe.pop () ;

		if (p->lchild)	qe.push(p->lchild) ;
		if (p->rchild)	qe.push(p->rchild) ;

		if (1 == if_output)	cout << p->v ;
		if ((1 == if_output) && !qe.empty())	cout << " " ;

		if (if_delete)	delete p ;
	}
	if (1 == if_output)	cout << endl ;

	// error
	// if (if_delete)	delete proot ;
}


int tree_add(p_tree_t *pproot, int n, string &s)
{
	// for new 
	p_tree_t *pnode = pproot ;

	// error1: 路径节点不存在
	for (char ch : s)
	{
		if (NULL == *pnode)	return -1 ;
		pnode = ('L' == ch) ? &((*pnode)->lchild) : &((*pnode)->rchild) ;
	}

	// error2: 节点已经存在过了
	if (*pnode)	return -2 ;
	*pnode = new tree_t(n) ;

	return 1 ;
}

int main()
{
#ifdef LOCAL
	freopen ("in.uva", "r", stdin) ;
#endif

	int n ;
	string line, s ;
	const string notcomp = "not complete" ;
	vector<pair<int, string>> nodes ;
	p_tree_t proot = NULL ;

	while (cin >> line)
	{
		// 一组数据输入结束
		if ("()" == line)	
		{
			// 排序后自上而下的建树，否则乱序不好建树
			sort (nodes.begin(), nodes.end(), sort_func) ;
			//for (auto pa : nodes)	cout << pa.first << " - " << pa.second << endl ;
			for (auto pa : nodes)	
				if (0 > tree_add(&proot, pa.first, pa.second))
				{
					cout << notcomp << endl ;
					traverse_tree (proot) ;	// free
					goto next_test ;
				}

			traverse_tree (proot, 1) ;	// output and free
next_test :
			nodes.clear () ;
			proot = NULL ;
			continue ;
		}

		get_n_s (line, n, s) ;
		nodes.push_back (make_pair(n, s)) ;
	}

	return 0 ;
}


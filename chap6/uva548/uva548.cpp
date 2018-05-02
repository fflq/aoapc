/*
* title: uva 548 - Tree 
* author: flq[flqnerve@163.com] - Wed May  2 11:22:14 CST 2018
* note:
*/

#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std ;

inline void set_vec(const string &line, vector<int> &vec)
{
	static stringstream ss ;
	static int num ;

	ss.str (line) ;
	while (ss >> num)	vec.push_back(num) ;
	ss.clear () ;
}

typedef struct tree
{
	struct tree *lchild, *rchild ;
	int v ;
	tree(int v):v(v),lchild(NULL),rchild(NULL) {}
} tree_t, *p_tree_t ;

typedef vector<int>	vint ; 

// 根据中序和后序，为当前root节点创建子树
void create_btree(p_tree_t *pproot, vint &in_vec, vint &post_vec)
{
	if (in_vec.empty())	return ;

	vint::iterator in_beg = in_vec.begin() ;
	vint::iterator in_ed = in_vec.end() ;

	// 后序的最后一个节点，就是中序的根节点
	int root_node = post_vec.back() ;
	post_vec.pop_back () ;
	vint::iterator root_iter = find(in_beg, in_ed, root_node) ;

	// 根节点左子孙数
	int left_nums = root_iter - in_vec.begin() ;
	*pproot = new tree_t(root_node) ;
	//cout << root_node << endl ;

	vint::iterator post_beg = post_vec.begin() ;
	vint::iterator post_ed = post_vec.end() ;

	vint new_in_vec, new_post_vec ;
	// 创建当前root的左子树
	
	new_in_vec = vint(in_beg, root_iter) ;
	new_post_vec = vint(post_beg, post_beg+left_nums) ;
	create_btree (&((*pproot)->lchild), new_in_vec, new_post_vec) ; 
	// 创建当前root的右子树
	new_in_vec = vint(root_iter+1, in_ed) ;
	new_post_vec = vint(post_beg+left_nums, post_ed) ;
	create_btree (&((*pproot)->rchild), new_in_vec, new_post_vec) ;
}

void bfs(p_tree_t proot)
{
	queue<p_tree_t> qe ;
	p_tree_t p ;

	if (proot)	qe.push(proot) ;

	while (!qe.empty())
	{
		p = qe.front() ; qe.pop () ;
		cout << p->v << " " ;
		if (p->lchild)	qe.push(p->lchild) ;
		if (p->rchild)	qe.push(p->rchild) ;
	}
	cout << endl ;
}

// 统计每个节点所有v(子孙+自己)和的最小值
/*
int bfs_min_sum(p_tree_t proot)
{
	if (!proot)	return 0 ;
	return proot->v + min(bfs_min_sum(proot->lchild), bfs_min_sum(proot->rchild)) ;
}
*/

typedef pair<int,int> pii ;
// 统计每个节点所有v(子孙+自己)和的最小值
// 并且记录最小值路径的叶子节点值，路径最小值相同则取叶节点最小值
// pair<leaf_node, path_sum>
pii *bfs_min_sum(p_tree_t proot)
{
	// 空节点，返回最大值，因为比较是比小，所以该节点会过滤掉
	if (!proot)	return new pii(INT_MAX, INT_MAX) ;

	// leaf
	if (!(proot->lchild) && !(proot->rchild))	return new pii(proot->v, proot->v) ;

	//return proot->v + min(bfs_min_sum(proot->lchild), bfs_min_sum(proot->rchild)) ;
	pii *lpii = bfs_min_sum(proot->lchild) ;
	pii *rpii = bfs_min_sum(proot->rchild) ;
	pii *ppii ;
	if (lpii->second != rpii->second)	ppii = (lpii->second < rpii->second) ? lpii : rpii ;	
	// 路径最小值相同时，取叶节点最小的
	else	ppii = (lpii->first < rpii->first) ? lpii : rpii ;

	ppii->second += proot->v ;

	return ppii ;
}

int main()
{
#ifdef LOCAL
	freopen ("in.uva", "r", stdin) ;
#endif

	string in_line, post_line ;
	vector<int> in_vec, post_vec ;
	p_tree_t proot ;
	while (getline(cin, in_line) && getline(cin, post_line))
	{
		in_vec.clear () ;
		post_vec.clear () ;

		//cout << in_line << " - " << post_line << endl ;

		set_vec (in_line, in_vec) ;
		set_vec (post_line, post_vec) ;

		create_btree (&proot, in_vec, post_vec) ;	
		//bfs (proot) ;

		pii *p = bfs_min_sum(proot) ;
		cout << p->first << endl ;
		delete p ;
	}

	return 0 ;
}


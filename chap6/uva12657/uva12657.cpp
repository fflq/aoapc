/*
 * uva 12657 - Boxes in a Line
 * flq - Tue Apr 24 12:35:50 CST 2018
 * note:
 * *1. stl list will "Time limit", write list by hand ;
 * *2. 不管怎么操作，gp_iters[1->n]都是按顺序的第1->n个盒子，避免了遍历链表查找
 * *3. 手写的双向链表也超时，后来经提示发现瓶颈在链表倒转上，根据如下提示修改：
 *	**1."如果数据结构上的某一个操作很耗时，有时可以用加标记的方式处理，而不
 *		需要真的执行那个操作。 但同时，该数据结构的所有其他操作都要考虑这个标记"
 *	**2."操作4比较特殊，为了避免一次修改所有元素的指针，此处增加一个标记
 *		inv，表示有没有执行过操作4（如果inv=1时再执行一次操作4，则inv变为0）。
 *		这样，当op为1和2且inv=1时，只需把op变成3-op（注意操作3不受inv影响）即可。 
 *		最终输出时要根据inv的值进行不同处理"
 */

#include <iostream>
#include <list>

using namespace std ;

const int MAX_ARR_NUMS = 100005 ;

typedef struct list
{
	struct list *next, *prev ;
	int v ;
} list_t, *p_list_t ;

list_t g_list[MAX_ARR_NUMS] ;
// when left/right_insert etc, remain each list->v(1-n) in gp_iters(1-n)
// 不管怎么操作，gp_iters[1->n]都是按顺序的第1->n个盒子，避免了遍历链表查找
p_list_t gp_iters[MAX_ARR_NUMS] = { NULL } ;
int g_if_rev = 0 ;	// if list reverse

// insert x to the left of y
inline void left_insert(p_list_t x, p_list_t y)
{
	x->prev->next = x->next ;
	x->next->prev = x->prev ;

	x->prev = y->prev ;
	x->next = y ;

	y->prev->next = x ;
	y->prev = x ;
}

// insert x to the right of y
inline void right_insert(p_list_t x, p_list_t y)
{
	x->prev->next = x->next ;
	x->next->prev = x->prev ;

	x->next = y->next ;
	x->prev = y ;

	y->next->prev = x ;
	y->next = x ;
}

// reverse list
inline void list_reverse(p_list_t p_li, const int n)
{
	p_list_t p_it ;
	for (int i = 0; i <= n; ++ i)
	{
		p_it = p_li[i].next ; 
		p_li[i].next = p_li[i].prev ; 
		p_li[i].prev = p_it ;
	}
}

void handle_list(int opcode, const int op1, const int op2, const int n)
{
	static p_list_t it1, it2 ;

	if ((3 > opcode) && (1 == g_if_rev))	opcode = 3 - opcode ;
	if (4 != opcode) (it1 = gp_iters[op1], it2 = gp_iters[op2]) ;

	switch (opcode)
	{
		// x -> left of y
		case 1 :	left_insert (it1, it2) ; break ;
		// x -> right of y
		case 2 :	right_insert (it1, it2) ; break ;
		case 3 :
		{
			int temp = it1->v ;
			it1->v = it2->v ;
			it2->v = temp ;

			p_list_t p_it = gp_iters[op1] ;
			gp_iters[op1] = gp_iters[op2] ;
			gp_iters[op2] = p_it ;
			break ;
		}
		case 4 : g_if_rev = (0 == g_if_rev) ? 1 : 0 ; break ;
	}
}

int main()
{
#ifdef LOCAL
	freopen ("in.uva", "r", stdin) ;
#endif

	int n, m, k, opcode, op1, op2, case_num=1 ;
	long score ; string line ; p_list_t p ;

	while (cin >> n >> m)
	{
		g_if_rev = score = 0 ;

		// init must, or the init order will not 1->n
		g_list[0].v = 0 ;
		g_list[0].next = &g_list[1] ;
		// create de-list
		for (int i = 1; i <= n; ++ i)	
		{
			g_list[i].v = i ;
			g_list[i].prev = &g_list[i-1] ;
			g_list[i].next = &g_list[i+1] ;
			gp_iters[i] = &g_list[i] ;
		}
		g_list[0].prev = &g_list[n] ;
		g_list[n].next = &g_list[0] ;
		

		while (m --)
		{
			cin >> opcode ;
			if (4 != opcode)	cin >> op1 >> op2 ;

			handle_list (opcode, op1, op2, n) ;

#ifdef LOCAL
		//for ( (p=g_list[0].next, k=1) ; k <= n; (++k, p=p->next) ) cout << p->v << " " ;
		//cout << endl ;
#endif
		}

		//for ( (p=g_if_rev?g_list[0].prev:g_list[0].next, k=1) ; k <= n; 
		//		(++k, p=g_if_rev?p->prev:p->next) )
		//	if (1 == k%2)	score += p->v ;
		for ( (p = g_if_rev ? g_list[0].prev : g_list[0].next, k = 1) ; 
				(k <= n) && (score += (1 == k%2 ? p->v : 0)) ;
				(++k, p = g_if_rev ? p->prev : p->next) )	;

		cout << "Case " << case_num++ << ": " << score << endl ;

	}

	return 0 ;
}

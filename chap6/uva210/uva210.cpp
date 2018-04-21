/*
 * uva 210
 * note:
 * *1. 指令原子性,正在执行的指令,不管配额quannum是否超时,都会执行完当前指令再切换
 *
 */

#include <iostream>
#include <sstream>
#include <string>
#include <queue>
#include <deque>
#include <map>
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std ;

map<string, int> g_stms_dict ;
map<string, int> g_var_dict ;

typedef struct step
{
	string line ;
	int remain ;
	int remain_bak ;
} step_t, *p_step_t ;
queue<p_step_t> **gpp_prog_queues ;
queue<pair<int, string>*> g_order_queue ;

string& trim(string &s)   
{  
	if (s.empty())   return s;  

	s.erase(0,s.find_first_not_of(" "));  
	s.erase(s.find_last_not_of(" ") + 1);  

	return s;  
}  

void get_input(int &prog_nums, int &quantnum, int &step_nums)
{
	string line ;
	while (getline(cin, line) && line.empty()) ;
	istringstream is(line) ;

	is >> prog_nums ;

	const char stms_strs[][16] = { "=", "print", "lock", "unlock", "end" } ;
	size_t stms_size = sizeof(stms_strs)/sizeof(stms_strs[0]) ;
	for (int i = 0; i < stms_size; ++ i)
		is >> g_stms_dict[stms_strs[i]] ;

	is >> quantnum ;


	p_step_t p_step ;
	step_nums = 0 ;
	gpp_prog_queues = new queue<p_step_t>*[prog_nums+1] ;
	for (int i = 1; i <= prog_nums; ++ i)
	{
		gpp_prog_queues[i] = new queue<p_step_t>() ;
		while (getline(cin, line))
		{
			++ step_nums ;

			p_step = new step_t() ;
			p_step->line = trim(line) ;
			// check "unlock" before "lock"
			// for (int k = 0; k < stms_size; ++ k)
			for (int k = stms_size-1; k >= 0; -- k)
				if (string::npos != line.find(stms_strs[k]))
				{
					p_step->remain = g_stms_dict[stms_strs[k]] ;
					p_step->remain_bak = g_stms_dict[stms_strs[k]] ;
					break ;
				}
			gpp_prog_queues[i]->push (p_step) ;

			if ("end" == line)	break ;
		}
	}
}

void test_input(int prog_nums)
{
	for (int i = 1; i <= prog_nums; ++ i)
	{
		cout << endl << i << endl ;
		queue<p_step_t> *p = gpp_prog_queues[i] ;
		while (!(p->empty()))
		{
			p_step_t p_step = p->front() ;
			cout << "* " << p_step->line << " - " << p_step->remain << endl ;
			p->pop () ;
		}
	}
}


enum stms_type 
{
	TYPE_LOCK = 10, 
	TYPE_UNLOCK,
	TYPE_OTHERS,
} ;
int hanle_steps(int pid, p_step_t &p_step)
{
	pair<int, string> *pa ;

	//cout << p_step->line << endl ;

	if (p_step->line == "lock")	return TYPE_LOCK ; 
	if (p_step->line == "unlock")	return TYPE_UNLOCK ; 

	// get real steps
	if ((string::npos!=(p_step->line).find('=')) 
			|| (string::npos!=(p_step->line).find("print")))	
	{
		pa = new pair<int, string>(pid, p_step->line) ;
		g_order_queue.push(pa) ;
	}

	return TYPE_OTHERS ;
}

void get_order_queue_old(const int prog_nums, const int quantnum, const int step_nums) 
{
	deque<int> wait_deque ;
	queue<int> block_queue ;
	queue<p_step_t> *p_step_queue ;
	p_step_t p_step ;

	// initial
	for (int i = 1; i <= prog_nums; ++ i)	wait_deque.push_back(i) ;	

	int pid, qn, type, lock_pid = -1 ;
	while (!wait_deque.empty())
	{
		pid = wait_deque.front() ;
		wait_deque.pop_front () ;

		qn = quantnum ;
		p_step_queue = gpp_prog_queues[pid] ;
		while (!p_step_queue->empty())
		{
			p_step = p_step_queue->front() ; ;

			type = hanle_steps(pid, p_step) ;
			// ignore the nonexist condition from question fro simplify
			if (TYPE_LOCK == type)
			{
				// the lock is free
				if (0 > lock_pid)	lock_pid = pid ;	
				// the others own the lock, so pid(remove from wait_deque, push into block_queue)
				else if (lock_pid != pid)	
				{
					block_queue.push(pid) ;
					break ;	// to the next pid in wait_deque
				}
			}
			else if (TYPE_UNLOCK == type)
			{
				if (lock_pid == pid)	
				{
					lock_pid = -1 ;	// the lock is free

					// mv the head ofr block_queue to the head of wait_deque
					if (!block_queue.empty())
					{
						wait_deque.push_front (block_queue.front()) ;
						block_queue.pop () ;
					}
				}
			}
			p_step_queue->pop () ;

			// the prog has cost all its quannum
			if (0 >= (qn -= p_step->remain))	
			{
				// if prog hasnot finish all steps, insert into wait_deque again
				if (!p_step_queue->empty())	wait_deque.push_back(pid) ;
				break ;
			}
		}
	}
}


void get_order_queue(const int prog_nums, const int quantnum, const int step_nums) 
{
	deque<int> wait_deque ;
	queue<int> block_queue ;
	queue<p_step_t> *p_step_queue ;
	p_step_t p_step ;

	// initial
	for (int i = 1; i <= prog_nums; ++ i)	wait_deque.push_back(i) ;	

	int pid, qn, type, lock_pid = -1 ;
	while (!wait_deque.empty())
	{
		pid = wait_deque.front() ;
		wait_deque.pop_front () ;

		qn = quantnum ;
		p_step_queue = gpp_prog_queues[pid] ;
		while (!p_step_queue->empty())
		{
			p_step = p_step_queue->front() ; ;

			type = hanle_steps(pid, p_step) ;
			// ignore the nonexist condition from question fro simplify
			if (TYPE_LOCK == type)
			{
				// the lock is free
				if (0 > lock_pid)	lock_pid = pid ;	
				// the others own the lock, so pid(remove from wait_deque, push into block_queue)
				else if (lock_pid != pid)	
				{
					block_queue.push(pid) ;
					break ;	// to the next pid in wait_deque
				}
			}
			else if (TYPE_UNLOCK == type)
			{
				if (lock_pid == pid)	
				{
					lock_pid = -1 ;	// the lock is free

					// mv the head ofr block_queue to the head of wait_deque
					if (!block_queue.empty())
					{
						wait_deque.push_front (block_queue.front()) ;
						block_queue.pop () ;
					}
				}
			}
			p_step_queue->pop () ;

			// the prog has cost all its quannum
			if (0 >= (qn -= p_step->remain))	
			{
				// if prog hasnot finish all steps, insert into wait_deque again
				if (!p_step_queue->empty())	wait_deque.push_back(pid) ;
				break ;
			}
		}
	}
}

void test_order_queue()
{
	while (!g_order_queue.empty())
	{
		pair<int, string> *pa = g_order_queue.front() ;
		cout << pa->first << ": " << pa->second << endl ;
		g_order_queue.pop () ;
	}
}


void output_order_queue()
{
	pair<int, string> *pa ;
	string str ;
	size_t pos ;
	int value ;

	while (!g_order_queue.empty())
	{
		pa = g_order_queue.front() ;
		string &line = pa->second ;
		//cout << line << endl ;
		if (string::npos != (pos = line.find('=')))
		{
			str = line.substr(pos+1) ;
			value = atoi(str.c_str()) ;
			str = line.substr(0, pos) ;
			str = trim(str) ;
			// cout << str << value << endl ;
			g_var_dict[str] = value ; 
		}
		else if (string::npos != (pos = line.find("print")))
		{
			str = line.substr(pos+strlen("print")) ;
			str = trim(str) ;
			cout << pa->first << ": " << g_var_dict[str] << endl ;
		}
		g_order_queue.pop () ;
	}
}

// ignore memore relase
void clear_global(const int &prog_nums)
{
	g_stms_dict.clear () ;
	g_var_dict.clear () ;

	pair<int, string> *pa ;
	while (!g_order_queue.empty())	
	{
		pa = g_order_queue.front() ;
		g_order_queue.pop () ;
		delete pa ;
	}

	queue<p_step_t> *p ;
	p_step_t p_step ;
	for (int i = 1; i <= prog_nums; ++ i)
	{
		p = gpp_prog_queues[i] ;
		while (!p->empty())
		{
			p_step = p->front() ;
			p->pop () ;
			delete p_step ;
		}
		delete p ;
	}
	delete []gpp_prog_queues ;
}

int main()
{
#ifdef LOCAL
	freopen ("in", "r", stdin) ;
#endif 

	int num, quantnum, prog_nums, step_nums ;
	cin >> num ;
	while (num --)
	{
		get_input (prog_nums, quantnum, step_nums) ;
#ifdef LOCAL
		//test_input (prog_nums) ;
#endif 

		//get_order_queue_old (prog_nums, quantnum, step_nums) ;
		get_order_queue (prog_nums, quantnum, step_nums) ;
#ifdef LOCAL
		//test_order_queue () ;
#endif

		output_order_queue () ;

		clear_global (prog_nums) ;

		if (0 != num)	cout << endl ;
	}

	return 0 ;
}

#!/bin/bash

if [ $# -ne 1 ]; then
	echo "usages: $0 number" ;
fi

number=$1 ;

dirname="uva$number" ;
dt=$(date) ;

content="/*
* title: uva $number - 
* author: flq[flqnerve@163.com] - $dt
* note:
*/

#include <iostream>

using namespace std ;

int main()
{
#ifdef LOCAL
	freopen (\"in.uva\", \"r\", stdin) ;
#endif

	return 0 ;
}
"

mkdir $dirname ;
cpp="./$dirname/${dirname}.cpp" ;
if [ -f $cpp ]; then
	echo "$cpp exists" ;
else
	echo "$content" > "$cpp" ;
fi

cp ./Makefile $dirname/ ;

exit 0 ;


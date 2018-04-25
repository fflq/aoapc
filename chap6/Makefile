Includes=
Libs=

Flags=-g -pg -std=c++11 -DLOCAL

Target=main

all : $(subst .c,.co,$(wildcard *.c)) $(subst .cpp,.cppo,$(wildcard *.cpp))
	g++ -o $(Target) $^ $(Includes) $(Libs) $(Flags) 

%.co : %.c
	gcc -c $< -o $@ $(Includes) $(Flags)

%.cppo : %.cpp
	g++ -c $< -o $@ $(Includes) $(Libs) $(Flags)

clean :
	rm -f $(Target) *.co *.cppo

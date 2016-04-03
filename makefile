all: kiss kiss-dbg

kiss: kiss.cpp
	g++ -std=c++11 -O2 -DNDEBUG -Wall -Wconversion kiss.cpp -o kiss

kiss-dbg: kiss.cpp
	g++ -std=c++11 -Wall -g -Wconversion kiss.cpp -o kiss-dbg

install: kiss
	cp -n kiss /usr/local/bin && diff ./kiss /usr/local/bin/kiss

clean:
	rm kiss

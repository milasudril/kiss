kiss: kiss.cpp
	g++ -std=c++11 -O2 -DNDEBUG -Wall -Wconversion kiss.cpp -o kiss

clean:
	rm kiss

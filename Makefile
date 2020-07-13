CPPFLAGS=--std=c++11 -mtune=native -O3 -DNDEBUG=1 -fpermissive 
CXX=g++
RM=rm -f

all: pagination 

pagination: src/pagination.o
	$(CXX) -o pagination src/pagination.o -lcurl -lpthread

pagination.o: src/pagination.cpp
	$(CXX) $(CPPFLAGS) -c src/pagination.cpp -o src/pagination.o

.PHONY: clean
clean:
	$(RM) src/pagination.o pagination src/discernCPU 


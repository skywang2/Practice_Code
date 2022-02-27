select_server : select_server.o
	g++ -o select_server select_server.o -std=c++11

select_server.o : select_server.cc
	g++ -c select_server.cc -std=c++11
	
.PHONY : clean
clean : 
	-rm *.o

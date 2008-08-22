all : p5fs p5c  mywrite myappend myls 

p5fs : Fileserver.cc
	g++ -o p5fs Fileserver.cc -w

p5c : FileServerClient.c
	gcc -o p5c FileServerClient.c -w

mycat : mycat.cc
	g++ -o mycat mycat.cc -w

mywrite : mywrite.cc
	g++ -o mywrite mywrite.cc -w

myappend : myappend.cc
	g++ -o myappend myappend.cc -w

myls : myls.cc
	g++ -o myls myls.cc -w


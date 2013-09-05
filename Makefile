loginServer:loginServer.o tinyxmlparser.o tinyxmlerror.o tinystr.o tinyxml.o createFile.o 
	g++ -o loginServer loginServer.o tinyxmlparser.o tinyxmlerror.o tinystr.o tinyxml.o createFile.o -locci -lnnz11 -lclntsh -lpthread
loginServer.o:loginServer.cpp createFile.h
	g++ -c loginServer.cpp -DTIXML_USE_STL 
createFile.o:  createFile.cpp createFile.h
	g++ -c  createFile.cpp -DTIXML_USE_STL
tinyxmlparser.o:tinyxmlparser.cpp 
	g++ -c tinyxmlparser.cpp -DTIXML_USE_STL
tinyxmlerror.o:tinyxmlerror.cpp 
	g++ -c tinyxmlerror.cpp -DTIXML_USE_STL
tinystr.o:tinystr.cpp
	g++ -c tinystr.cpp  -DTIXML_USE_STL
tinyxml.o:tinyxml.cpp
	g++ -c tinyxml.cpp -DTIXML_USE_STL
clean:
	rm loginServer loginServer.o  tinyxmlparser.o tinyxmlerror.o tinystr.o tinyxml.o createFile.o

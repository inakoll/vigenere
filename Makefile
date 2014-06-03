all:vigenere.exe

vigenere.exe:main.cpp vigenere.hpp clo.hpp ascii.hpp alpha.hpp
	cls
	g++ -W -Wall -std=c++11 -O3 main.cpp -o vigenere.exe 
	
test: vigenere.exe
	vigenere -i test/french_input_test.txt -c vigenere -o test/french_input_test_c.txt
	vigenere -i test/french_input_test_c.txt -d
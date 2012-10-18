all:	main
main:	main.cpp
		g++ -o main main.cpp -g
clean:	
		rm main

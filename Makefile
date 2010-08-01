all: TYF

TYF:
	g++ src/*.cpp -o ./TenYardFootball -ltinyxml -ggdb -Wall
	
valgrind:
	g++ src/*.cpp -o ./TenYardFootball -ltinyxml -g -O0 -Wall
	valgrind --leak-check=yes ./TenYardFootball

release:
	g++ src/*.cpp -o ./TenYardFootball -ltinyxml -O2 -pipe -march=native

windows:
	i686-pc-mingw32-g++ -I../tinyxml ../tinyxml/*.cpp src/*.cpp -o ./TenYardFootball.exe -O2 -pipe

run:
	./TenYardFootball

clean:
	rm ./TenYardFootball

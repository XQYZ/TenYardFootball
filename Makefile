all: TYF

TYF:
	g++ src/*.cpp -o ./TenYardFootball -ltinyxml -ggdb -Wall
	
gui:
	g++ src/*.cpp -o ./TenYardFootball -ltinyxml -ggdb -Wall -DGUI `wx-config --cxxflags --libs`
	
release:
	g++ src/*.cpp -o ./TenYardFootball -ltinyxml -O2 -pipe -march=native `wx-config --cxxflags --libs`

windows:
	i686-pc-mingw32-g++ -I../tinyxml ../tinyxml/*.cpp src/*.cpp -o ./TenYardFootball.exe -O2 -pipe `/usr/i686-pc-mingw32/bin/wx-config --cxxflags --libs` -Wl,--subsystem,windows -mwindows

run:
	./TenYardFootball

clean:
	rm ./TenYardFootball

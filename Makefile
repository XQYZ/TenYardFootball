all: TYF

TYF:
	g++ src/*.cpp -o ./TenYardFootball -ltinyxml -ggdb
	
release: clean
	g++ src/*.cpp -o ./TenYardFootball -ltinyxml -O2 -pipe -march=native

run:
	./TenYardFootball

clean:
	rm ./TenYardFootball

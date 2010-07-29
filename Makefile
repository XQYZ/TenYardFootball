all:
	g++ src/*.cpp -o ./TenYardFootball -ltinyxml

run:
	./TenYardFootball

clean:
	rm ./TenYardFootball

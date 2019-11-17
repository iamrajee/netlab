p=shape
# p=fifo
q=fifo_new
# p=combined
run:
	g++ $(p).cpp -o $(p) -std=c++14
	g++ $(q).cpp -o $(q) -std=c++14
	./$(p) 250 4.0 < ./$(q) 250 4.0 < arrivals.txt
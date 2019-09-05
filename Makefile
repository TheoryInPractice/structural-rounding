
compile:
	gcc -O3 -o generator/generator.out generator/OCTgenerator.c -lm

small_graphs:
	mkdir -p graphs/small
	./generator/create_graphs_vc.sh 4000000 graphs/small

small_data:
	export PYTHONHASHSEED=0
	python3 main.py graphs/small/

medium_graphs:
	mkdir -p graphs/medium
	./generator/create_graphs_vc.sh 40000000 graphs/medium

medium_data:
	export PYTHONHASHSEED=0
	python3 main.py graphs/medium/

large_graphs:
	mkdir -p graphs/large
	./generator/create_graphs_vc.sh 200000000 graphs/large

large_data:
	export PYTHONHASHSEED=0
	python3 main.py graphs/large/

test_graphs:
	mkdir -p graphs/test
	./generator/create_graphs_vc.sh 100000 graphs/test

test_data:
	export PYTHONHASHSEED=0
	python3 variance.py graphs/test/

clean:
	rm -f generator/generator.out
	rm -r -f graphs/*

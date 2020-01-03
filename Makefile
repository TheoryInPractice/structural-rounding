
compile:
	gcc -O3 -o generator/generator.out generator/OCTgenerator.c -lm

small_graphs:
	mkdir -p graphs/small
	./generator/create_graphs_vc.sh 4000000 graphs/small

small_data:
	mkdir -p results
	export PYTHONHASHSEED=0
	python3 main.py graphs/small/

medium_graphs:
	mkdir -p graphs/medium
	./generator/create_graphs_vc.sh 40000000 graphs/medium

medium_data:
	mkdir -p results
	export PYTHONHASHSEED=0
	python3 main.py graphs/medium/

large_graphs:
	mkdir -p graphs/large
	./generator/create_graphs_vc.sh 200000000 graphs/large

large_data:
	mkdir -p results
	export PYTHONHASHSEED=0
	python3 main.py graphs/large/

test_graphs:
	mkdir -p graphs/test
	./generator/create_graphs_vc.sh 100000 graphs/test

test_data:
	mkdir -p results
	export PYTHONHASHSEED=0
	python3 variance.py graphs/test/

clean:
	rm -f generator/generator.out
	rm -r -f graphs/*


CC=g++
CCFLAGS=-Isr_apx/util -Isr_apx/setmap -Isr_apx/graph -Isr_apx/graphio -Isr_apx/vc/apx -Isr_apx/octset -Isr_apx/vc/exact -O3
PYTHON_DIR=/usr/include/python3.8

python: sr_apx/util/util.so sr_apx/setmap/setmap.so sr_apx/graph/graph.so sr_apx/graphio/graphio.so sr_apx/vc/apx/vc_apx.so sr_apx/octset/octset.so sr_apx/vc/exact/vc_exact.so

sr_apx/util/util.so: build/util.o sr_apx/util/util_module.cpp
	$(CC) $(CCFLAGS) -shared -fPIC -I$(PYTHON_DIR) -o sr_apx/util/util.so sr_apx/util/util_module.cpp build/util.o

build/util.o: sr_apx/util/util.hpp sr_apx/util/util.cpp
	mkdir -p build
	$(CC) $(CCFLAGS) -c -o build/util.o sr_apx/util/util.cpp

sr_apx/setmap/setmap.so: sr_apx/setmap/setmap.hpp sr_apx/setmap/setmap.tpp sr_apx/setmap/setmap_module.cpp sr_apx/setmap/pyset.hpp build/util.o
	$(CC) $(CCFLAGS) -shared -fPIC -I$(PYTHON_DIR) -o sr_apx/setmap/setmap.so sr_apx/setmap/setmap_module.cpp build/util.o

sr_apx/graph/graph.so: build/graph.o sr_apx/setmap/pyset.hpp sr_apx/setmap/setmap.so sr_apx/graph/graph_module.cpp
	$(CC) $(CCFLAGS) -shared -fPIC -I$(PYTHON_DIR) -o sr_apx/graph/graph.so sr_apx/graph/graph_module.cpp sr_apx/setmap/setmap.so build/graph.o

build/graph.o: sr_apx/graph/graph.hpp sr_apx/graph/graph.cpp
	mkdir -p build
	$(CC) $(CCFLAGS) -c -o build/graph.o sr_apx/graph/graph.cpp

sr_apx/graphio/graphio.so: build/util.o build/graph.o sr_apx/graph/pygraph.hpp sr_apx/graph/graph.so sr_apx/graphio/graphio.hpp sr_apx/graphio/graphio.cpp sr_apx/graphio/graphio_module.cpp
	$(CC) $(CCFLAGS) -shared -fPIC -I$(PYTHON_DIR) -o sr_apx/graphio/graphio.so sr_apx/graphio/graphio_module.cpp sr_apx/graph/graph.so build/graph.o build/util.o sr_apx/graphio/graphio.cpp

sr_apx/vc/apx/vc_apx.so: build/vc_apx.o sr_apx/vc/apx/vc_apx_module.cpp sr_apx/setmap/setmap.so build/graph.o
	$(CC) $(CCFLAGS) -shared -fPIC -I$(PYTHON_DIR) -o sr_apx/vc/apx/vc_apx.so sr_apx/vc/apx/vc_apx_module.cpp build/vc_apx.o sr_apx/setmap/setmap.so build/graph.o

build/vc_apx.o: sr_apx/vc/apx/vc_apx.hpp sr_apx/vc/apx/vc_apx.cpp
	mkdir -p build
	$(CC) $(CCFLAGS) -c -o build/vc_apx.o sr_apx/vc/apx/vc_apx.cpp

sr_apx/octset/octset.so: build/graph.o sr_apx/octset/oct_module.cpp sr_apx/octset/octset.cpp sr_apx/octset/octset.hpp
	$(CC) $(CCFLAGS) -shared -fPIC -I$(PYTHON_DIR) -o sr_apx/octset/octset.so sr_apx/octset/oct_module.cpp sr_apx/setmap/setmap.so build/graph.o sr_apx/octset/octset.cpp

sr_apx/vc/exact/vc_exact.so: build/vc_exact.o sr_apx/octset/octset.cpp sr_apx/setmap/setmap.so sr_apx/graph/graph.so
	$(CC) $(CCFLAGS) -shared -fPIC -I$(PYTHON_DIR) -o sr_apx/vc/exact/vc_exact.so sr_apx/vc/exact/vc_exact_module.cpp build/vc_exact.o build/graph.o sr_apx/setmap/setmap.so sr_apx/graph/graph.so sr_apx/octset/octset.so

build/vc_exact.o: sr_apx/vc/exact/vc_exact.hpp sr_apx/vc/exact/vc_exact.cpp
	$(CC) $(CCFLAGS) -c -o build/vc_exact.o sr_apx/vc/exact/vc_exact.cpp

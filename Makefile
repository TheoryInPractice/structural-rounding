
CC=g++
CCFLAGS=-O3 -std=c++17

PYINCLUDE=$(shell python3-config --includes)
PYFLAGS=$(shell python3-config --ldflags)

INCLUDES=-Isr_apx/graph/ -Isr_apx/util/ -Isr_apx/graphio/ -Isr_apx/setmap/ -Isr_apx/vc/apx/ -Isr_apx/vc/exact/ -Isr_apx/vc/lift/ -Isr_apx/octset/

build/util.o: sr_apx/util/util.cpp sr_apx/util/util.hpp
	mkdir -p build
	$(CC) $(CCFLAGS) -c $(INCLUDES) -o build/util.o sr_apx/util/util.cpp

build/graph.o: sr_apx/graph/graph.cpp sr_apx/graph/graph.hpp
	mkdir -p build
	$(CC) $(CCFLAGS) -c $(INCLUDES) -o build/graph.o sr_apx/graph/graph.cpp

build/graphio.o: sr_apx/graphio/graphio.cpp sr_apx/graphio/graphio.hpp
	mkdir -p build
	$(CC) $(CCFLAGS) -c $(INCLUDES) -fPIC -o build/graphio.o sr_apx/graphio/graphio.cpp

build/vc_apx.o: sr_apx/vc/apx/vc_apx.cpp sr_apx/vc/apx/vc_apx.hpp
	mkdir -p build
	$(CC) $(CCFLAGS) -c $(INCLUDES) -o build/vc_apx.o sr_apx/vc/apx/vc_apx.cpp

build/vc_exact.o: sr_apx/vc/exact/vc_exact.cpp sr_apx/vc/exact/vc_exact.hpp
	mkdir -p build
	$(CC) $(CCFLAGS) -c $(INCLUDES) -o build/vc_exact.o sr_apx/vc/exact/vc_exact.cpp

build/vc_lift.o: sr_apx/vc/lift/vc_lift.cpp sr_apx/vc/lift/vc_lift.hpp
	mkdir -p build
	$(CC) $(CCFLAGS) -c $(INCLUDES) -o build/vc_lift.o sr_apx/vc/lift/vc_lift.cpp

build/octset.o: sr_apx/octset/octset.cpp sr_apx/octset/octset.hpp
	mkdir -p build
	$(CC) $(CCFLAGS) -c $(INCLUDES) -fPIC -o build/octset.o sr_apx/octset/octset.cpp

sr_apx.so: build/util.o build/graph.o build/graphio.o build/vc_apx.o build/vc_exact.o build/vc_lift.o build/octset.o sr_apx/setmap/setmap.hpp sr_apx/setmap/setmap.tpp
	mkdir -p build
	$(CC) $(CCFLAGS) -o sr_apx.so -shared -fPIC $(INCLUDES) build/util.o build/graph.o build/graphio.o build/vc_apx.o build/vc_exact.o build/vc_lift.o build/octset.o

build/main.o: main.cpp
	mkdir -p build
	$(CC) $(CCFLAGS) -c $(INCLUDES) -o build/main.o main.cpp

cpp: build/main.o sr_apx.so
	$(CC) $(CCFLAGS) -o main $(INCLUDES) build/main.o ./sr_apx.so

sr_apx/util/util.so: sr_apx.so sr_apx/util/util_module.cpp
	$(CC) $(CCFLAGS) -shared -fPIC $(INCLUDES) $(PYINCLUDE) -o sr_apx/util/util.so ./sr_apx.so sr_apx/util/util_module.cpp $(PYFLAGS)

sr_apx/setmap/setmap.so: sr_apx.so sr_apx/setmap/setmap.tpp sr_apx/setmap/setmap.hpp sr_apx/setmap/setmap_module.cpp sr_apx/setmap/pyset.hpp
	$(CC) $(CCFLAGS) -shared -fPIC $(INCLUDES) $(PYINCLUDE) -o sr_apx/setmap/setmap.so sr_apx/setmap/setmap_module.cpp ./sr_apx.so $(PYFLAGS)

sr_apx/graph/graph.so: sr_apx.so sr_apx/setmap/setmap.so sr_apx/graph/graph_module.cpp sr_apx/graph/pygraph.hpp
	$(CC) $(CCFLAGS) -shared -fPIC $(INCLUDES) $(PYINCLUDE) -o sr_apx/graph/graph.so sr_apx/setmap/setmap.so ./sr_apx.so sr_apx/graph/graph_module.cpp $(PYFLAGS)

sr_apx/graphio/graphio.so: sr_apx.so sr_apx/graph/graph.so sr_apx/graphio/graphio_module.cpp
	$(CC) $(CCFLAGS) -shared -fPIC $(INCLUDES) $(PYINCLUDE) -o sr_apx/graphio/graphio.so sr_apx/graph/graph.so ./sr_apx.so sr_apx/graphio/graphio_module.cpp $(PYFLAGS)

sr_apx/vc/apx/vc_apx.so: sr_apx.so sr_apx/vc/apx/vc_apx_module.cpp sr_apx/setmap/setmap.so
	$(CC) $(CCFLAGS) -shared -fPIC $(INCLUDES) $(PYINCLUDE) -o sr_apx/vc/apx/vc_apx.so sr_apx/setmap/setmap.so ./sr_apx.so sr_apx/vc/apx/vc_apx_module.cpp $(PYFLAGS)

sr_apx/octset/octset.so: sr_apx.so sr_apx/octset/oct_module.cpp sr_apx/setmap/setmap.so
	$(CC) $(CCFLAGS) -shared -fPIC $(INCLUDES) $(PYINCLUDE) -o sr_apx/octset/octset.so ./sr_apx.so sr_apx/setmap/setmap.so sr_apx/octset/oct_module.cpp $(PYFLAGS)

sr_apx/vc/exact/vc_exact.so: sr_apx.so sr_apx/octset/octset.cpp sr_apx/setmap/setmap.so sr_apx/graph/graph.so
	$(CC) $(CCFLAGS) -shared -fPIC $(INCLUDES) $(PYINCLUDE) -o sr_apx/vc/exact/vc_exact.so sr_apx/vc/exact/vc_exact_module.cpp ./sr_apx.so sr_apx/setmap/setmap.so sr_apx/graph/graph.so sr_apx/octset/octset.so $(PYFLAGS)

sr_apx/vc/lift/vc_lift.so: sr_apx.so sr_apx/setmap/setmap.so sr_apx/graph/graph.so sr_apx/vc/lift/vc_lift_module.cpp
	$(CC) $(CCFLAGS) -shared -fPIC $(INCLUDES) $(PYINCLUDE) -o sr_apx/vc/lift/vc_lift.so sr_apx/vc/lift/vc_lift_module.cpp sr_apx/graph/graph.so sr_apx/setmap/setmap.so ./sr_apx.so $(PYFLAGS)

python: sr_apx/util/util.so sr_apx/setmap/setmap.so sr_apx/graph/graph.so sr_apx/graphio/graphio.so sr_apx/vc/apx/vc_apx.so sr_apx/octset/octset.so sr_apx/vc/exact/vc_exact.so sr_apx/vc/lift/vc_lift.so

generator: generator/OCTgenerator.c
	gcc -O3 -o generator/generator.out generator/OCTgenerator.c -lm

clean:
	rm -f sr_apx.so
	rm -f -r build
	rm -f main
	rm -f sr_apx/util/util.so
	rm -f sr_apx/setmap/setmap.so
	rm -f sr_apx/graph/graph.so
	rm -f sr_apx/graphio/graphio.so
	rm -f sr_apx/vc/apx/vc_apx.so
	rm -f sr_apx/octset/octset.so
	rm -f sr_apx/vc/exact/vc_exact.so
	rm -f sr_apx/vc/lift/vc_lift.so


CC=g++
CCFLAGS=-O3 -std=c++11 -fPIC

INCLUDES=-Isr_apx/graph/ -Isr_apx/util/ -Isr_apx/setmap/ -Isr_apx/vc/apx/ -Isr_apx/vc/exact/ -Isr_apx/vc/lift/ -Isr_apx/vc/kernel/ -Isr_apx/octset/

PYINCLUDE=$(shell python3-config --includes)
PYFLAGS=$(shell python3-config --ldflags) -L. -L./sr_apx/setmap -L./sr_apx/graph -Wl,-rpath,. -Wl,-rpath,./sr_apx/setmap -Wl,-rpath,./sr_apx/graph

# cpp #####################################################################################################

build/util.o: sr_apx/util/util.cpp sr_apx/util/util.hpp
	mkdir -p build
	$(CC) $(CCFLAGS) -c $(INCLUDES) -o build/util.o sr_apx/util/util.cpp

build/graph.o: sr_apx/graph/graph.cpp sr_apx/graph/graph.hpp
	mkdir -p build
	$(CC) $(CCFLAGS) -c $(INCLUDES) -o build/graph.o sr_apx/graph/graph.cpp

build/vc_apx.o: sr_apx/vc/apx/vc_apx.cpp sr_apx/vc/apx/vc_apx.hpp
	mkdir -p build
	$(CC) $(CCFLAGS) -c $(INCLUDES) -o build/vc_apx.o sr_apx/vc/apx/vc_apx.cpp

build/vc_exact.o: sr_apx/vc/exact/vc_exact.cpp sr_apx/vc/exact/vc_exact.hpp
	mkdir -p build
	$(CC) $(CCFLAGS) -c $(INCLUDES) -o build/vc_exact.o sr_apx/vc/exact/vc_exact.cpp

build/vc_lift.o: sr_apx/vc/lift/vc_lift.cpp sr_apx/vc/lift/vc_lift.hpp
	mkdir -p build
	$(CC) $(CCFLAGS) -c $(INCLUDES) -o build/vc_lift.o sr_apx/vc/lift/vc_lift.cpp

build/vc_kernel.o: sr_apx/vc/kernel/lp_kernel.cpp sr_apx/vc/kernel/lp_kernel.hpp
	mkdir -p build
	$(CC) $(CCFLAGS) -c $(INCLUDES) -o build/vc_kernel.o sr_apx/vc/kernel/lp_kernel.cpp

build/octset.o: sr_apx/octset/octset.cpp sr_apx/octset/octset.hpp
	mkdir -p build
	$(CC) $(CCFLAGS) -c $(INCLUDES) -o build/octset.o sr_apx/octset/octset.cpp

lib_sr_apx.so: build/util.o build/graph.o build/vc_apx.o build/vc_exact.o build/vc_lift.o build/vc_kernel.o build/octset.o sr_apx/setmap/setmap.hpp sr_apx/setmap/setmap.tpp
	$(CC) -shared -o lib_sr_apx.so build/util.o build/graph.o build/vc_apx.o build/vc_exact.o build/vc_lift.o build/vc_kernel.o build/octset.o

build/main.o: main.cpp
	mkdir -p build
	$(CC) -O3 -std=c++11 -c $(INCLUDES) -o build/main.o main.cpp

cpp: build/main.o lib_sr_apx.so
	$(CC) -o main -L. -Wl,-rpath,. build/main.o -l_sr_apx

# python ###########################################################################################################

build/util_module.o: sr_apx/util/util_module.cpp
	mkdir -p build
	$(CC) $(CCFLAGS) -c $(INCLUDES) $(PYINCLUDE) -o build/util_module.o sr_apx/util/util_module.cpp

sr_apx/util/lib_util.so: lib_sr_apx.so build/util_module.o
	$(CC) -shared -o sr_apx/util/lib_util.so build/util_module.o $(PYFLAGS) -l_sr_apx

build/setmap_module.o: sr_apx/setmap/setmap.tpp sr_apx/setmap/setmap.hpp sr_apx/setmap/setmap_module.cpp sr_apx/setmap/pyset.hpp
	$(CC) $(CCFLAGS) -c $(INCLUDES) $(PYINCLUDE) -o build/setmap_module.o sr_apx/setmap/setmap_module.cpp

sr_apx/setmap/lib_setmap.so: lib_sr_apx.so build/setmap_module.o
	$(CC) -shared -o sr_apx/setmap/lib_setmap.so build/setmap_module.o $(PYFLAGS) -l_sr_apx

build/graph_module.o: sr_apx/graph/graph_module.cpp sr_apx/graph/pygraph.hpp
	$(CC) $(CCFLAGS) -c $(INCLUDES) $(PYINCLUDE) -o build/graph_module.o sr_apx/graph/graph_module.cpp

sr_apx/graph/lib_graph.so: lib_sr_apx.so sr_apx/setmap/lib_setmap.so build/graph_module.o
	$(CC) -shared -o sr_apx/graph/lib_graph.so build/graph_module.o $(PYFLAGS) -l_sr_apx -l_setmap

build/vc_apx_module.o: sr_apx/vc/apx/vc_apx_module.cpp
	$(CC) $(CCFLAGS) -c $(INCLUDES) $(PYINCLUDE) -o build/vc_apx_module.o sr_apx/vc/apx/vc_apx_module.cpp

sr_apx/vc/apx/lib_vc_apx.so: lib_sr_apx.so build/vc_apx_module.o sr_apx/setmap/lib_setmap.so
	$(CC) -shared -o sr_apx/vc/apx/lib_vc_apx.so build/vc_apx_module.o $(PYFLAGS) -l_sr_apx -l_setmap

build/oct_module.o: sr_apx/octset/oct_module.cpp
	$(CC) $(CCFLAGS) -c $(INCLUDES) $(PYINCLUDE) -o build/oct_module.o sr_apx/octset/oct_module.cpp

sr_apx/octset/lib_octset.so: lib_sr_apx.so build/oct_module.o sr_apx/setmap/lib_setmap.so
	$(CC) -shared -o sr_apx/octset/lib_octset.so build/oct_module.o $(PYFLAGS) -l_sr_apx -l_setmap

build/vc_exact_module.o: sr_apx/vc/exact/vc_exact_module.cpp
	$(CC) $(CCFLAGS) -c $(INCLUDES) $(PYINCLUDE) -o build/vc_exact_module.o sr_apx/vc/exact/vc_exact_module.cpp

sr_apx/vc/exact/lib_vc_exact.so: lib_sr_apx.so sr_apx/setmap/lib_setmap.so build/vc_exact_module.o
	$(CC) -shared -o sr_apx/vc/exact/lib_vc_exact.so build/vc_exact_module.o $(PYFLAGS) -l_sr_apx -l_setmap

build/vc_lift_module.o: sr_apx/vc/lift/vc_lift_module.cpp
	$(CC) $(CCFLAGS) -c $(INCLUDES) $(PYINCLUDE) -o build/vc_lift_module.o sr_apx/vc/lift/vc_lift_module.cpp

sr_apx/vc/lift/lib_vc_lift.so: lib_sr_apx.so sr_apx/setmap/lib_setmap.so build/vc_lift_module.o
	$(CC) -shared -o sr_apx/vc/lift/lib_vc_lift.so build/vc_lift_module.o $(PYFLAGS) -l_sr_apx -l_setmap

build/lp_kernel_module.o: sr_apx/vc/kernel/lp_kernel_module.cpp
	$(CC) $(CCFLAGS) -c $(INCLUDES) $(PYINCLUDE) -o build/lp_kernel_module.o sr_apx/vc/kernel/lp_kernel_module.cpp

sr_apx/vc/kernel/lib_lp_kernel.so: lib_sr_apx.so sr_apx/setmap/lib_setmap.so build/lp_kernel_module.o
	$(CC) -shared -o sr_apx/vc/kernel/lib_lp_kernel.so build/lp_kernel_module.o $(PYFLAGS) -l_sr_apx -l_setmap

python: sr_apx/util/lib_util.so sr_apx/setmap/lib_setmap.so sr_apx/graph/lib_graph.so sr_apx/vc/apx/lib_vc_apx.so sr_apx/octset/lib_octset.so sr_apx/vc/exact/lib_vc_exact.so sr_apx/vc/lift/lib_vc_lift.so sr_apx/vc/kernel/lib_lp_kernel.so

# generator ##########################################################################################

generator: generator/generator.out

generator/generator.out: generator/OCTgenerator.c
	gcc -O3 -std=gnu11 -o generator/generator.out generator/OCTgenerator.c -lm

# remove compiled files ###############################################################################

clean:
	rm -f -r build
	rm -f main
	rm -f lib_sr_apx.so
	rm -f generator/generator.out
	rm -f sr_apx/util/lib_util.so
	rm -f sr_apx/setmap/lib_setmap.so
	rm -f sr_apx/graph/lib_graph.so
	rm -f sr_apx/vc/apx/lib_vc_apx.so
	rm -f sr_apx/octset/lib_octset.so
	rm -f sr_apx/vc/exact/lib_vc_exact.so
	rm -f sr_apx/vc/lift/lib_vc_lift.so

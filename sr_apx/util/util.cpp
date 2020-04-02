
#include "util.hpp"

int log2(int n) {
	int k = 0;
	while ((1 << k) < n) {
		++k;
	}
	return k;
}

int pow(int b, int e) {
	int x = 1;
	for (int i = 0; i < e; i++) {
		x *= b;
	}

	return x;
}


#include "util.hpp"
#include <cstddef>
#include <new>

#define DEFAULT_SIZE 3
#define LINEAR_PROBE 4

template<class T>
int Map<T>::lookup(int key, bool stop) {
	int mask = (1 << logsize) - 1;
	int index = key & mask;
	int x = array[index].key;
	while (x != EMPTY && x != key && (x != ERASED || !stop)) {
		for (int i = 0; i < LINEAR_PROBE && index + i < 1 << logsize; i++) {
			x = array[index + i].key;
			if (x == EMPTY || x == key || (x == ERASED && stop)) {
				return index + i;
			}
		}
		index = (5 * index + 1) & mask;
		x = array[index].key;
	}

	return index;
}

template<class T>
void Map<T>::initialize(int ls) {
	load = 0;
	eraseload = 0;
	logsize = ls;
	void* buffer = operator new((1 << logsize) * sizeof(Elem<T>));
	array = (Elem<T>*) buffer;
	for (int i = 0; i < 1 << logsize; i++) {
		new(array + i) Elem<T>();
		array[i].key = EMPTY;
	}
}

template<class T>
void Map<T>::rehash(int ls) {
	Elem<T>* oldarray = array;
	int oldsize = logsize;

	initialize(ls > DEFAULT_SIZE ? ls : DEFAULT_SIZE);

	for (int i = 0; i < 1 << oldsize; i++) {
		int x = oldarray[i].key;
		if (x != EMPTY && x != ERASED) {
			int index = lookup(x, false);
			array[index] = oldarray[i];
			++load;
		}
	}

	delete (char*) oldarray;
}

template<class T>
Map<T>::Map() {
	load = 0;
	eraseload = 0;
	logsize = -1;
	array = NULL;
}

template<class T>
Map<T>::Map(int size) {
	initialize(log2(size));
}

template<class T>
Map<T>::~Map() {
	delete (char*) array;
}

template<class T>
void Map<T>::clear() {
	for (int i = 0; i < 1 << logsize; i++) {
		array[i].~Elem<T>();
	}
}

template<class T>
int Map<T>::size() {
	return load;
}

template<class T>
int Map<T>::max_size() {
	return 1 << logsize;
}

template<class T>
bool Map<T>::empty() {
	return load == 0;
}

template<class T>
float Map<T>::load_factor() {
	return float(load) / float(1 << logsize);
}

template<class T>
int Map<T>::max_load() {
	return ((1 << logsize) * 3) >> 2;
}

template<class T>
int Map<T>::min_load() {
	return 1 << (logsize - 2);
}

template<class T>
bool Map<T>::contains(int key) {
	if (logsize < 0) {
		return false;
	}

	int index = lookup(key, false);
	return array[index].key == key;
}

template<class T>
void Map<T>::erase(int key) {
	if (logsize < 0) {
		return;
	}

	int index = lookup(key, false);
	if (array[index].key != key) {
		return;
	}

	--load;
	++eraseload;
	array[index].~Elem<T>();
	new(&array[index]) Elem<T>();
	array[index].key = ERASED;

	if (load < eraseload) {
		rehash(logsize - 1);
	}
}

template<class T>
void Map<T>::remove(int key) {
	if (logsize < 0) {
		return;
	}

	int index = lookup(key, false);
	if (array[index].key != key) {
		return;
	}

	--load;
	++eraseload;
	array[index].key = ERASED;

	if (load < eraseload) {
		rehash(logsize - 1);
	}
}

template<class T>
void Map<T>::insert(int key, T& value) {
	if (load + eraseload >= max_load()) {
		rehash(logsize + 1);
	}

	int index = lookup(key, false);
	if (array[index].key == key) {
		return;
	}

	++load;
	// index = lookup(key, true);
	array[index].key = key;
	array[index].value = value;
}

template<class T>
void Map<T>::insert(int key) {
	if (load + eraseload >= max_load()) {
		rehash(logsize + 1);
	}

	int index = lookup(key, false);
	if (array[index].key == key) {
		return;
	}

	++load;
	// index = lookup(key, true);
	array[index].key = key;
}

template<class T>
T& Map<T>::at(int key) {
	if (load + eraseload >= max_load()) {
		rehash(logsize + 1);
	}

	int index = lookup(key, false);
	if (array[index].key == key) {
		return array[index].value;
	}

	++load;
	// index = lookup(key, true);
	array[index].key = key;
	return array[index].value;
}

template<class T>
T& Map<T>::operator[](int key) {
	if (load + eraseload >= max_load()) {
		rehash(logsize + 1);
	}

	int index = lookup(key, false);
	if (array[index].key == key) {
		return array[index].value;
	}

	++load;
	// index = lookup(key, true);
	array[index].key = key;
	return array[index].value;
}

template<class T>
void Map<T>::reserve(int size) {
	if (size < load) {
		return;
	}
	rehash(log2(size) + 1);
}

template<class T>
typename Map<T>::Iterator Map<T>::find(int key) {
	if (logsize < 0) {
		return Iterator(1 << logsize, this);
	}

	int index = lookup(key, false);
	if (array[index].key != key) {
		return Iterator(1 << logsize, this);
	}

	return Iterator(index, this);
}

template<class T>
typename Map<T>::Iterator Map<T>::begin() {
	if (logsize < 0) {
		return Iterator(1 << logsize, this);
	}
	return Iterator(this);
}

template<class T>
typename Map<T>::Iterator Map<T>::end() {
	return Iterator(1 << logsize, this);
}


#ifndef SETMAP_H
#define SETMAP_H

// key values for empty and erased elems in the array
#define EMPTY -1
#define ERASED -2

// value type used for sets
struct NullObj {};

// standard Elem key, value object for maps
template<class T>
struct Elem {
	int key;
	T value;
};

// specialized Elem object for sets
template<>
struct Elem<NullObj> {
	int key;
};

template<class T>
class Map {
private:
	int load;
	int eraseload;
	int logsize;
	Elem<T>* array;

	int lookup(int, bool);
	void initialize(int);
	void rehash(int);
public:
	class Iterator {
	private:
		int index;
		Map<T>* ref;
	public:
		Iterator(Map<T>* r) {index = -1; ref = r; ++*this;};
		Iterator(int i, Map<T>* r) {index = i; ref = r;};
		Iterator operator++() {
			if (index >= 1 << ref->logsize) {
				index = 1 << ref->logsize;
				return *this;
			}

			++index;
			while (index < 1 << ref->logsize && (ref->array[index].key == ERASED || ref->array[index].key == EMPTY)) {
				++index;
			}
			return *this;
		};
		Iterator operator++(int) {
			Iterator temp = *this;
			++*this;
			return temp;
		};
		bool operator==(const Iterator x) {return ref == x.ref && index == x.index;}
		bool operator!=(const Iterator x) {return ref != x.ref || index != x.index;}
		Elem<T>* operator->() {
			return &(ref->array[index]);
		};
		int operator*() {return ref->array[index].key;};
	};

	Map();
	Map(int);
	~Map();
	void clear();

	int size();
	int max_size();
	bool empty();
	float load_factor();
	int max_load();
	int min_load();

	void insert(int, T&);
	void insert(int);
	void erase(int);
	void remove(int);
	bool contains(int);

	T &operator[](int);
	T& at(int);

	Iterator find(int);
	Iterator begin();
	Iterator end();

	void reserve(int);
};

#include "setmap.tpp"

using Set = Map<NullObj>;

#endif

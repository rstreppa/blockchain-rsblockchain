/**
Merkle.h
Purpose:	Template Class for Merkle Trees

@author rstreppa
@version 1.0 09/17/2018
*/

#pragma once

#include <vector>
#include <iostream>
#include "Utility.h"
#include <string>

using namespace std;

template <typename T>
class Merkle {
public:
	Merkle(T(*f)(const T&, const T&)) { this->hasher = f; }
	int size() { return values.size(); }
	void add(const T& value) { values.push_back(value); }

	T root()
	{
		vector<T> current;
		current = getHashedParents(this->values);
		while (current.size() != 1)
			current = getHashedParents(current);
		return current[0];
	}

private:
	vector<T> values;
	T(*hasher)(const T&, const T&);
	vector<T> getHashedParents(const vector<T> &children)
	{
		vector<T> result;
		for (int i = 0; i < children.size();)
		{
			T a = children[i], b = children[i];
			if (++i < children.size())
				b = children[i++];
			T hash = this->hasher(a, b);
			cout << "hash(" << a << ", " << b << ") => " << hash << " ";
			result.push_back(hash);
		}
		cout << "\n";
		return result;
	}
};

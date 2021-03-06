#pragma once

#include <string>
#include <sstream>

#include "LinkedList.h"

using namespace std;

template<class T>
class AVL {
private:
	const int MAX_PER_LINE = 8;
	
	class node {
	public:
		node(T value) : value(value), height(1), left(NULL), right(NULL) {}
		~node() {
			delete left;
			delete right;
		}
		
		T value;
		int height;
		node* left;
		node* right;
	};
	
	node* root;
	int count;
	
	int height(node*& n) {
		if (n == NULL) return 0;
		else return n->height;
	}
	
	void detach(node*& n) {
		n->left = NULL;
		n->right = NULL;
	}
	
	int checkHeight(node*& n) {
		int tallest = 0;
		int hLeft = height(n->left);
		int hRight = height(n->right);
		if (hLeft > hRight) tallest = hLeft;
		else tallest = hRight;
		return tallest + 1;
	}
	
	bool balanced(node*& n) {
		int i = imbalance(n);
		return -1 <= i && i <= 1;
	}
	
	int imbalance(node*& n) {
		return height(n->right) - height(n->left);
	}
	
	void rotateLeft(node*& n) {
		// cout << "rotating " << n->value << " left" << endl;
		node* k = n->right;
		n->right = k->left;
		k->left = n;
		n->height = checkHeight(n);
		k->height = checkHeight(k);
		n = k;
	}
	
	void rotateRight(node*& n) {
		// cout << "rotating " << n->value << " right" << endl;
		node* k = n->left;
		n->left = k->right;
		k->right = n;
		n->height = checkHeight(n);
		k->height = checkHeight(k);
		n = k;
	}
	
	void rebalance (node*& n) {
		if (!balanced(n)) {
			if (imbalance(n) < -1) {
				if (imbalance(n->left) > 0) rotateLeft(n->left);
				rotateRight(n);
			}
			else if (imbalance(n) > 1) {
				if (imbalance(n->right) < 0) rotateRight(n->right);
				rotateLeft(n);
			}
			
		}
		// else {
		// 	cout << n->value << " is balanced" << endl;
		// }
	}
	
	node* rFind(node*& n, T value) {
		if (n == NULL || n->value == value) {
			return n;
		}
		else if (value < n->value) {
			return rFind(n->left, value);
		}
		else if (value > n->value) {
			return rFind(n->right, value);
		}
		else throw n; // shouldn't get here
	}
	
	void rInsert(node*& n, T value) {
		if (n == NULL) {
			// cout << "inserting " << value << endl;
			n = new node(value);
			count++;
		}
		else if (value < n->value) {
			rInsert(n->left, value);
		}
		else if (value > n->value) {
			rInsert(n->right, value);
		}
		n->height = checkHeight(n);
		rebalance(n);
	}
	
	node* findMin(node*& n) {
		if (n->left != NULL)
			return findMin(n->left);
		else return n;
	}
	
	void rRemove(node*& n, T value) {
		if (n == NULL) 
			return;
		else if (value < n->value) {
			rRemove(n->left, value);
		}
		else if (value > n->value) {
			rRemove(n->right, value);
		}
		else {
			// cout << "removing " << value << endl;
			if (n->right == NULL) {
				node* d = n;
				n = n->left;
				detach(d);
				delete d;
			}
			else if (n->left == NULL) {
				node* d = n;
				n = n->right;
				detach(d);
				delete d;
			}
			else {
				node* min = findMin(n->right);
				n->value = min->value;
				rRemove(n->right, n->value);
			}
			count--;
		}
		if (n != NULL) {
			n->height = checkHeight(n);
			rebalance(n);
		} 
	}
	
	string rToStringInorder(node*& n) {
		stringstream ss;

		if (n != NULL) {
			ss << rToStringInorder(n->left);
			ss << n->value << "(" << n->height << ") ";
			ss << rToStringInorder(n->right);
		}

		return ss.str();
	}
	
	string rToStringPreorder(node*& n) {
		stringstream ss;

		if (n != NULL) {
			ss << n->value << "(" << n->height << ") ";
			ss << rToStringPreorder(n->left);
			ss << rToStringPreorder(n->right);
		}

		return ss.str();
	}
	
public:
	AVL() : root(NULL), count(0) {}
	~AVL() { delete root; }
	
	bool find(T value) { 
		node* n = rFind(root, value); 
		if (n == NULL) return false;
		else return true;
	}
	void insert(T value) { rInsert(root, value); }
	void remove(T value) { rRemove(root, value); }
	void clear() {
		node* d = root;
		count = 0;
		root = NULL;
		delete d;
	}
	
	string toStringInorder() { 
		if (root == NULL) return "EMPTY";
		return rToStringInorder(root); 
	}
	string toStringPreorder() { 
		if (root == NULL) return "EMPTY";
		return rToStringPreorder(root); 
	}
	string toString() {
		stringstream ss;
		
		LinkedList<node*> queue;
		if (root != NULL) queue.insert(root, 0);
		int level = -1;
		
		while (queue.getSize() > 0) {
			level++;
			int size = queue.getSize();
			
			for (int i = 0; i < size; i++) {
				if (i % MAX_PER_LINE == 0) ss << endl << "level " << level << ":";
			
				node* n = queue.remove(queue.getSize() - 1);
				if (n->left != NULL) queue.insert(n->left, 0);
				if (n->right != NULL) queue.insert(n->right, 0);
				ss << " " << n->value << "(" << n->height << ")";
			}
		}
		
		return ss.str();
	}
	
	int getCount() { return count; }
};
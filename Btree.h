#include <iostream>
#include <fstream>
#include <filesystem>
#include "String.h"
// #include "Utility_functions.h"
// #include "queue.h"
#include <unistd.h>
#include <cmath> //for ceil function

using namespace std;

int curr_node = 1;
// btree node struct
template <typename t>
struct Btree_node
{

	int order; // order of the btree
	int n;	   // current number of keys
	t *keys;   // array of teh keys
	// Btree_node** children;  //array to keep track of the child pointers
	// Btree_node* parent;
	bool leaf;
	String *children;
	String parent;
	Btree_node(int o = 0, t *k = nullptr) //, String* c = "nullptr", int num = 0, bool l = true)
	{
		order = o;
		if (k == nullptr)
		{
			keys = new t[order];
		}
		else
		{
			keys = k;
		}

		children = new String[order + 1];
		for (int i = 0; i <= order; i++)
		{
			// initially we set all child pointers to null
			children[i] = "nullptr";
		}

		n = 0;
		leaf = true;
	}
	// constructor to initialise from node
	Btree_node(String init)
	{
		char *buffer = new char[1000];
		ifstream file(init.getdata());

		file.getline(buffer, 1000);
		order = csToInt(buffer);
		file.getline(buffer, 1000);
		n = csToInt(buffer);

		file.getline(buffer, 2);
		if (buffer[0] == '1')
		{
			leaf = true;
		}
		else
		{
			leaf = false;
		}
		file.getline(buffer, 1000);
		parent = buffer;
		keys = new String[order];
		for (int i = 0; i < order; i++)
		{

			file.getline(buffer, 1000);
			keys[i] = buffer;
		}
		children = new String[order + 1];
		for (int i = 0; i <= order; i++)
		{
			file.getline(buffer, 1000);
			children[i] = buffer;
		}
		file.close();
	}
};

template <typename t>
class Btree
{
public:
	int order;
	int max_keys;	  // variable to stor the maximum amount of keys so we can split when limit reached   ceil(m/2-1)----(m-1)
	int max_children; // variable to stor the maximum amount of children so we can split when limit reached	ceil(m/2)---m
	String root;

	int key_line;	   // line number in file where keys are stored;
	int children_line; // line number in file where children are stored//
	int min_keys;
	String path = "./BTree/main/init/";
	String datapath = "./Files/";
	Btree(int o = 0, String r = "nullptr")
	{
		::curr_node = 1;
		order = o;
		max_keys = o - 1;
		max_children = o;
		root = r;

		key_line = 5;
		children_line = 5 + (order);

		min_keys = ceil(order / 2.0) - 1;
	}

	String getPathDirectory()
	{
		return path;
	}

	// function to sort the keys in incresing order
	void sort(t *&k, int n)
	{

		// insrtion sort
		for (int i = 1; i < n; ++i)
		{
			int key = k[i];
			int j = i - 1;

			/* Move elements of arr[0..i-1], that are
			   greater than key, to one position ahead
			   of their current position */
			while (j >= 0 && k[j] > key)
			{
				k[j + 1] = k[j];
				j = j - 1;
			}
			k[j + 1] = key;
		}
	}
	// function to split child in case num of keys exceeded
	void splitChild(Btree_node<t> &parent, String par_str, int i)
	{
		// we find the child node that needs to be split
		// Btree_node<t>* childToSplit = parent->children[i];
		Btree_node<t> *childToSplit = new Btree_node<t>(parent.children[i]);
		// we crete the new node
		Btree_node<t> *new_node = new Btree_node<t>(order);
		new_node->parent = childToSplit->parent;
		new_node->leaf = childToSplit->leaf;

		char temp[10];
		toString(curr_node, temp);
		String temp_str(temp);
		String newNode_str(temp_str);
		//  cout << "Newnode : " << newNode_str.getdata() << endl;
		newNode_str = newNode_str + "_node.txt";
		if (order % 2 != 0)
		{

			// we adjust the count of keys on new node
			// we shift half of the keys to the new node
			new_node->n = ceil((order / 2.0)) - 1;
			for (int j = 0; j < int(ceil((order / 2.0))) - 1; j++)
				new_node->keys[j] = childToSplit->keys[j + int(ceil((order / 2.0)))];
			// we then move the child pointers also if it is not a leaf node
			if (!childToSplit->leaf)
			{
				for (int j = 0; j < ceil((order / 2.0)); j++)
				{
					new_node->children[j] = childToSplit->children[j + int(ceil((order / 2.0)))];
					if (new_node->children[j] != "nullptr")
					{
						// if the children are not null we also change their parent pointer to the new node
						replaceLine(new_node->children[j], 4, newNode_str);
					}
				}
			}
			for (int j = ceil((order / 2.0)); j <= order; j++)
			{
				childToSplit->children[j] = "nullptr";
			}
			childToSplit->n = ceil((order / 2.0)) - 1;

			// we adjust the child pointers of the parent node to accomodate the new node
			for (int j = parent.n; j >= i + 1; j--)
				parent.children[j + 1] = parent.children[j];
			parent.children[i + 1] = newNode_str;

			// promote the middle node to the parent
			for (int j = parent.n - 1; j >= i; j--)
				parent.keys[j + 1] = parent.keys[j];
			parent.keys[i] = childToSplit->keys[int(ceil(order / 2.0)) - 1];

			// update parent key count
			parent.n = parent.n + 1;

			// after that we overwrite all the files with the vlaues
			// overwrite the parent file
			ofstream parent_node(par_str.getdata());
			parent_node << parent.order << '\n';
			parent_node << parent.n << '\n';
			parent_node << parent.leaf << '\n';
			parent_node << parent.parent << '\n';

			for (int i = 0; i < parent.order; i++)
			{
				if (parent.keys[i])
				{
					parent_node << parent.keys[i] << '\n';
				}
				else
				{
					parent_node << "null" << '\n';
				}
			}
			for (int i = 0; i <= parent.order; i++)
			{
				parent_node << parent.children[i] << '\n';
			}

			parent_node.close();

			// over write the splitted child
			ofstream split_child(parent.children[i].getdata());
			split_child << childToSplit->order << '\n';
			split_child << childToSplit->n << '\n';
			split_child << childToSplit->leaf << '\n';
			split_child << childToSplit->parent << '\n';

			for (int i = 0; i < childToSplit->order; i++)
			{
				if (childToSplit->keys[i])
					split_child << childToSplit->keys[i] << '\n';
				else
					split_child << "null" << '\n';
			}
			for (int i = 0; i <= childToSplit->order; i++)
			{
				split_child << childToSplit->children[i] << '\n';
			}

			// overwrite the new child
			curr_node++;
			ofstream new_child(newNode_str.getdata());
			new_child << new_node->order << '\n';
			new_child << new_node->n << '\n';
			new_child << new_node->leaf << '\n';
			new_child << childToSplit->parent << '\n';

			for (int i = 0; i < new_node->order; i++)
			{
				if (new_node->keys[i])
					new_child << new_node->keys[i] << '\n';
				else
					new_child << "null" << '\n';
			}
			for (int i = 0; i <= new_node->order; i++)
			{
				new_child << new_node->children[i] << '\n';
			}
		}
		else
		{
			// new_node->n = order / 2;
			// for (int j = 0; j < order / 2; j++)
			//	new_node->keys[j] = childToSplit->keys[j + order / 2];
			////we then move the child pointers also if it is not a leaf node
			// if (!childToSplit->leaf) {
			//	for (int j = 0; j < order / 2.0; j++)
			//		new_node->children[j] = childToSplit->children[j + order / 2];
			// }
			// childToSplit->n = order / 2 - 1;

			////we adjust the child pointers of the parent node to accomodate the new node
			// for (int j = parent->n; j >= i + 1; j--)
			//	parent->children[j + 1] = parent->children[j];
			// parent->children[i + 1] = new_node;

			////promote the middle node to the parent
			// for (int j = parent->n - 1; j >= i; j--)
			//	parent->keys[j + 1] = parent->keys[j];
			// parent->keys[i] = childToSplit->keys[(order / 2) - 1];

			////update parent key count
			// parent->n = parent->n + 1;

			// we adjust the count of keys on new node
			// we shift half of the keys to the new node
			new_node->n = order / 2;
			for (int j = 0; j < order / 2; j++)
				new_node->keys[j] = childToSplit->keys[j + order / 2];
			// we then move the child pointers also if it is not a leaf node
			if (!childToSplit->leaf)
			{
				for (int j = 0; j < order / 2; j++)
				{
					new_node->children[j] = childToSplit->children[j + order / 2];
					if (new_node->children[j] != "nullptr")
					{
						// if the children are not null we also change their parent pointer to the new node
						replaceLine(new_node->children[j], 4, newNode_str);
					}
				}
			}
			for (int j = order / 2; j <= order; j++)
			{
				childToSplit->children[j] = "nullptr";
			}
			childToSplit->n = order / 2 - 1;

			// we adjust the child pointers of the parent node to accomodate the new node
			for (int j = parent.n; j >= i + 1; j--)
				parent.children[j + 1] = parent.children[j];
			parent.children[i + 1] = newNode_str;

			// promote the middle node to the parent
			for (int j = parent.n - 1; j >= i; j--)
				parent.keys[j + 1] = parent.keys[j];
			parent.keys[i] = childToSplit->keys[(order / 2) - 1];

			// update parent key count
			parent.n = parent.n + 1;

			// after that we overwrite all the files with the vlaues
			// overwrite the parent file
			ofstream parent_node(par_str.getdata());
			parent_node << parent.order << '\n';
			parent_node << parent.n << '\n';
			parent_node << parent.leaf << '\n';
			parent_node << parent.parent << '\n';

			for (int i = 0; i < parent.order; i++)
			{
				if (parent.keys[i])
				{
					parent_node << parent.keys[i] << '\n';
				}
				else
				{
					parent_node << "null" << '\n';
				}
			}
			for (int i = 0; i <= parent.order; i++)
			{
				parent_node << parent.children[i] << '\n';
			}

			parent_node.close();

			// over write the splitted child
			ofstream split_child(parent.children[i].getdata());
			split_child << childToSplit->order << '\n';
			split_child << childToSplit->n << '\n';
			split_child << childToSplit->leaf << '\n';
			split_child << childToSplit->parent << '\n';

			for (int i = 0; i < childToSplit->order; i++)
			{
				if (childToSplit->keys[i])
					split_child << childToSplit->keys[i] << '\n';
				else
					split_child << "null" << '\n';
			}
			for (int i = 0; i <= childToSplit->order; i++)
			{
				split_child << childToSplit->children[i] << '\n';
			}

			// overwrite the new child
			curr_node++;
			ofstream new_child(newNode_str.getdata());
			new_child << new_node->order << '\n';
			new_child << new_node->n << '\n';
			new_child << new_node->leaf << '\n';
			new_child << childToSplit->parent << '\n';

			for (int i = 0; i < new_node->order; i++)
			{
				if (new_node->keys[i])
					new_child << new_node->keys[i] << '\n';
				else
					new_child << "null" << '\n';
			}
			for (int i = 0; i <= new_node->order; i++)
			{
				new_child << new_node->children[i] << '\n';
			}
		}
	}
	// void insertNonLeaf(t val, String &r, int col)
	// {
	// }
	void insertNonFull(t val, String &r, int col)
	{
		// Initialize the current node with file data
		Btree_node<String> rt(r);

		if (rt.leaf)
		{
			// If the node is a leaf, insert the value directly
			val = val - path;					 // Remove the base path from the value
			String curr(extract(val, col, ',')); // Extract the column value
			String col_val = path + curr;		 // Form the full path-based column value

			int i = rt.n - 1;

			// Shift keys to make room for the new key
			while (i >= 0 && col_val < (rt.keys[i] - ".txt"))
			{
				rt.keys[i + 1] = rt.keys[i];
				i--;
			}

			// Insert the new key into the correct position
			rt.keys[i + 1] = col_val + ".txt";
			String new_key = col_val + ".txt";

			// Create the new file for the key and write the value
			ofstream new_file(new_key.getdata());
			new_file << val << '\n';
			new_file.close();

			// Increment the key count in the node
			rt.n++;

			// Update the current node's file with the modified data
			r = path + r; // Ensure the node's file is accessed with the full path
			ofstream curr_nodeFile(r.getdata());
			curr_nodeFile << rt.order << '\n';
			curr_nodeFile << rt.n << '\n';
			curr_nodeFile << rt.leaf << '\n';
			curr_nodeFile << rt.parent << '\n';

			// Write all keys and children back to the file
			for (int i = 0; i < rt.order; i++)
			{
				curr_nodeFile << rt.keys[i] << '\n';
			}
			for (int i = 0; i <= rt.order; i++)
			{
				curr_nodeFile << rt.children[i] << '\n';
			}

			curr_nodeFile.close();
		}
		else
		{
			// For non-leaf nodes, find the child to recurse into
			int i = 0;
			for (; i < rt.n; i++)
			{
				if (val < rt.keys[i])
				{
					break;
				}
			}

			// Recurse into the selected child
			String child_path = path + rt.children[i]; // Ensure full path for the child
			insertNonFull(val, rt.children[i], col);   // Recursive call with proper path

			// Check if the child is now overfull
			if (csToInt(getLineFile(child_path, 2)) == max_keys + 1)
			{
				splitChild(rt, r, i); // Split the overfull child node
			}
		}
	}

	/*void insert(String val)
	{
		insert(val, this->root);
	}*/
	void insert(String val, String r, int col, String path)
	{

		// we also check if file already exists then we just append to that file
		val = path + val;
		cout << val << endl;
		String existing(extract(val, col, ','));
		String data = datapath + val;
		if (access(existing.getdata(), F_OK) == 0)
		{
			std::ofstream file(existing.getdata(), std::ios::app);
			file << val << '\n';
			file.close();
			return;
		}
		////we check if file already exists
		// std::ifstream file(existing.getdata());

		// if (!file) {

		//}
		// else {
		//	file.close()
		//	ofstream file(existing.getdata(), std::ios::app);
		//	file << val << '\n';
		//	file.close();
		//	return;
		//}

		if (r == "nullptr")
		{
			char temp[10];

			toString(curr_node, temp); // Convert the current node to a string
			String temparr(temp);
			String rt(temparr); // Convert the current node to a string
			curr_node++;

			// Construct the full path for the node file
			rt = rt + "_node.txt";
			rt = path + rt; // Append the base path

			// Create the root node file
			ofstream root(rt.getdata());
			this->root = rt;		   // Update the class's root reference
			root << order << '\n';	   // B-tree order
			root << 1 << '\n';		   // Number of keys
			root << 1 << '\n';		   // Leaf node flag
			root << "nullptr" << '\n'; // Parent reference (initially null)
			root << '\n';
			root << data << '\n';

			// Extract column value and construct its file path
			String col_val(extract(val, col, ',')); // Extract a part of the value based on the column
			col_val = col_val + ".txt";
			col_val = path + col_val; // Append the base path
			// cout << "Coll_val " << col_val << endl;

			// Create the file for the first key
			ofstream first_key(col_val.getdata());
			first_key << val << '\n';

			// Write the key's file path to the root node file
			root << col_val << '\n';

			// Fill the remaining key slots with "null"
			for (int i = 1; i < max_keys + 1; i++)
			{
				root << "nullptr" << '\n';
			}

			// Initialize child pointers as "nullptr"
			for (int i = 0; i < max_children + 1; i++)
			{
				root << "nullptr" << '\n';
			}

			return;
		}

		// if (getLineFile(r,3)=="1" && getLineFile(r,2) ==  toString(max_keys)) {
		//	/Btree_node<t> new_root = new Btree_node<t>(this->order);
		//	new_root->leaf = false;
		//	new_root->children[0] = r;
		//	r = new_root;*/
		//	String nr(toString(curr_node));
		//	nr = nr + ".txt";
		//	ofstream new_root(nr.getdata());
		//	new_root<< order << '\n';	//order
		//	new_root << 1 << '\n';		//num keys
		//	new_root << 0 << '\n';		//leaf
		//	new_root << "nullptr" << '\n';//parent

		//	//insertNonFull(val, new_root->children[0]);
		//	//splitChild(new_root, 0);
		//}
		else
		{
			Btree_node<String> rt(r);
			insertNonFull(val, r, col);

			// Check if the root node has more than max keys and needs splitting
			if (csToInt(getLineFile(r, 2)) == max_keys + 1)
			{
				// Assign the file to the new root node
				Btree_node<t> new_root(this->order);
				new_root.leaf = false;
				new_root.children[0] = r;
				new_root.parent = "nullptr";

				// Generate a new file name for the parent node
				char arr[10];
				String temparr;
				toString(curr_node, arr); // Convert the current node to a string
				temparr = arr;
				String new_parent(temparr);
				new_parent = new_parent + "_node.txt";
				new_parent = path + new_parent; // Append the base path

				// Update the parent reference of the current root
				rt.parent = new_parent;
				this->root = new_parent;
				replaceLine(r, 4, new_parent); // Update the parent reference in the current root file
				curr_node++;

				ofstream new_rootFile(new_parent.getdata());
				new_rootFile << new_root.order << '\n'; // Order of the B-tree
				new_rootFile << new_root.n << '\n';		// Number of keys
				new_rootFile << new_root.leaf << '\n';	// Leaf node flag
				new_rootFile << "nullptr" << '\n';		// Parent reference (null for the new root)
				new_rootFile << '\n';
				new_rootFile << data << '\n';
				for (int i = 0; i < order; i++)
				{
					new_rootFile << "nullptr" << '\n';
				}

				// Write the reference to the old root
				new_rootFile << r << '\n';

				for (int i = 0; i < order; i++)
				{
					new_rootFile << "nullptr" << '\n';
				}
				new_rootFile.close();

				// Split the child node and update the new root
				// cout << "new_root: " << new_root.children[0] << endl;
				// cout << "new_parent: " << new_parent << endl;
				splitChild(new_root, new_parent, 0);
			}
		}
	}

	// void traverse(Btree_node<t>* r)
	//{
	//	//level order traversal

	//	Queue<Btree_node<t>*> q;

	//	q.enqueue(r);

	//	while (!q.isEmpty())
	//	{
	//		Btree_node<t>* temp = q.dequeue();
	//		//first we print
	//		for (int i = 0;i < temp->n;i++)
	//		{
	//			cout << temp->keys[i] << " ";
	//		}
	//		//then we enque all the child pointers
	//		for (int i = 0;i < temp->order;i++)
	//		{
	//			if (temp->children[i] != nullptr)
	//			{
	//				q.enqueue(temp->children[i]);
	//			}
	//		}
	//	}

	//}
	void remove_node(String val, String &r, int ind) // ind is the paretn child number that we came from
	{
		Btree_node<String> rt(r);

		int i = 0;
		while (i < rt.n && (rt.keys[i] - path - ".txt") < val)
			i++;

		// if node is found we delete it
		if (val == rt.keys[i] - path - ".txt")
		{
			// if node is leaf

			if (rt.leaf)
			{
				// we just remove the value
				rt.keys[i] = "null";
				rt.n--;
				// shidt the keys
				for (int j = i; j < order - 2; j++)
				{
					rt.keys[j] = rt.keys[j + 1];
				}
				rt.keys[order - 2] = "null";

				ofstream root(r.getdata());
				root << rt.order << '\n';
				root << rt.n << '\n';
				root << rt.leaf << '\n';
				root << rt.parent << '\n';

				for (int i = 0; i < rt.order; i++)
				{
					root << rt.keys[i] << '\n';
				}
				for (int i = 0; i <= rt.order; i++)
				{
					root << rt.children[i] << '\n';
				}

				root.close();
			}
			else
			{
				// for non leaf node

				// we get thee successor
				String succ_node = rt.children[i + 1];
				cout << "Succ " << succ_node << endl;
				String keyToReplace = getLineFile(succ_node, 5); // we get teh first key of teh right node

				// we replace the current key with the successor
				replaceLine(r, 5 + i, keyToReplace);

				// we dlete from the subtree
				remove_node(keyToReplace - path - ".txt", succ_node, i + 1);
			}
		}
		// else we recurse to the appropriate child
		else
		{
			remove_node(val, rt.children[i], i);
		}

		// after deleteion we need to check if any restructuring is needed in our tree
		if (rt.n < min_keys && r != this->root)
		{
			// if the node has less than min keys we need to rebalance this
			//  first we need to chekc if the left or right nodes have the extra file
			Btree_node<String> parent(rt.parent.getdata());

			// if left child has more keys we borrow from it
			if (ind > 0 && csToInt(getLineFile(parent.children[ind - 1], 2)) > min_keys)
			{
				Btree_node<String> left_neighbour(parent.children[ind - 1]);
				// we borrow key from left neighbour
				// but first we assign left neihbour key to paretn and then the parent key to our dleeted node
				rt.keys[min_keys - 1] = parent.keys[ind - 1];
				rt.n++;

				// if we borrow from left we need to shift the child pointer and add the other borrowed pointr
				if (!rt.leaf)
				{
					for (int i = rt.n; i > 0; i--)
					{
						rt.children[i] = rt.children[i + 1];
					}
					rt.children[0] = left_neighbour.children[min_keys];
				}
				parent.keys[ind - 1] = left_neighbour.keys[left_neighbour.n - 1];

				// we also need to shift the keys
				for (int j = rt.n - 1; j > 0; j--)
				{
					left_neighbour.keys[j] = left_neighbour.keys[j - 1];
				}
				left_neighbour.n--;

				// after modifications we write back all the data into the file
				ofstream curr_root(r.getdata());
				curr_root << rt.order << '\n';
				curr_root << rt.n << '\n';
				curr_root << rt.leaf << '\n';
				curr_root << rt.parent << '\n';

				for (int i = 0; i < rt.order; i++)
				{
					curr_root << rt.keys[i] << '\n';
				}
				for (int i = 0; i <= rt.order; i++)
				{
					curr_root << rt.children[i] << '\n';
				}

				curr_root.close();

				ofstream parent_node(rt.parent.getdata());
				parent_node << parent.order << '\n';
				parent_node << parent.n << '\n';
				parent_node << parent.leaf << '\n';
				parent_node << parent.parent << '\n';

				for (int i = 0; i < parent.order; i++)
				{
					parent_node << parent.keys[i] << '\n';
				}
				for (int i = 0; i <= parent.order; i++)
				{
					parent_node << parent.children[i] << '\n';
				}

				parent_node.close();

				ofstream left_node(parent.children[ind - 1].getdata());
				left_node << left_neighbour.order << '\n';
				left_node << left_neighbour.n << '\n';
				left_node << left_neighbour.leaf << '\n';
				left_node << left_neighbour.parent << '\n';

				for (int i = 0; i < left_neighbour.order; i++)
				{
					left_node << left_neighbour.keys[i] << '\n';
				}
				for (int i = 0; i <= left_neighbour.order; i++)
				{
					left_node << left_neighbour.children[i] << '\n';
				}

				left_node.close();
			}
			// if right neighbour has keys we borrow from it
			else if (ind < order && csToInt(getLineFile(parent.children[ind + 1], 2)) > min_keys)
			{
				Btree_node<String> right_neighbour(parent.children[ind + 1]);
				// we borrow key from right neighbour
				// but first we assign right neihbour key to paretn and then the parent key to our dleeted node
				rt.keys[min_keys - 1] = parent.keys[ind];
				rt.n++;
				// we also need to get the pointer of that specific node
				if (!rt.leaf)
				{
					rt.children[min_keys] = right_neighbour.children[0];
				}
				parent.keys[ind] = right_neighbour.keys[0];
				// we also shift th ekeys of the right neighbour
				for (int j = 0; j < right_neighbour.n - 1; j++)
				{
					right_neighbour.keys[j] = right_neighbour.keys[j + 1];
				}
				right_neighbour.n--;

				// we need to laso adjuxt the pointer if it is non leaf node
				if (!rt.leaf)
				{
					rt.children[min_keys] = right_neighbour.children[0];
				}

				// after modifications we write back all the data into the file
				ofstream curr_root(r.getdata());
				curr_root << rt.order << '\n';
				curr_root << rt.n << '\n';
				curr_root << rt.leaf << '\n';
				curr_root << rt.parent << '\n';

				for (int i = 0; i < rt.order; i++)
				{
					curr_root << rt.keys[i] << '\n';
				}
				for (int i = 0; i <= rt.order; i++)
				{
					curr_root << rt.children[i] << '\n';
				}

				curr_root.close();

				ofstream parent_node(rt.parent.getdata());
				parent_node << parent.order << '\n';
				parent_node << parent.n << '\n';
				parent_node << parent.leaf << '\n';
				parent_node << parent.parent << '\n';

				for (int i = 0; i < parent.order; i++)
				{
					parent_node << parent.keys[i] << '\n';
				}
				for (int i = 0; i <= parent.order; i++)
				{
					parent_node << parent.children[i] << '\n';
				}

				parent_node.close();

				ofstream right_node(parent.children[ind + 1].getdata());
				right_node << right_neighbour.order << '\n';
				right_node << right_neighbour.n << '\n';
				right_node << right_neighbour.leaf << '\n';
				right_node << right_neighbour.parent << '\n';

				for (int i = 0; i < right_neighbour.order; i++)
				{
					right_node << right_neighbour.keys[i] << '\n';
				}
				for (int i = 0; i <= right_neighbour.order; i++)
				{
					right_node << right_neighbour.children[i] << '\n';
				}

				right_node.close();
			}
			else
			{
				// we merge the nodes together
				if (ind > 0)
				{
					// if node in which deletion occurs is not the left most, we merge with the left node
					Btree_node<String> left_neighbour(parent.children[ind - 1]);

					// first we add the parent to the left neighbor
					left_neighbour.keys[min_keys] = parent.keys[ind - 1];
					left_neighbour.n++;
					parent.n--;
					// then ew add the deficient nodes keys
					for (int j = 0; j < rt.n; j++)
					{
						left_neighbour.keys[min_keys + 1 + j] = rt.keys[j];
						left_neighbour.n++;
					}

					// we then shift parent keys
					for (int j = ind - 1; j < parent.n; j++)
					{
						parent.keys[j] = parent.keys[j + 1];
					}
					// then shidt the pointers
					for (int j = ind; j < order; j++)
					{
						parent.children[j] = parent.children[j + 1];
					}

					// we delete the extra file left

					// after removing ew update the file
					ofstream curr_root(r.getdata());
					curr_root << rt.order << '\n';
					curr_root << rt.n << '\n';
					curr_root << rt.leaf << '\n';
					curr_root << rt.parent << '\n';

					for (int i = 0; i < rt.order; i++)
					{
						curr_root << rt.keys[i] << '\n';
					}
					for (int i = 0; i <= rt.order; i++)
					{
						curr_root << rt.children[i] << '\n';
					}

					curr_root.close();

					ofstream parent_node(rt.parent.getdata());
					parent_node << parent.order << '\n';
					parent_node << parent.n << '\n';
					parent_node << parent.leaf << '\n';
					parent_node << parent.parent << '\n';

					for (int i = 0; i < parent.order; i++)
					{
						parent_node << parent.keys[i] << '\n';
					}
					for (int i = 0; i <= parent.order; i++)
					{
						parent_node << parent.children[i] << '\n';
					}

					parent_node.close();

					ofstream left_node(parent.children[ind - 1].getdata());
					left_node << left_neighbour.order << '\n';
					left_node << left_neighbour.n << '\n';
					left_node << left_neighbour.leaf << '\n';
					left_node << left_neighbour.parent << '\n';

					for (int i = 0; i < left_neighbour.order; i++)
					{
						left_node << left_neighbour.keys[i] << '\n';
					}
					for (int i = 0; i <= left_neighbour.order; i++)
					{
						left_node << left_neighbour.children[i] << '\n';
					}

					left_node.close();
				}
				else
				{
				}
			}
		}
	}
	void delete_node(String val, String &r)
	{
		if (r == "nullptr")
		{
			cout << "value does not exist";
			return;
		}
		// first we need to find the node
		Btree_node<String> rt(r);
		//	cout << r << endl;
		// cout << rt.keys[0] << endl;
		// cout << rt.n << endl;
		int i = 0;
		while (i < rt.n && (rt.keys[i] - path - ".txt") < val)
		{

			i++;
		}
		// if node is found we delete it
		if (val == (rt.keys[i] - path - ".txt"))
		{

			// if node is leaf
			if (rt.leaf)
			{
				// we just remove the value
				rt.keys[i] = "null";
				rt.n--;
				// shidt the keys
				for (int j = i; j < order - 2; j++)
				{
					rt.keys[j] = rt.keys[j + 1];
				}
				rt.keys[order - 2] = "null";

				// afterdeleting we write it to the file

				ofstream root(r.getdata());
				root << rt.order << '\n';
				root << rt.n << '\n';
				root << rt.leaf << '\n';
				root << rt.parent << '\n';

				for (int i = 0; i < rt.order; i++)
				{
					root << rt.keys[i] << '\n';
				}
				for (int i = 0; i <= rt.order; i++)
				{
					root << rt.children[i] << '\n';
				}

				root.close();
			}
			else
			{

				// for non leaf node
				// we get thee successor
				String succ_node = rt.children[i + 1];
				String keyToReplace = getLineFile(succ_node, 5); // we get teh first key of teh right node

				// we replace the current key with the successor
				replaceLine(r, 5 + i, keyToReplace);

				// we dlete from the subtree
				remove_node(keyToReplace - ".txt", succ_node, i + 1);
			}
		}
		// else we recurse to the appropriate child
		else
		{

			remove_node(val, rt.children[i], i);
		}

		// after deletion we come back to the root
		// now e need to check the keys of root

		// root can have min 1 key if it has 0 keys we nned to delete the root
		String curr_root(r); // save the current root filename to delete later
		if (csToInt(getLineFile(r, 2)) == 0)
		{
			// we change the tree root to teh child node
			this->root = getLineFile(r, children_line);

			// then we change the parent of teh child node (new root) to nullptr
			replaceLine(this->root, 4, "nullptr");

			// we then delete the file of initial root
			std::remove(curr_root.getdata());
		}
	}
	// String traverse(String &r)
	// {
	// 	// we use the quesu structure for level order traversal
	// 	Queue<String> q;
	// 	// Btree_node<String> rt(r);

	// 	// we also create a file to sotre all thee nodes
	// 	ofstream file("nodes.txt");
	// 	// first we enques the root
	// 	q.enqueue(r);
	// 	while (!q.isEmpty())
	// 	{
	// 		Btree_node<String> rt(q.dequeue());

	// 		// first we print the keys
	// 		cout << '[';
	// 		for (int i = 0; i < rt.n; i++)
	// 		{
	// 			if (rt.keys[i])
	// 			{
	// 				// cout << rt.keys[i] - ".txt" << ' ';
	// 				file << rt.keys[i] << '\n';
	// 			}
	// 		}

	// 		// then ew enque the child pointers
	// 		for (int i = 0; i < order; i++)
	// 		{
	// 			if (rt.children[i] != "nullptr")
	// 				q.enqueue(rt.children[i]);
	// 		}
	// 	}
	// 	file.close();
	// 	return "node.txt";
	// }
	// void display(String &r)
	// {
	// 	// we use the quesu structure for level order traversal
	// 	Queue<String> q;
	// 	// Btree_node<String> rt(r);
	// 	//
	// 	// first we enques the root
	// 	q.enqueue(r);
	// 	while (!q.isEmpty())
	// 	{
	// 		Btree_node<String> rt(q.dequeue());

	// 		// first we print the keys
	// 		cout << '[';
	// 		for (int i = 0; i < rt.n; i++)
	// 		{
	// 			if (rt.keys[i])
	// 			{
	// 				cout << rt.keys[i] - ".txt" << ' ';
	// 			}
	// 			else
	// 			{
	// 				cout << "null" << ' ';
	// 			}
	// 		}
	// 		cout << ']';
	// 		cout << ' ';

	// 		// then ew enque the child pointers
	// 		for (int i = 0; i < order; i++)
	// 		{
	// 			if (rt.children[i] != "nullptr")
	// 				q.enqueue(rt.children[i]);
	// 		}
	// 	}
	// }

	// void addBranch()
	// {

	// 	writeBranches(this->root);
	// 	cout << "Enter the branch name: ";
	// 	String branchName;
	// 	cin >> branchName;

	// 	String sourceDir = "./RedBlack/main/init/";
	// 	String always = "./RedBlack/";
	// 	String targetDir = always + branchName;
	// 	String totalPath = targetDir + '/';
	// 	cout << totalPath << endl;
	// 	mkdir(totalPath.getdata(), 0777);
	// 	RedBlackTree newTree;
	// 	totalPath = totalPath + "init/";
	// 	this->path = totalPath;

	// 	mkdir(totalPath.getdata(), 0777);
	// 	copyTree(*this, newTree, root, sourceDir, totalPath);
	// 	String curr = newTree.getRoot();
	// 	writeBranches(curr);
	// }

	// void copyTree(Btree<String> &oldTree, RedBlackTree &newTree, String &curr, String &sourceDir, String &targetDir)
	// {
	// 	Btree_node<String> rt(curr);
	// 	// first we enques the root
	// 	Queue<String> q;
	// 	q.enqueue(curr);
	// 	while (!q.isEmpty())
	// 	{
	// 		Btree_node<String> rt(q.dequeue());
	// 		// first we print the keys
	// 		for (int i = 0; i < rt.n; i++)
	// 		{
	// 			if (rt.keys[i])
	// 			{
	// 				String key = rt.keys[i];
	// 				String sourceFile = sourceDir + key;
	// 				String targetFile = targetDir + key;
	// 				if (access(sourceFile.getdata(), F_OK) == 0)
	// 				{
	// 					ifstream source(sourceFile.getdata(), ios::binary);
	// 					ofstream target(targetFile.getdata(), ios::binary);
	// 					target << source.rdbuf();
	// 					source.close();
	// 					target.close();
	// 				}
	// 			}
	// 		}
	// 		// then ew enque the child pointers
	// 		for (int i = 0; i < order; i++)
	// 		{
	// 			if (rt.children[i] != "nullptr")
	// 			{
	// 				q.enqueue(rt.children[i]);
	// 			}
	// 		}
	// 	}
	// }

	void writeBranches()
	{

		String branch = "./Branches/btree_branches.txt";

		ofstream file(branch.getdata(), ios::app);
		file << path << '\n';
		file.close();
	}

	void writeBranches(String &r)
	{
		String branch = "./Branches/btree_branches.txt";

		ofstream file(branch.getdata(), ios::app);
		file << r << '\n';
		file.close();
	}

	// String getPathDirectory()
	// {
	// 	return pathDirectory;
	// }

	// void setPathDirectory(String &path)
	// {
	// 	pathDirectory = path;
	// }

	void setRoot(String &r)
	{
		root = r;
	}

	// void SwitchBranches()
	// {
	// 	int resultCount = 0;
	// 	// showBranches(resultCount);
	// 	cout << "Enter the branch number (1- " << resultCount << ") : ";
	// 	int number;
	// 	cin >> number;

	// 	String curr = getLineFile("Branches/rb_branch", number);

	// 	this->root = curr;
	// 	cout << root << endl;

	// 	// this->inOrderTraversal(this->root.getdata());

	// 	cout << "Switched to branch " << number << endl;
	// }

	// void showBranches(int &resultCount)
	// {
	// 	String branch = "./Branches/btree_branches.txt";
	// 	ifstream file(branch.getdata());
	// 	String line;
	// 	while (getline(file, line))
	// 	{
	// 		resultCount++;
	// 		cout << resultCount << ". " << line << endl;
	// 	}
	// 	file.close();
	// }

	// void deleteBranch()
	// {
	// 	int resultCount = 0;
	// 	showBranches(resultCount);
	// 	cout << "Enter the branch number (1- " << resultCount << ") : ";
	// 	int number;
	// 	cin >> number;

	// 	String curr = getLineFile("Branches/btree_branches", number);
	// 	std::remove(curr.getdata());
	// 	cout << "Branch " << number << " deleted" << endl;
	// }

	void inOrderTraversal(String &r)
	{
		Btree_node<String> rt(r);
		for (int i = 0; i < rt.n; i++)
		{
			if (rt.children[i] != "nullptr")
			{
				inOrderTraversal(rt.children[i]);
			}
			cout << rt.keys[i] << " ";
		}
		if (rt.children[rt.n] != "nullptr")
		{
			inOrderTraversal(rt.children[rt.n]);
		}
	}
};

// int main()
// {

// 	Btree<String> b(3);
// 	String path = b.getPathDirectory();
// 	b.insert("1", b.root, 1, path);
// 	b.insert("4", b.root, 1, path);
// 	b.insert("7", b.root, 1, path);
// 	b.insert("10", b.root, 1, path);
// 	b.insert("13", b.root, 1, path);

// 	// b.delete_node("4", b.root);
// }
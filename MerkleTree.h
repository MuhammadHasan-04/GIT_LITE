#include <iostream>
#include "String.h"
#include "Utility_functions.h"
#include "queue.h"
// #include "Btree.h"
#include "Avl.h"
#include "RedBlack.h"

#include <fstream>
#include <openssl/sha.h>
#include <iomanip>

using namespace std;
int curr_hash = 0;

String AVL_traverse(String &r)
{
    if (r == "nullptr")
    {
        return "nulllptr";
    }

    ofstream file("nodes.txt");
    // we do level order traversel of tree
    Queue<String> q;
    q.enqueue(r);

    while (!q.isEmpty())
    {
        String temp(q.dequeue());
        // we add the temp to the nodes fil;
        file << getLineFile(temp, 6) << '\n';

        String left(getLineFile(temp, 2));
        String right(getLineFile(temp, 3));
        // if left or right child exist we add them to the que
        if (left != "nullptr")
        {
            q.enqueue(left);
        }
        if (right != "nullptr")
        {
            q.enqueue(right);
        }
    }

    return "nodes.txt";
}
String RB_traverse(String &r)
{
    if (r == "nullptr")
    {
        return "nulllptr";
    }

    ofstream file("nodes.txt");
    // we do level order traversel of tree
    Queue<String> q;
    q.enqueue(r);

    while (!q.isEmpty())
    {
        String temp(q.dequeue());
        // we add the temp to the nodes fil;
        file << getLineFile(temp, 6) << '\n';

        String left(getLineFile(temp, 2));
        String right(getLineFile(temp, 3));
        // if left or right child exist we add them to the que
        if (left != "nullptr")
        {
            q.enqueue(left);
        }
        if (right != "nullptr")
        {
            q.enqueue(right);
        }
    }

    return "nodes.txt";
}
String sha256(String &data)
{
    unsigned char hash[SHA256_DIGEST_LENGTH]; // Array to store the hash

    // Use the custom String class's c_str() method
    // Reinterpret char* as const unsigned char* since SHA256 expects that
    SHA256(reinterpret_cast<const unsigned char *>(data.getdata()), data.getLength(), hash); // Compute the hash

    String hashStr;

    // Iterate through the hash bytes and convert them to a hex string
    for (unsigned char byte : hash)
    {
        // Convert high nibble (byte >> 4) and low nibble (byte & 0x0F) to hex characters
        hashStr = hashStr + "0123456789abcdef"[byte >> 4];   // High nibble
        hashStr = hashStr + "0123456789abcdef"[byte & 0x0F]; // Low nibble
    }

    // Return the custom String with the generated hex string
    return hashStr;
}
String generate_hash(String file)
{
    ifstream node(file.getdata());
    char *buffer = new char[1000];
    node.getline(buffer, 1000);
    // we read from the files and store evrything in a sigle string
    String concatenated_string(buffer);
    while (node.getline(buffer, 1000))
    {
        String buffer_temp(buffer);
        concatenated_string = concatenated_string + buffer_temp;
    }

    return sha256(concatenated_string);
}
class merkleTree
{
public:
    String root;

    String buildMerkleTree(String &root, int treeChoice, int order = 0)
    {
        curr_hash = 0;
        String level;
        // we first traverse the trees and add all the nodes to the file
        if (treeChoice == 3)
        {
            // avl case
            AVL_tree<String> t;
            t.root = root;

            level = AVL_traverse(root);
        }
        else if (treeChoice == 2)
        {

            // btree case
            // Btree<String> t(order);
            // t.root = root;
            // level = (t.traverse(t.root));
        }
        else if (treeChoice == 1)
        {

            // rbtree case
            RedBlackTree t;
            t.getRoot() = root;
            level = RB_traverse(root);
        }

        // file with all the nodes
        ifstream node_file("nodes.txt");
        if (node_file)
        {
            cout << "completed";
        }
        char *buffer = new char[500];

        // create a new file with all the hashes
        ofstream hashes_file("hashes.txt");

        int count = 0;
        while (node_file.getline(buffer, 500))
        {

            // we hash each node
            String temp(buffer);
            String hash(generate_hash(temp));

            // we create the hasehd node
            temp = temp - ".txt";
            temp = temp + "_h.txt";
            // we add the hashes node to the list of hashes
            hashes_file << temp << '\n';
            // we create the hashed node
            ofstream hash_node(temp.getdata());
            hash_node << hash << '\n';
            hash_node << "nullptr" << '\n'; // left child
            hash_node << "nullptr" << '\n'; // right child
            hash_node << "nullptr" << '\n'; // parent

            count++;
        }
        hashes_file.close();

        // after initial hashes we build the tree
        while (count != 1 && count > 0) //(getLineFile("hashes.txt", 2)!="")
        {
            ofstream temp_file("temp_hashes.txt");
            ifstream hashes_file("hashes.txt");

            while (hashes_file.getline(buffer, 500))
            {

                // we then find the combined hashes of two node

                // we load 2 hashes
                String temp1(buffer);
                String temp2;
                if (hashes_file.getline(buffer, 500))
                {
                    temp2 = buffer;
                }
                else
                {
                    temp2 = temp1;
                }
                String hash1(getLineFile(temp1, 1));
                String hash2(getLineFile(temp2, 1));

                // we generate the combined hash
                String new_hash(generate_hash(hash1 + hash2));

                // we then create a new parent hash node of the two hashes

                String parent(temp1);
                parent = parent - ".txt";
                parent = parent + "-";
                parent = parent + temp2;
                // parent = parent + ".txt";

                ofstream parent_node(parent.getdata());
                parent_node << new_hash << '\n';
                parent_node << temp1 << '\n';
                parent_node << temp2 << '\n';
                parent_node << "nullptr" << '\n';

                parent_node.close();
                temp_file << parent << '\n';
                // we also update the parent pointers of the child hashes
                replaceLine(temp1, 4, parent);
                replaceLine(temp2, 4, parent);
            }
            temp_file.close();
            hashes_file.close();
            // after that we copy back the tmep into the hashes
            ifstream t_file("temp_hashes.txt");
            ofstream hashesFile("hashes.txt");
            count = 0;
            while (t_file.getline(buffer, 500))
            {
                this->root = buffer;
                hashesFile << buffer << '\n';
                count++;
            }
            t_file.close();
            hashes_file.close();
        }

        // we need to return the root node of our merkel tree
        return this->root;
    }

    bool compareTree(String &t1, String &t2)
    {
        String hash1 = getLineFile(t1, 1);
        String hash2 = getLineFile(t2, 1);

        return hash1 == hash2;
    }

    String compareFiles(String &t1, String &t2)
    {
        String hash1 = getLineFile(t1, 1);
        String hash2 = getLineFile(t2, 1);
        String changes_file("changes.txt");
        if (hash1 == hash2)
        {
            return "nullptr";
        }
        else
        {
            // we do level order traversal of both trees to find the nodes with chnages
            Queue<String> q1;
            Queue<String> q2;

            // first we add both roots to the ques
            q1.enqueue(t1);
            q2.enqueue(t2);

            while (!q1.isEmpty() && !q2.isEmpty())
            {
                String temp1 = q1.dequeue();
                String temp2 = q2.dequeue();

                String hash1 = getLineFile(temp1, 1);
                String hash2 = getLineFile(temp2, 1);

                String l1(getLineFile(temp1, 2));
                String r1(getLineFile(temp1, 3));

                String l2(getLineFile(temp2, 2));
                String r2(getLineFile(temp2, 3));
                // after dequing we compare the hashes
                // if they are equal we skip that branch else we enque the children

                if (hash1 != hash2)
                {
                    // if any of the children are null means they are chnaged so we append too the chanegs fiel
                    if (l1 == "nullptr")
                    {
                        // this means the node is leaf

                        changes_file << temp1 << '\n';
                    }
                    else
                    {
                        q1.enqueue(getLineFile(temp1, 2));
                        q1.enqueue(getLineFile(temp1, 3));
                    }
                    if (l2 == "nullptr")
                    {
                        // this means the node is leaf

                        changes_file << temp2 << '\n';
                    }
                    else
                    {
                        q2.enqueue(getLineFile(temp2, 2));
                        q2.enqueue(getLineFile(temp2, 3));
                    }
                }
            }

            // we check if either one que is empty and teh other is not

            if (q1.isEmpty() && !q2.isEmpty())
            {
                // we add the extra to the file
                while (!q2.isEmpty())
                {
                    String temp(q2.dequeue());
                    changes_file << temp << '\n';
                }
            }
            else if (!q1.isEmpty())
            {
                while (!q2.isEmpty())
                {
                    String temp(q1.dequeue());
                    changes_file << temp << '\n';
                }
            }
        }

        return changes_file;
    }
};
#include <iostream>
#include <fstream>
#include "String.h"
#include <sys/stat.h>
// #include <unistd.h>
//  #include <windows.h>
#include "Utility_functions.h"
using namespace std;

class RedBlackTree
{
private:
    String root;
    String pathDirectory;
    String filesPath;
    int commits;

public:
    RedBlackTree() : root("nullptr")
    {
        pathDirectory = "./RedBlack/main/";
        filesPath = "./Files/";
        commits = 0;
    }

    String getRoot() const { return root; }

    void leftRotate(String nodeFile)
    {
        String rightChild = getRightChild(nodeFile);
        if (rightChild == "nullptr")
            return;

        String rightLeftChild = getLeftChild(rightChild);
        String parent = getLineFile(nodeFile, 4);

        // Update parent
        if (parent != "nullptr")
        {
            if (getLeftChild(parent) == nodeFile)
                replaceLine(parent, 2, rightChild);
            else
                replaceLine(parent, 3, rightChild);
        }
        else
        {
            root = rightChild; // Update root if node is the root
        }

        // Update right child's parent
        replaceLine(rightChild, 4, parent);

        // Update node's parent to right child
        replaceLine(nodeFile, 4, rightChild);

        // Update right child's left child to current node
        replaceLine(rightChild, 2, nodeFile);

        // Update current node's right child
        replaceLine(nodeFile, 3, rightLeftChild);

        if (rightLeftChild != "nullptr")
        {
            replaceLine(rightLeftChild, 4, nodeFile);
        }
    }
    String getDataPath(String Node)
    {
        return getLineFile(Node, 6);
    }
    void rightRotate(String nodeFile)
    {
        String leftChild = getLeftChild(nodeFile);
        if (leftChild == "nullptr")
            return;

        String leftRightChild = getRightChild(leftChild);
        String parent = getLineFile(nodeFile, 4);

        // Update parent
        if (parent != "nullptr")
        {
            if (getLeftChild(parent) == nodeFile)
                replaceLine(parent, 2, leftChild);
            else
                replaceLine(parent, 3, leftChild);
        }
        else
        {
            root = leftChild; // Update root if node is the root
        }

        // Update left child's parent
        replaceLine(leftChild, 4, parent);

        // Update node's parent to left child
        replaceLine(nodeFile, 4, leftChild);

        // Update left child's right child to current node
        replaceLine(leftChild, 3, nodeFile);

        // Update current node's left child
        replaceLine(nodeFile, 2, leftRightChild);

        if (leftRightChild != "nullptr")
        {
            replaceLine(leftRightChild, 4, nodeFile);
        }
    }
    String returnFilePaths(RedBlackTree root)
    {
        if (root.getRoot() == "nullptr")
        {
            return "nullptr";
        }

        String left = getLeftChild(root.getRoot());

        String right = getRightChild(root.getRoot());

        return getDataPath(root.getRoot());
    }
    void Insert(String val, String parentFile, int col, String path1)
    {
        String key = val;
        String tempfile = key + ".txt";
        String data = filesPath + tempfile;
        String path = path1;

        String newNodeFile = path + key;
        newNodeFile = newNodeFile + ".txt";

        if (root == "nullptr")
        { // Root creation
            ofstream newNode(newNodeFile.getdata());
            newNode << newNodeFile << '\n'; // Value
            newNode << "nullptr" << '\n';   // left child
            newNode << "nullptr" << '\n';   // right child
            newNode << "nullptr" << '\n';   // parent
            newNode << "BLACK" << '\n';     // Root is always black
            newNode << data << '\n';        // the file containing the data is stored here
            newNode.close();
            root = newNodeFile;

            return;
        }

        // Insert logic
        String currentNodeFile = root;
        String insertedNode;
        while (true)
        {
            String currentValue = getLineFile(currentNodeFile, 1);
            String currentKey = extractOnlyTxt(currentValue);

            if (key == currentKey)
            {

                return;
            }

            else if (key < currentKey)
            {
                String leftChild = getLeftChild(currentNodeFile);
                if (leftChild == "nullptr")
                {
                    // Create new node
                    ofstream newNode(newNodeFile.getdata());
                    newNode << newNodeFile << '\n';
                    newNode << "nullptr" << '\n';
                    newNode << "nullptr" << '\n';
                    newNode << currentNodeFile << '\n';
                    newNode << "RED" << '\n';
                    newNode << data << '\n'; // the file containing the data is stored here
                    newNode.close();

                    replaceLine(currentNodeFile, 2, newNodeFile);
                    insertedNode = newNodeFile;
                    break;
                }
                currentNodeFile = leftChild;
            }
            else if (key > currentKey)
            {
                String rightChild = getRightChild(currentNodeFile);
                if (rightChild == "nullptr")
                {
                    // Create new node
                    ofstream newNode(newNodeFile.getdata());
                    newNode << newNodeFile << '\n';
                    newNode << "nullptr" << '\n';
                    newNode << "nullptr" << '\n';
                    newNode << currentNodeFile << '\n';
                    newNode << "RED" << '\n';
                    newNode << data << '\n'; // the file containing the data is stored here
                    newNode.close();

                    replaceLine(currentNodeFile, 3, newNodeFile);
                    insertedNode = newNodeFile;
                    break;
                }
                currentNodeFile = rightChild;
            }
            else
            {
                return; // Node already exists
            }
        }

        String newNodeParent = getLineFile(newNodeFile, 4);
        while (newNodeParent != "nullptr" && getNodeColor(newNodeParent) == "RED")
        {
            String grandParent = getLineFile(newNodeParent, 4);
            if (getLeftChild(grandParent) == newNodeParent)
            {
                String uncle = getRightChild(grandParent);
                if (getNodeColor(uncle) == "RED")
                {
                    replaceLine(newNodeParent, 5, "BLACK");
                    replaceLine(uncle, 5, "BLACK");
                    replaceLine(grandParent, 5, "RED");
                    newNodeFile = grandParent;
                }
                else
                {
                    if (getRightChild(newNodeParent) == newNodeFile)
                    {
                        leftRotate(newNodeParent);
                        newNodeFile = newNodeParent;
                        newNodeParent = getLineFile(newNodeFile, 4);
                    }
                    replaceLine(newNodeParent, 5, "BLACK");
                    replaceLine(grandParent, 5, "RED");
                    rightRotate(grandParent);
                }
            }
            else
            {
                String uncle = getLeftChild(grandParent);
                if (getNodeColor(uncle) == "RED")
                {
                    replaceLine(newNodeParent, 5, "BLACK");
                    replaceLine(uncle, 5, "BLACK");
                    replaceLine(grandParent, 5, "RED");
                    newNodeFile = grandParent;
                }
                else
                {
                    if (getLeftChild(newNodeParent) == newNodeFile)
                    {
                        rightRotate(newNodeParent);
                        newNodeFile = newNodeParent;
                        newNodeParent = getLineFile(newNodeFile, 4);
                    }
                    replaceLine(newNodeParent, 5, "BLACK");
                    replaceLine(grandParent, 5, "RED");
                    leftRotate(grandParent);
                }
            }
        }
        replaceLine(root, 5, "BLACK");
    }

    // void Insert(String val, String parentFile, int col, String path1)
    // {
    //     String key = val;
    //     String tempfile = key + ".txt";
    //     String data = filesPath + tempfile;
    //     String path = path1;

    //     String newNodeFile = path + key;
    //     newNodeFile = newNodeFile + ".txt";
    //     // String tempFile = "./RbTree/main/" + newNodeFile;

    //     if (root == "nullptr")
    //     { // Root creation
    //         ofstream newNode(newNodeFile.getdata());
    //         newNode << newNodeFile << '\n'; // Value
    //         newNode << "nullptr" << '\n';   // Left child
    //         newNode << "nullptr" << '\n';   // Right child
    //         newNode << "nullptr" << '\n';   // Parent
    //         newNode << "BLACK" << '\n';     // Root is always black
    //         newNode << data << '\n';        // the file containing the data is stored here
    //         newNode.close();
    //         root = newNodeFile;

    //         return;
    //     }

    //     // Insert logic
    //     String currentNodeFile = root;
    //     while (true)
    //     {
    //         String currentValue = getLineFile(currentNodeFile, 1);
    //         String currentKey = currentValue;

    //         if (key == currentKey)
    //         {

    //             return;
    //         }

    //         if (key < currentKey)
    //         {
    //             String leftChild = getLeftChild(currentNodeFile);
    //             if (leftChild == "nullptr")
    //             {
    //                 // Check for consecutive duplicates
    //                 if (currentKey == key)
    //                 {
    //                     // Handle consecutive duplicate (do not create a new node)
    //                     return;
    //                 }

    //                 // Create new node
    //                 ofstream newNode(newNodeFile.getdata());
    //                 newNode << newNodeFile << '\n';
    //                 newNode << "nullptr" << '\n';
    //                 newNode << "nullptr" << '\n';
    //                 newNode << currentNodeFile << '\n';
    //                 newNode << "RED" << '\n'; // input is always red
    //                 newNode << data << '\n';
    //                 newNode.close();

    //                 replaceLine(currentNodeFile, 2, newNodeFile);
    //                 break;
    //             }
    //             currentNodeFile = leftChild;
    //         }
    //         else if (key > currentKey)
    //         {
    //             String rightChild = getRightChild(currentNodeFile);
    //             if (rightChild == "nullptr")
    //             {
    //                 // Check for consecutive duplicates
    //                 if (currentKey == key)
    //                 {
    //                     // Handle consecutive duplicate (do not create a new node)
    //                     return;
    //                 }

    //                 // Create new node
    //                 ofstream newNode(newNodeFile.getdata());
    //                 newNode << newNodeFile << '\n';
    //                 newNode << "nullptr" << '\n';
    //                 newNode << "nullptr" << '\n';
    //                 newNode << currentNodeFile << '\n';
    //                 newNode << "RED" << '\n';
    //                 newNode << data << '\n';
    //                 newNode.close();

    //                 replaceLine(currentNodeFile, 3, newNodeFile);
    //                 break;
    //             }
    //             currentNodeFile = rightChild;
    //         }
    //         else
    //         {
    //             // Key already exists, do not create a new node
    //             return;
    //         }
    //     }

    //     // Fix Red-Black Tree violations
    //     String newNodeParent = getLineFile(newNodeFile, 4);
    //     while (newNodeParent != "nullptr" && getNodeColor(newNodeParent) == "RED")
    //     {
    //         String grandParent = getLineFile(newNodeParent, 4);
    //         if (getLeftChild(grandParent) == newNodeParent)
    //         {
    //             String uncle = getRightChild(grandParent);
    //             if (getNodeColor(uncle) == "RED")
    //             {
    //                 replaceLine(newNodeParent, 5, "BLACK");
    //                 replaceLine(uncle, 5, "BLACK");
    //                 replaceLine(grandParent, 5, "RED");
    //                 newNodeFile = grandParent;
    //             }
    //             else
    //             {
    //                 if (getRightChild(newNodeParent) == newNodeFile)
    //                 {
    //                     leftRotate(newNodeParent);
    //                     newNodeFile = newNodeParent;
    //                     newNodeParent = getLineFile(newNodeFile, 4);
    //                 }
    //                 replaceLine(newNodeParent, 5, "BLACK");
    //                 replaceLine(grandParent, 5, "RED");
    //                 rightRotate(grandParent);
    //             }
    //         }
    //         else
    //         {
    //             String uncle = getLeftChild(grandParent);
    //             if (getNodeColor(uncle) == "RED")
    //             {
    //                 replaceLine(newNodeParent, 5, "BLACK");
    //                 replaceLine(uncle, 5, "BLACK");
    //                 replaceLine(grandParent, 5, "RED");
    //                 newNodeFile = grandParent;
    //             }
    //             else
    //             {
    //                 if (getLeftChild(newNodeParent) == newNodeFile)
    //                 {
    //                     rightRotate(newNodeParent);
    //                     newNodeFile = newNodeParent;
    //                     newNodeParent = getLineFile(newNodeFile, 4);
    //                 }
    //                 replaceLine(newNodeParent, 5, "BLACK");
    //                 replaceLine(grandParent, 5, "RED");
    //                 leftRotate(grandParent);
    //             }
    //         }
    //     }
    //     replaceLine(root, 5, "BLACK");
    // }

    void writeBranches(String &name)
    {
        ofstream file("./Branches/rb_branch", ios::app);
        file << name << '\n';

        file.close();
    }

    void copyTree(RedBlackTree &soruce, RedBlackTree &target, String currentNode, String sourceDir, String targetDir)
    {
        if (currentNode == "nullptr")
            return;

        String leftChild = soruce.getLeftChild(currentNode);
        copyTree(soruce, target, leftChild, sourceDir, targetDir);
        String rightChild = soruce.getRightChild(currentNode);
        copyTree(soruce, target, rightChild, sourceDir, targetDir);

        target.Insert(extractOnlyTxt(currentNode), target.getRoot(), 1, targetDir);
    }
    String extractOnlyTxt(String &fileName)
    {
        int count = 0;
        String temp;
        for (int i = 0; i < fileName.getLength(); i++)
        {
            if (fileName[i] == '/')
            {
                count++;
                if (count == 4)
                {

                    for (int j = i + 1; j < fileName.getLength(); j++)
                    {
                        if (fileName[j] == '.' || fileName[j] == '/')
                        {
                            break;
                        }
                        temp = temp + fileName[j];
                    }
                    break;
                }
            }
        }
        return temp;
    }

    void dislayLog()
    {
        String curr = pathDirectory;
        char temp[10];
        toString(commits, temp);
        String total(temp);
        curr = curr + "Commit";
        curr = curr + total;

        curr = curr + "/log.txt";
        cout << curr << endl;
        ifstream file(curr.getdata());

        char buffer[256];

        while (file.getline(buffer, sizeof(buffer)))
        {
            String curr(buffer);
            cout << curr << endl;
        }
    }

    void DisplayCurrentBracnh()
    {

        // red the file
        ifstream file("./Branches/rb_branch", ios::in);
        char buffer[102];
        int i = 1;
        while (file.getline(buffer, sizeof(buffer)))
        {
            String curr(buffer);
            if (curr == root)
            {
                extarctBranchName(curr);
                cout << "Current Branch: " << curr << endl;
                break;
            }
            i++;
        }
    }
    void addBranch()
    {

        writeBranches(this->root);
        cout << "Enter the branch name: ";
        String branchName;
        cin >> branchName;

        String sourceDir = "./RedBlack/main/init/";
        String always = "./RedBlack/";
        String targetDir = always + branchName;
        String totalPath = targetDir + '/';
        cout << totalPath << endl;
        mkdir(totalPath.getdata(), 0777);
        RedBlackTree newTree;
        totalPath = totalPath + "init/";
        pathDirectory = totalPath;

        mkdir(totalPath.getdata(), 0777);
        copyTree(*this, newTree, root, sourceDir, totalPath);
        String curr = newTree.getRoot();
        writeBranches(curr);
    }

    String getCurrentPathDirectory()
    {
        return pathDirectory;
    }

    void SwitchBranches()
    {
        int resultCount = 0;
        showBranches(resultCount);
        cout << "Enter the branch number (1- " << resultCount << ") : ";
        int number;
        cin >> number;

        String curr = getLineFile("Branches/rb_branch", number);

        this->root = curr;
        // cout << root << endl;

        // this->inOrderTraversal(this->root.getdata());

        cout << "Switched to branch " << number << endl;
    }
    void ShowRedBlackBranches()
    {

        ifstream file("./Branches/rb_branch", ios::in);

        char buffer[256];
        int i = 1;
        while (file.getline(buffer, sizeof(buffer)))
        {
            String curr(buffer);
            extarctBranchName(curr);

            cout << i << ": " << curr << endl;
            i++;
        }

        file.close();
    }
    void showBranches(int &count)
    {
        ifstream file("./Branches/rb_branch", ios::in);

        char buffer[256];

        int i = 1;
        ShowRedBlackBranches();
        while (file.getline(buffer, sizeof(buffer)))
        {
            String curr(buffer);
            extarctBranchName(curr);
            // cout << i << ": " << curr << endl;
            count++;
            i++;
        }

        file.close();
    }
    void deleteBranch()
    {
        int resultCount = 0;
        // showBranches(resultCount);
        ShowRedBlackBranches();
        cout << "Enter the branch number (1- " << resultCount << ") : ";
        int number;
        cin >> number;

        String curr = getLineFile("Branches/rb_branch", number);
        // removeBranch(curr);
        removeOneBranch(curr, number);
    }
    void removeBranch(String &name)
    {
        String curr = "./RedBlack/";
        curr = curr + name;
        curr = curr + "/";
        remove(curr.getdata());
    }
    void removeOneBranch(String &name, int input)
    {
        replaceLine("Branches/rb_branch", input, "nullptr");
    }

    String getLeftChild(String node)
    {
        return getLineFile(node, 2);
    }

    String getRightChild(String node)
    {
        return getLineFile(node, 3);
    }

    String getNodeColor(String node)
    {
        return getLineFile(node, 5);
    }

    String getParent(String node)
    {
        return getLineFile(node, 4);
    }

    // String extractKey(String input)
    // {
    //     int length = input.getLength();
    //     int end = 0;

    //     for (end = 0; end < length; end++)
    //     {
    //         if (input[end] == ';')
    //             break;
    //     }

    //     String key;
    //     for (int i = 0; i < end; i++)
    //     {
    //         if (input[i] >= '0' && input[i] <= '9')
    //         {
    //             key = key + input[i];
    //         }
    //         else
    //         {
    //             key = "";
    //             break;
    //         }
    //     }

    //     for (int i = 0; i < end; i++)
    //     {
    //         if ((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z') || (input[i] == '+') || (input[i] == '-') || (input[i] == '.'))
    //         {
    //             key = key + input[i];
    //         }
    //     }

    //     return key;
    // }

    void inOrderTraversal(String node)
    {
        if (node == "nullptr")
            return;

        inOrderTraversal(getLeftChild(node));
        cout << "Key: " << node << ", Color: " << getNodeColor(node) << endl;
        inOrderTraversal(getRightChild(node));
    }

    int compare(char *key1, char *key2)
    {
        int cmp = strcmp(key1, key2);
        if (cmp < 0)
            return -1; // key1 < key2
        if (cmp > 0)
            return 1; // key1 > key2
        return 0;     // key1 == key2
    }

    String search_node(char *val, String r)
    {
        // Extract the key from the node at 'r'
        String val2 = extractOnlyTxt(r);
        // cout << val2 << endl;
        //  If the current node is null, return "nullptr"
        if (r == "nullptr")
        {
            String null("nullptr");
            return null;
        }

        //  current node for debugging
        //  cout << "Checking node: " << r << " with value: " << val2 << endl;

        int comparison = compare(val, val2.getdata());

        if (comparison == 0)
        {
            return r; // Match found
        }

        // If the value is less, search in the left child
        if (comparison < 0)
        {
            String leftChild = getLineFile(r, 2);
            String left = leftChild;
            return search_node(val, left);
        }
        // If the value is greater, search in the right child
        else
        {
            String rightChild = getLineFile(r, 3);
            String right = rightChild;
            return search_node(val, right);
        }
    }

    // to find the smallest node
    String smallNode(String node)
    {
        while (getLeftChild(node) != "nullptr")
        {
            node = getLeftChild(node);
        }
        return node;
    }

    void deletion(String key)
    {
        String nodeFile = search_node(key.getdata(), root.getdata());
        if (nodeFile == "nullptr")
        {
            cout << "Node with key '" << key << "' not found." << endl;
            return;
        }

        //  is a leaf
        if (getLeftChild(nodeFile) == "nullptr" && getRightChild(nodeFile) == "nullptr")
        {
            // String parent = getParent(nodeFile);
            if (getParent(nodeFile) != "nullptr")
            {
                if (getLeftChild(getParent(nodeFile)) == nodeFile)
                    replaceLine(getParent(nodeFile), 2, "nullptr");
                else
                    replaceLine(getParent(nodeFile), 3, "nullptr");
            }
            else
            {
                root = "nullptr"; // Deleting the root node
            }

            remove(nodeFile.getdata()); // Remove the file
            // String color = getNodeColor(nodeFile);
            if (getNodeColor(nodeFile) == "BLACK")
            {
                RBBalancing(getParent(nodeFile));
            }
            return;
        }

        // when one child either left or right
        if (getLeftChild(nodeFile) == "nullptr" || getRightChild(nodeFile) == "nullptr")
        {
            // String null = "nullptr";
            String child = (getLeftChild(nodeFile) != "nullptr") ? getLeftChild(nodeFile) : getRightChild(nodeFile); // get the non null child
            // String parent = getParent(nodeFile);

            if (getParent(nodeFile) != "nullptr")
            {
                if (getLeftChild(getParent(nodeFile)) == nodeFile)
                    replaceLine(getParent(nodeFile), 2, child);
                else
                    replaceLine(getParent(nodeFile), 3, child);
            }
            else
            {
                root = child;
            }

            replaceLine(child, 4, getParent(nodeFile));
            remove(nodeFile.getdata());
            String color = getNodeColor(nodeFile);
            if (color == "BLACK")
            {
                RBBalancing(getParent(nodeFile));
            }
            return;
        }
        //=============find the inordersuccessor the min node when this case =======//
        //  has two children
        String successorFile = smallNode(getRightChild(nodeFile));
        String successor = extractOnlyTxt(successorFile);
        // String parent = getParent(successorFile);

        if (getParent(successorFile) != nodeFile)
        {
            String rightChild = getRightChild(successorFile);                      // here we are getting successor
            replaceLine(getParent(successorFile), 2, rightChild);                  // getting rightchild
            replaceLine(rightChild, 4, getParent(successorFile));                  // getting parent
            replaceLine(getRightChild(successorFile), 3, getRightChild(nodeFile)); // replacing with right child
            replaceLine(getRightChild(nodeFile), 4, successorFile);                // replacing with successor
        }

        replaceLine(nodeFile, 1, successor);
        remove(successorFile.getdata());
        // String color = getNodeColor(successorFile);
        if (getNodeColor(successorFile) == "BLACK")
        {
            RBBalancing(getParent(successorFile));
        }

        return;
    }
    void extarctBranchName(String &name)
    {
        String temp;
        int count = 0;

        for (int i = 0; i < name.getLength(); i++)
        {
            if (name[i] == '/')
            {
                count++;
                continue; // Move to the next character after '/'
            }

            if (count == 2)
            {
                temp = temp + name[i];
            }
            else if (count > 2)
            {
                break;
            }
        }

        name = temp; // Assign the extracted branch name
    }

    void removerbBranches()
    {
        ofstream file("./Branches/rb_branch", ios::trunc);
        file.close();
    }

    void RBBalancing(String nodeFile)
    {
        // when it is null
        if (nodeFile == "nullptr" || nodeFile == root)
        {
            return; // Nothing to fix
        }

        String s;
        String par = getLineFile(nodeFile, 4);
        if (getLeftChild(par) == nodeFile)
        {
            s = getRightChild(par);
        }
        else
        {
            s = getLeftChild(par);
        }

        // String sColor = getNodeColor(s);

        //=================SimpleColor Switches==================//
        //  s is red
        if (getNodeColor(s) == "RED")
        {
            replaceLine(par, 5, "RED");
            replaceLine(s, 5, "BLACK");
            if (getLeftChild(par) == nodeFile)
            {
                leftRotate(par);
            }
            else
            {
                rightRotate(par);
            }
            RBBalancing(nodeFile); // Recur for the node's new position
            return;
        }

        //=================When in line==================//
        //  s is black and both its children are black
        // String sLeft = getLeftChild(s);
        // String sRight = getRightChild(s);
        if (getLeftChild(s) == "nullptr" || getNodeColor(getLeftChild(s)) == "BLACK")
        {
            if (getRightChild(s) == "nullptr" || getNodeColor(getRightChild(s)) == "BLACK")
            {
                replaceLine(s, 5, "RED");
                if (getNodeColor(par) == "RED")
                {
                    replaceLine(par, 5, "BLACK");
                }
                else
                {
                    RBBalancing(par); // Recur for the parent
                }
                return;
            }
        }

        //===========When they form a triangular pattern ===========//
        //  s is black, left child is red, and right child is black
        if (getLeftChild(s) != "nullptr" && getNodeColor(getLeftChild(s)) == "RED")
        {
            if (getRightChild(s) == "nullptr" || getNodeColor(getRightChild(s)) == "BLACK")
            {
                replaceLine(getLeftChild(s), 5, "BLACK");
                replaceLine(s, 5, "RED");
                rightRotate(s);
                RBBalancing(nodeFile); // Recur for the node's new position
                return;
            }
        }

        //  s is black, right child is red
        if (getRightChild(s) != "nullptr" && getNodeColor(getRightChild(s)) == "RED")
        {
            replaceLine(s, 5, getNodeColor(par));
            replaceLine(getRightChild(s), 5, "BLACK");
            replaceLine(par, 5, "BLACK");
            if (getLeftChild(par) == nodeFile)
            {
                leftRotate(par);
            }
            else
            {
                rightRotate(par);
            }
            return;
        }
    }
    void removeOneBranchRB(String &name)
    {
        ofstream file("./Branches/rb_branch", ios::trunc);
        remove(name.getdata());
        file << name << '\n';
    }

    bool SearchBranches(String branchName, int lineNumber)
    {
        ifstream file("./Branches/rb_branch", ios::in);

        char buffer[256];
        cout << "====================================" << endl;
        cout << " EXISTING BRANCHES IN RED BLACK TREE" << endl;
        cout << "====================================" << endl;

        while (file.getline(buffer, sizeof(buffer)))
        {
            String curr(buffer);
            if (curr == branchName)
            {
                return true;
            }
            lineNumber++;
        }

        file.close();

        return false;
    }

    String forCommits(String name)
    {
        int count = 0;
        String temp;
        for (int i = 0; i < name.getLength(); i++)
        {
            if (name[i] == '/')
            {
                temp = temp + name[i];
                count++;
            }
            if (count == 2)
            {
                break;
            }
            else
            {
                temp = temp + name[i];
            }
        }
        return temp;
    }
    void CommitChanges(String data, int number)
    {
        String tempfilepath;
        cout << data << endl;
        String node = this->search_node(data.getdata(), this->root.getdata());
        cout << node << endl;
        if (node == "nullptr")
        {
            cout << "Node with key '" << data << "' not found." << endl;
            return;
        }
        String newDirectory = this->pathDirectory;
        if (commits == 0)
        {
            newDirectory = newDirectory + "init/";
        }

        tempfilepath = "./Files/temp";
        char arr[10];
        toString(commits, arr);
        String num = arr;
        // cout << num << endl;
        tempfilepath = tempfilepath + num;
        tempfilepath = tempfilepath + ".txt";
        // cout << tempfilepath << endl;

        ofstream tempFile(tempfilepath.getdata(), ios::out);
        if (!tempFile.is_open())
        {
            cout << "Failed to open temp file for writing." << endl;
            return;
        }

        String dataFile = getDataPath(node);
        cout << dataFile << endl;
        cout << "Enter the name of the user you want to commit the changes to: ";
        String user;
        cin >> user;
        cout << "Enter the new name : ";
        String newname;
        cin >> newname;

        fstream file(dataFile.getdata(), ios::in | ios::out);
        if (!file.is_open())
        {
            cout << "Failed to open data file." << endl;
            return;
        }

        char buffer[1024];
        bool userFound = false;
        long pos = -1;
        int lineNumber = 0;
        while (file.getline(buffer, sizeof(buffer)))
        {
            String curr(buffer);
            String updatedLine;
            int fieldCount = 1;
            cout << number << endl;
            int x = number - 1;
            String currentField = extract(curr, x, ',');
            cout << "User " << user << " " << "Current field : " << currentField << endl;

            for (int i = 0; i <= curr.getLength(); ++i)
            {
                if (i == curr.getLength() || curr[i] == ',')
                {
                    // End of a field
                    if (fieldCount == number)
                    {
                        // If this is the column to update
                        if (currentField == user)
                        {
                            // Match user to update
                            currentField = newname; // Update current field
                            userFound = true;       // Mark user found
                        }
                    }

                    updatedLine = updatedLine + currentField;

                    if (i < curr.getLength())
                    {
                        updatedLine = updatedLine + ','; // Append comma if not at the end
                    }

                    // Reset current field and increment field count
                    currentField = "";
                    fieldCount++;
                }
                else
                {
                    currentField = currentField + curr[i]; // Accumulate characters
                }
            }

            tempFile << updatedLine.getdata() << '\n';
        }

        file.close();

        RedBlackTree newTree;
        String newp = forCommits(pathDirectory);
        cout << newp << endl;
        String nameFolder = newp + "Commit";

        // ifstream log(logFile.getdata());
        // if (!log.is_open())
        // {
        //     cout << "Failed to open log file." << endl;
        //     return;
        // }

        cout << "Enter Commit Message : ";
        char arr1[30];
        cin.ignore();
        cin.getline(arr1, sizeof(arr1));

        String message(arr1); // Convert the char array to String if needed

        // ofstream logFileWrite(logFile.getdata(), ios::app);
        // logFileWrite << "Commit " << commits + 1 << ": " << message << '\n';
        // logFileWrite.close();

        nameFolder = nameFolder + num;
        mkdir(nameFolder.getdata(), 0777);
        nameFolder = nameFolder + "/";
        String logFile = nameFolder;
        logFile = logFile + "log.txt";
        cout << logFile << endl;

        String sourceFolder = this->pathDirectory + "Commit";
        int x = num;
        x = x - 1;
        char arr2[10];
        toString(x, arr2);
        String xname = arr2;
        sourceFolder = sourceFolder + xname;
        if (x - 1 == 0)
        {
            sourceFolder = "./RedBlack/main/init/";
        }
        else
        {
            sourceFolder = sourceFolder + "/";
        }
        ofstream logFileWrite(logFile.getdata(), ios::out);
        logFileWrite << "Commit " << commits + 1 << ": " << message << '\n';
        logFileWrite << this->root.getdata() << '\n';
        logFileWrite.close();

        ofstream tempFileWrite("./RedBlack/main/init/log.txt", ios::app);

        tempFileWrite << "Commit " << commits + 1 << ": " << message << '\n';
        tempFileWrite.close();

        copyTree(*this, newTree, root, sourceFolder, nameFolder);
        this->root = newTree.root;
        String node1 = search_node(data.getdata(), root.getdata());
        // cout << "Hello there" << endl;

        String update = tempfilepath;
        // cout << "NODE : " << node1 << endl;
        replaceLine(node1, 6, update);
        commits++;
    }

    void displayCommitLog()
    {

        if (commits == 0)
        {
            cout << "No commits have been made yet." << endl;
            return;
        }

        else
        {
            ifstream file("./RedBlack/main/init/log.txt", ios::in);
            char buffer[1024];
            while (file.getline(buffer, sizeof(buffer)))
            {
                String curr(buffer);
                cout << curr << endl;
            }
            file.close();
        }
    }

    void revertCommits()
    {
        String targetDir = "./RedBlack/main/";
        if (commits == 0)
        {
            cout << "No commits to revert." << endl;
            return; // Exit early if there are no commits to revert
        }

        if (commits == 1)
        {
            // Handle reverting to the initial state
            String nameFolder = pathDirectory + "Commit0";
            String total = nameFolder + "/";
            String newdir = targetDir + "init/";
            cout << targetDir << endl;
            cout << "Reverting to: " << newdir << endl;
            cout << "Removing: " << total << endl;
            RedBlackTree newTree;
            copyTree(*this, newTree, root, total, newdir);
            this->root = newTree.root;

            // remove last line from log file
            char buffer[1024];
            int count = 0;

            // Optionally remove the commit folder
            // RemoveDirectory(nameFolder.getdata());
            total = total - '/'; // Ensure `total` contains the correct directory path
            // cout << total;
            // String rm = "rm -rf ";
            // String command = rm + total;
            // cout << command << endl;
            // system(command.getdata());
            commits--; // Decrement commit count
            return;
        }

        // Handle reverting for commits > 1
        int x = commits - 1;
        char arr[10];
        toString(x, arr);
        String xname = arr;

        int prevX = x - 1;
        char arr2[10];
        toString(prevX, arr2);
        String xname1 = (prevX == 0) ? "0" : arr2; // Handle edge case where prevX == 0

        String nameFolder = targetDir + "Commit";
        String newFolder = nameFolder + xname;
        String total = newFolder + "/";
        // cout << "Reverting to: " << total << endl;

        String prevCommit = nameFolder + xname1 + "/";
        cout << "Previous commit path: " << prevCommit << endl;

        RedBlackTree newTree;
        copyTree(*this, newTree, root, total, prevCommit);
        this->root = newTree.root;

        String rm = "rm -rf ";
        String command = rm + total;
        cout << command << endl;
        system(command.getdata());

        commits--; // Decrement commit count
    }

    void mergeBranches()
    {
        int line1;
        int line2;
        String mergeBranchRB;
        cout << "Enter the name of the  branches You want to  merge " << endl;
        cin >> mergeBranchRB;
        cout << "Enter First Branch : ";
        String branch1;
        cin >> branch1;

        cout << "Enter Second Branch : ";
        String branch2;
        cin >> branch2;

        // if (SearchBranches(branch1, line1) && SearchBranches(branch2, line2))
        {
            String branch1Path = "./RedBlack/" + branch1;
            String branch2Path = "./RedBlack/" + branch2;

            cout << "Enter the name of the new branch that is to be merged: ";
            cin >> mergeBranchRB;
            cout << "ping";

            mergeBranchRB = "./RedBlack/" + mergeBranchRB;

            mkdir(mergeBranchRB.getdata(), 0777);
            RedBlackTree mergeTree;

            mergeTree.addBranch();
            mergeTree.addBranch();
        }
    }
};

// int main()
// {
//     RedBlackTree rbt;
//     String path1 = "./RedBlack/main/init/";

//     rbt.Insert("a", "nullptr", 1, path1);
//     rbt.Insert("b", rbt.getRoot(), 1, path1);
//     rbt.Insert("c", rbt.getRoot(), 1, path1);
//     rbt.Insert("d", rbt.getRoot(), 1, path1);
//     rbt.Insert("e", rbt.getRoot(), 1, path1);

//     rbt.CommitChanges("a", 1);
//     // rbt.CommitChanges("b");
//     rbt.dislayLog();
//     // rbt.revertCommits();

//     // cout << "In-Order Traversal:" << endl;
//     // rbt.inOrderTraversal(rbt.getRoot());
//     // rbt.deletion("c");
//     // rbt.addBranch();
//     // rbt.SwitchBranches();
//     // String path2 = rbt.getCurrentPathDirectory();

//     // // cout << path2 << endl;

//     // rbt.Insert("f", rbt.getRoot(), 1, path2);
//     // rbt.Insert("g", rbt.getRoot(), 1, path2);
//     // rbt.Insert("h", rbt.getRoot(), 1, path2);

//     // rbt.SwitchBranches();
//     // rbt.inOrderTraversal(rbt.getRoot());
//     // // // rbt.visualizeTree("main");
//     // rbt.removeBranches();

//     return 0;
// }

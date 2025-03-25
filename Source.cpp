
//Muhammad Hasan ALTAF i230698
//Hasan Nvaeed i230797
//Saqib Atif i230769

#include <iostream>
#include <fstream>
#include "RedBlack.h"
#include "Avl.h"
#include "String.h" // Custom String library
// #include "Utility_functions.h"
//   #include "MerkleTree.h"
#include "Btree.h"
// #include "MerkleTree.h"
using namespace std;

// RedBlackTree rbtree;
// AVL_tree<String> avl;
RedBlackTree rbtree;
String rbtreePath = "./RedBlack/main/init/";
AVL_tree<String> avl;
String avlPath = "./AVL/main/init/";
Btree<String> btree;
String btreePath = "./BTree/main/init/";

int choice;
// MerkleTree mt;
void extractColumns(String filename, int col)
{
    ifstream file(filename.getdata(), ios::in); // Open the file in read mode
    char buffer[1000];
    int lineIndex = 0;

    while (file.getline(buffer, 1000))
    {
        if (lineIndex == 0)
        {
            lineIndex++;
            continue; // Skip first line
        }

        String curr(buffer);
        String extracted = extract(curr, col, ',');
        String path = "./Files/";
        path = path + extracted + ".txt";

        if (rbtree.search_node(extracted.getdata(), rbtree.getRoot()) == "nullptr")
        {
            if (choice == 1)
                rbtree.Insert(extracted, rbtree.getRoot(), 1, "./RedBlack/main/init/");
            if (choice == 2)
            {
                avl.Insert(extracted, avl.root, 1, "./AVL/main/init/");
            }
            if (choice == 3)
            {
                btree.insert(extracted, btree.root, 1, "./BTree/main/init/");
            }
            // mt.InsertInMerkle(path);
        }
        else
        {
            // cout << "duplicate value: " << extracted << endl;
        }

        ofstream newFile(path.getdata(), ios::app);
        newFile << curr << '\n';

        lineIndex++;
    }
}

int displayColumnOne(String filename)
{
    ifstream file(filename.getdata(), ios::in); // Open the file in read mode

    if (!file)
    {
        // cout << "Error: Unable to open file." << endl;
        return -1;
    }

    char buffer[1024]; // Buffer to hold a single line
    file.getline(buffer, sizeof(buffer));

    String curr(buffer);

    // Print out the first row as headers
    for (int i = 0; i < curr.getLength(); i++)
    {
        if (curr[i] == ',')
        {
            cout << endl;
        }
        else
        {
            cout << curr[i];
        }
    }

    cout << "\nSelect the column based on which you want to commit changes at : ";
    int input;
    cin >> input;

    file.close(); // Close the CSV file

    return input;
}

void FileReading(String filename)
{
    ifstream file(filename.getdata(), ios::in); // Open the file in read mode

    if (!file)
    {
        cout << "Error: Unable to open file." << endl;
        return;
    }

    char buffer[1024]; // Buffer to hold a single line
    file.getline(buffer, sizeof(buffer));

    String curr(buffer);

    // Print out the first row as headers
    for (int i = 0; i < curr.getLength(); i++)
    {
        if (curr[i] == ',')
        {
            cout << endl;
        }
        else
        {
            cout << curr[i];
        }
    }

    cout << "\nSelect the column based on which you want to create the Trees: ";
    int choice;
    cin >> choice;

    extractColumns(filename, choice);

    file.close(); // Close the CSV file

    // Example call to verify specific row reading
    // cout << "\nEnter the filename containing pointers to verify a row: ";
    // String pointerFileName;
    // cin >> pointerFileName;
    // String fullPath = pointerFileName;
    // fullPath = fullPath + ".txt";
    // ReadFileRows(filename, fullPath);
}

int main()
{

    cout << "**********************************************" << endl;
    cout << "*               GIT LITE                     *" << endl;
    cout << "*      ----------------------------------    *" << endl;
    cout << "*        Hello and Welcome to GIT LITE!      *" << endl;
    cout << "*      ----------------------------------    *" << endl;
    cout << "**********************************************" << endl;
    cout << endl;
    cout << "Initializing the REPOS ..." << endl;
    cout << "Enter the Tree you want to create: " << endl;
    cout << "1. Red Black Tree" << endl;
    cout << "2. AVL Tree" << endl;
    cout << "3. BTree" << endl;
    cin >> choice;
    String fileName;

    while (true)
    {
        String input;

        cout << "git_> ";
        cin >> input;

        if (input == "init")
        {
            cin >> fileName;
            cout << "The Columns of the Entered File are: " << endl;
            FileReading("./health.csv");
            // rbtree.inOrderTraversal(rbtree.getRoot());
        }

        if (input == "branch")
        {
            if (choice == 1)
            {
                rbtree.addBranch();
            }
            if (choice == 2)
            {
                avl.addBranch();
            }
            if (choice == 3)
            {
                // btree.addBranch();
            }
            cout << "Branch Added Successfully" << endl;
        }

        if (input == "commit")
        {
            int number = displayColumnOne("./health.csv");
            cout << "Enter the filename  : ";
            String in;
            cin >> in;
            if (choice == 1)
            {
                rbtree.CommitChanges(in, number);
            }
            if (choice == 2)
            {
                avl.CommitChangesavl(in, number);
            }
            if (choice == 3)
            {
                // btree.CommitChanges();
            }
            cout << "Changes Committed Successfully" << endl;
        }
        if (input == "checkout")
        {
            if (choice == 1)
            {
                rbtree.SwitchBranches();
            }
            if (choice == 2)
            {
                avl.SwitchAvlBranches();
            }
            if (choice == 3)
            {
                // btree.SwitchBranches();
            }
        }
        if (input == "log")
        {
            if (choice == 1)
            {
                rbtree.displayCommitLog();
            }
            if (choice == 2)
            {
                avl.displayCommitLog();
            }
            if (choice == 3)
            {
                // btree.dislayLog();
            }
        }

        if (input == "branches")
        {
            if (choice == 1)
            {
                rbtree.ShowRedBlackBranches();
            }
            if (choice == 2)
            {
                avl.ShowAVLbranch();
            }
            if (choice == 3)
            {
                // btree.showBranches();
            }
        }
        if (input == "current-branch")
        {
            if (choice == 1)
            {
                rbtree.DisplayCurrentBracnh();
            }
            if (choice == 2)
            {
                avl.DisplayCurrentBracnhAVL();
            }
            if (choice == 3)
            {
                // btree.DisplayCurrentBracnh();
            }
        }

        if (input == "revert")
        {
            if (choice == 1)
            {
                rbtree.revertCommits();
            }
            if (choice == 2)
            {
                avl.revertCommitsavl();
            }
            if (choice == 3)
            {
                // btree.revertCommits();
            }
        }

        if (input == "delete-branch")
        {
            if (choice == 1)
            {
                rbtree.deleteBranch();
            }
            if (choice == 2)
            {
                avl.deleteBranch();
            }
            if (choice == 3)
            {
                // btree.deleteBranch();
            }
        }
       

        if (input == "merge")
        {
            if (choice == 1)
            {
                rbtree.mergeBranches();
            }
            if (choice == 2)
            {
                avl.merge();
            }
            if (choice == 3)
            {
                // btree.mergeBranches();
            }
        }
        if (input == "help")
        {
            cout << "init : Initialize the repository" << endl;
            cout << "branch : Create a new branch" << endl;
            cout << "commit : Commit the changes" << endl;
            cout << "checkout : Switch to a different branch" << endl;
            cout << "log : Display the commit log" << endl;
            cout << "branches : Display all the branches" << endl;
            cout << "current-branch : Display the current branch" << endl;
            cout << "revert : Revert the commits" << endl;
            cout << "delete-branch : Delete a branch" << endl;
            cout << "merge : Merge the branches" << endl;
            cout << "exit : Exit the program" << endl;
        }

        if (input == "exit")
        {
            break;
        }

        // if (input == "viewHashes")
        // {
        //     // merkleTree mt;
        //     if (choice == 1)
        //     {
        //         String curr = rbtree.getRoot();
        //         mt.buildMerkleTree(curr, 1);
        //     }
        //     if (choice == 2)
        //     {
        //         mt.buildMerkleTree(avl.root, 2);
        //     }
        //     if (choice == 3)
        //     {
        //         // mt.buildMerkleTree(btree.root, 3);
        //     }
        // }
    }
}
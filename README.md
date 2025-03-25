GitLite: A Lightweight Version Control System with Advanced Tree Structures
GitLite is a custom-built version control system inspired by Git but designed with a unique approach to data management. Unlike traditional Git, which primarily relies on hashes and object storage, GitLite introduces AVL Trees, B-Trees, and Red-Black Trees to efficiently organize, track, and retrieve versioned files. These tree structures are implemented using file-based storage, allowing for a structured and efficient way to handle version control without relying on databases.

🔹 Key Features
✅ File Versioning & Change Tracking
GitLite enables users to track file modifications, maintain a commit history, and revert to previous versions when needed. It provides a structured approach to version control, similar to Git but with a unique file-based implementation.

🌳 Tree-Based Indexing for Efficient Lookups
GitLite introduces multiple tree structures to manage file versions effectively:

AVL Tree: A self-balancing binary search tree that ensures fast and predictable lookups.

B-Tree: An optimized tree structure for efficient disk-based storage, reducing I/O operations.

Red-Black Tree: A balanced binary search tree offering quick insertions and deletions, making it ideal for dynamic version management.

📂 Folder Structure
To use GitLite, you should create the following folders in the project directory:

avl/ – Contains the implementation of AVL Trees for version control.

main/ – The main logic for handling AVL-based storage.

init/ – Initialization scripts and configurations for AVL tree storage.

btree/ – Stores the B-Tree versioning system.

main/ – Core implementation for B-Tree based indexing.

init/ – Setup files for B-Tree structure.

rbtree/ – Implements Red-Black Trees for efficient file organization.

main/ – The main execution files for Red-Black Tree storage.

init/ – Initialization and setup configurations.

branch/ – An empty folder reserved for branching features (future expansion).

⏪ Commit History & Rollback Mechanism
Each commit is stored in a structured file format, enabling quick retrieval of past changes. The rollback feature allows users to revert to a previous version efficiently, leveraging the tree structures to optimize the lookup process.

🔍 Fast Search & Retrieval
By implementing these advanced tree structures, GitLite enhances search performance for files, commits, and metadata. This results in quicker access to version history compared to traditional linear file scanning.

🚀 Why Choose GitLite?
GitLite is ideal for developers, researchers, and students looking for a lightweight yet powerful version control system. It serves as an excellent learning tool for understanding advanced data structures while offering practical file versioning and tracking capabilities.

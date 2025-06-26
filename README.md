# Database-Engine-25
# B-Tree of Order 4 (Key-Value Storage)

A modular C implementation of a **B-Tree of order 4**, where each node can store up to **4 keys** and **5 children**.  
Each key is an `int`, and each value is a `string`.  

This project supports:
- ✅ Insertion
- ✅ Deletion
- ✅ Search
- ✅ Level-order traversal (grouped by level)
- 🧱 Modular file structure
- 🛠 Build system via `Makefile`

---

## 📁 File Structure
.
├── main.c # Entry point
├── btree.c # Core B-Tree logic
├── btree.h # B-Tree header
├── queue.c # Level-order traversal helper
├── queue.h # Queue header
├── Makefile # Build script
└── README.md # This file

---

## 🔧 Compilation

Use **MSYS2 UCRT64 shell** (or any POSIX-compatible Bash with working `gcc` and `make`):

To run the program:
./btree_app

## Sample Output

Search result for 20: Rahul
Search result for 100: Not Found

Level-order traversal:
[10,Pratham]
[2,Hardik 3,Burhan 5,Ridin] [12,Yash 17,Divyansh 20,Rahul 30,Shivam]

# Btree details

Order: 4 (i.e., max 4 keys, max 5 children)

Key: Integer

Value: String

Balanced: Always remains height-balanced due to node splitting/merging.

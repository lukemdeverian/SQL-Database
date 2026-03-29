# SQL Engine

A relational database engine built from scratch in C++. Supports a subset of SQL with file-backed persistent storage, B+ tree indexing, and a boolean expression evaluator for WHERE clauses. All underlying data structures — including the B+ tree, multimap, stack, and queue — are implemented from scratch without relying on the C++ STL containers. The STL is 
used only for `std::string`, `std::vector`, and `std::fstream`.

Built by **Luke Deverian** as a final project for Data Structures.

---

## Features

- `CREATE` — define new tables with named fields
- `INSERT` — add records to a table
- `SELECT` — query records with optional `WHERE`, field projection, and `*`
- `UPDATE` — modify existing records with optional `WHERE` condition
- Boolean `WHERE` clauses with `AND`, `OR`, parentheses, and all comparison operators (`=`, `>`, `<`, `>=`, `<=`)
- B+ tree indexing on every field for fast lookups
- File-backed storage — tables persist across program runs as `.bin` files
- Custom tokenizer and state-machine parser built without any parsing libraries

---

## Architecture

```
SQL
 └── Parser            — tokenizes input and builds a parse tree via state machine
      └── STokenizer   — character-level tokenizer using a DFA transition table
 └── Table             — executes queries against a named table
      └── FileRecord   — fixed-size binary record I/O
      └── MMap         — multimap index (string → list of record numbers)
           └── BPlusTree — self-balancing B+ tree with leaf-chain iteration
 └── ShuntingYard      — converts infix WHERE conditions to postfix for evaluation
```

**Parser** uses a hand-built state machine (`_ptable`) to validate and tokenize SQL commands into a parse tree (`MMap<string, string>`). No regex or external libraries are used.

**Table** stores records as fixed-size `FileRecord` structs in a binary file. Every field has a corresponding `MMap<string, long>` index mapping field values to record numbers, enabling O(log n) lookups.

**WHERE evaluation** converts infix boolean expressions to postfix using the shunting-yard algorithm, then evaluates them using a record-number stack — intersecting sets for `AND` and unioning them for `OR`.

---

## Build

Requires CMake 3.5+ and a C++11 compiler.

```bash
cmake -B build
cmake --build build
```

Binaries are placed in `build/bin/`.

---

## Run the Tests

```bash
./build/bin/basic_test          # Core SELECT, INSERT, CREATE tests
./build/bin/update_test         # UPDATE command tests
./build/bin/basic_test debug    # Prints full table output
./build/bin/update_test debug
```

---

## Generate the HTML Demo

```bash
./build/bin/demo_main
```

Opens `demo_output.html` — a self-contained page showing query execution with results, record numbers, and timing. Open it in any browser.

---

## Example Queries

```sql
-- Create a table
make table employee fields last, first, dep, salary, year

-- Insert records
insert into employee values Johnson, Jack, HR, 150000, 2014
insert into employee values Blow, Joe, CS, 100000, 2018

-- Select all
select * from employee

-- Select with WHERE
select * from employee where last = Johnson

-- Boolean WHERE
select * from employee where last = Blow and dep = CS

-- Field projection
select last, first from employee

-- Wildcard with complex condition
select * from student where lname > J and (major = CS or major = Art)

-- Update matching rows
update employee set salary = 99999 where last = Johnson

-- Update all rows
update employee set dep = Finance
```

---

## Known Limitations

- **No DELETE command** — records cannot be removed once inserted (planned)
- **No JOIN support** — queries operate on a single table at a time
- **Fixed record size** — field values are capped at 100 characters, longer values are silently truncated
- **No persistent table registry** — the `SQL` object discovers tables on demand by loading from disk; there is no master catalog
- **Temp files** — complex SELECT queries create temporary `.bin` files (`tempPostFix`, `tempPostFix(1)`, ...) that are not automatically cleaned up
- **B+ tree iterator** — leaf-chain traversal has a known edge case in certain insertion orders; core operations (insert, contains, get) are stable for all tested workloads
- **B+ remove** — has not been implemented since the current SQL functions do not require it

---

## Project Structure

```
.
├── includes/
│   ├── sql/          — SQL class (command dispatcher)
│   ├── parser/       — Parser and state machine
│   ├── table/        — Table, FileRecord, token types
│   ├── bplustree/    — BPlusTree, Map, MMap
│   ├── tokenizer/    — STokenizer, FTokenizer
│   ├── shunting_yard/— Infix to postfix conversion
│   ├── queue/        — Custom queue
│   ├── stack/        — Custom stack
│   └── html_logger.h — HTML demo output generator
├── _tests/
│   └── _test_files/
│       ├── basic_test.cpp
│       └── update_test.cpp
├── demo_main.cpp     — Runs demo queries and writes demo_output.html
├── main.cpp
└── CMakeLists.txt
```

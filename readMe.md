# Page replacement algorithms

A program for ram management with page replacement algorithms!

## How to Run

### Build Instructions

To compile the program and create the necessary object files, simply run the following command:

```bash
make
```

To clean up all the object files, use:

```bash
make clean
```

## Running different algorithms

1. FIFO:
   ```bash
   ./pageReplacement fifo "Assignment 2 input file.csv"
   ```
2. LRU:
   ```bash
   ./pageReplacement lru "Assignment 2 input file.csv"
   ```
3. Optimal:
   ```bash
   ./pageReplacement optimal "Assignment 2 input file.csv"
   ```
4. Second Chance CLK:
   ```bash
   ./pageReplacement secondChance "Assignment 2 input file.csv"
   ```

### Input file format

The input file follow a specific format for the algorthims to work. Please ensure that this file is correctly structured before running the algorithms.

Enjoy managing the ram!!!

# CS 300 Project One – Runtime and Memory Analysis

## Overview
This document analyzes the runtime and memory tradeoffs of three data structures—Vector, Hash Table, and Binary Search Tree—used to store and manage course data.

## Data Structures Analyzed
- Vector
- Hash Table
- Binary Search Tree (BST)

## Runtime Analysis (n = number of courses)

| Task | Vector | Hash Table | BST (Average) |
|-----|-------|-----------|---------------|
| Build structure | O(n) | O(n) | O(n log n) |
| Search for course | O(n) | O(1) average | O(log n) |
| Print sorted list | O(n log n) | O(n log n) | O(n) |
| Memory usage | Low | Moderate | Higher per node |

## Advantages and Disadvantages

### Vector
- Simple and memory efficient
- Requires linear search and sorting

### Hash Table
- Fastest lookup time on average
- No inherent ordering; sorting required

### Binary Search Tree
- Naturally ordered
- Efficient sorted output via in-order traversal
- Worst-case performance if unbalanced

## Recommendation
The Binary Search Tree is the most suitable data structure for this problem because it supports efficient searching and allows printing all courses in alphanumeric order without additional sorting.

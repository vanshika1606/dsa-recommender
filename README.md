# E-commerce Product Recommendation System (Using Graphs)

Data Structures and Algorithms Design Lab (25B17CI578), ODD 2026 mini project. Language: C++.

## Problem

Online stores suggest related products ("customers who bought this also bought"). This project models the store as a graph: each product is a vertex, and a weighted edge (1 to 10) means two products are bought together. A higher weight means a stronger link.

## Concepts used

| Feature | Data structure / algorithm |
|---|---|
| Product catalog and links | Graph with adjacency list (`unordered_map`) |
| Customers also bought | BFS using a queue |
| Related chain | Recursive DFS with a depth limit |
| Strongest match | Dijkstra using a priority queue (min-heap), edge cost = 11 - weight |
| Trending products | Degree counting and sorting |
| Data loading | File handling (text files) |
| User interface | Menu-driven console program |

## Folder structure

```
dsa-recommender/
├── include/
│   ├── product.h       Product struct
│   └── graph.h         Graph class declaration
├── src/
│   ├── graph.cpp       Graph, BFS, DFS, Dijkstra, trending, file loading
│   └── main.cpp        Menu-driven interface
├── data/
│   ├── products.txt    id,name,category,price
│   └── edges.txt       id1,id2,strength
├── tests/
│   └── test_cases.md   Test cases with expected output
├── README.md
└── .gitignore
```

## Build and run

Run these from inside the project folder (the program reads `data/` using a relative path).

```
mkdir bin
g++ -std=c++17 -Wall -Iinclude src/main.cpp src/graph.cpp -o bin/recommender.exe
./bin/recommender.exe
```

On PowerShell use `.\bin\recommender.exe`. If `bin` already exists, skip `mkdir bin`.

## Menu

```
1. View all products
2. View product connections
3. Recommendations: customers also bought (BFS)
4. Recommendations: related chain (DFS)
5. Recommendations: strongest match (Dijkstra)
6. Trending products
7. Add a new product
8. Add a connection between products
0. Exit
```

Products and connections added at runtime live in memory for that run. To keep them, add them to the text files in `data/`.

## Data file format

`products.txt`: one product per line, `id,name,category,price`.
`edges.txt`: one link per line, `id1,id2,strength` with strength from 1 to 10.
Lines starting with `#` and blank lines are ignored. Bad lines are skipped with a message.

## How Dijkstra is used (worked example)

Strength becomes cost: cost = 11 - strength, so a strong link is cheap. For Smartphone, Wireless Earbuds costs 3 (strength 8). Laptop is not linked to Smartphone directly, but the cheapest path is Smartphone > Wireless Earbuds (3) > Noise Cancelling Headphones (6) > Laptop (7), a total cost of 16. So Laptop appears as the 5th match, found through two middle products.

## Testing

See `tests/test_cases.md`.

#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include "product.h"

struct Edge {
    int neighbor;
    int weight;
};

class Graph {
private:
    std::unordered_map<int, Product> products;
    std::unordered_map<int, std::vector<Edge>> adjList;

    void dfsHelper(int current, int depth, int depthLimit, int maxResults,
                   std::unordered_set<int>& visited, std::vector<int>& result) const;

public:
    bool addProduct(const Product& p);
    bool addEdge(int id1, int id2, int weight);
    bool productExists(int id) const;
    const Product& getProduct(int id) const;
    int productCount() const;
    void printProducts() const;
    void printGraph() const;

    std::vector<int> recommendBFS(int startId, int maxResults = 5) const;
    std::vector<int> recommendDFS(int startId, int depthLimit = 3, int maxResults = 5) const;
    std::vector<std::pair<int, int>> recommendDijkstra(int startId, int maxResults = 5) const;
    std::vector<std::pair<int, int>> trendingProducts(int topN = 5) const;

    int loadProducts(const std::string& filename);
    int loadEdges(const std::string& filename);
};

#endif

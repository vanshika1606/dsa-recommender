#include "graph.h"
#include <algorithm>
#include <climits>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <queue>
#include <sstream>

bool Graph::addProduct(const Product& p) {
    if (productExists(p.id)) return false;
    products[p.id] = p;
    adjList[p.id] = std::vector<Edge>();
    return true;
}

bool Graph::addEdge(int id1, int id2, int weight) {
    if (id1 == id2) return false;
    if (!productExists(id1) || !productExists(id2)) return false;
    if (weight < 1 || weight > 10) return false;

    for (const Edge& e : adjList[id1]) {
        if (e.neighbor == id2) return false;
    }

    adjList[id1].push_back({id2, weight});
    adjList[id2].push_back({id1, weight});
    return true;
}

bool Graph::productExists(int id) const {
    return products.find(id) != products.end();
}

const Product& Graph::getProduct(int id) const {
    return products.at(id);
}

int Graph::productCount() const {
    return (int)products.size();
}

void Graph::printProducts() const {
    std::vector<int> ids;
    for (const auto& kv : products) ids.push_back(kv.first);
    std::sort(ids.begin(), ids.end());

    std::cout << std::left << std::setw(5) << "ID" << std::setw(30) << "Name"
              << std::setw(14) << "Category" << "Price\n";
    std::cout << std::string(58, '-') << "\n";
    for (int id : ids) {
        const Product& p = products.at(id);
        std::cout << std::left << std::setw(5) << p.id << std::setw(30) << p.name
                  << std::setw(14) << p.category << "Rs. " << p.price << "\n";
    }
}

void Graph::printGraph() const {
    std::vector<int> ids;
    for (const auto& kv : products) ids.push_back(kv.first);
    std::sort(ids.begin(), ids.end());

    for (int id : ids) {
        std::cout << "[" << id << "] " << products.at(id).name << " -> ";
        const std::vector<Edge>& edges = adjList.at(id);
        if (edges.empty()) std::cout << "(no connections)";
        for (const Edge& e : edges) {
            std::cout << products.at(e.neighbor).name << "(" << e.weight << ") ";
        }
        std::cout << "\n";
    }
}

std::vector<int> Graph::recommendBFS(int startId, int maxResults) const {
    std::vector<int> result;
    if (!productExists(startId)) return result;

    std::unordered_set<int> visited;
    std::queue<int> q;

    visited.insert(startId);
    q.push(startId);

    while (!q.empty() && (int)result.size() < maxResults) {
        int current = q.front();
        q.pop();

        for (const Edge& e : adjList.at(current)) {
            if (visited.count(e.neighbor) == 0) {
                visited.insert(e.neighbor);
                result.push_back(e.neighbor);
                q.push(e.neighbor);
                if ((int)result.size() >= maxResults) break;
            }
        }
    }
    return result;
}

void Graph::dfsHelper(int current, int depth, int depthLimit, int maxResults,
                      std::unordered_set<int>& visited, std::vector<int>& result) const {
    if (depth > depthLimit) return;

    for (const Edge& e : adjList.at(current)) {
        if ((int)result.size() >= maxResults) return;
        if (visited.count(e.neighbor) == 0) {
            visited.insert(e.neighbor);
            result.push_back(e.neighbor);
            dfsHelper(e.neighbor, depth + 1, depthLimit, maxResults, visited, result);
        }
    }
}

std::vector<int> Graph::recommendDFS(int startId, int depthLimit, int maxResults) const {
    std::vector<int> result;
    if (!productExists(startId)) return result;

    std::unordered_set<int> visited;
    visited.insert(startId);
    dfsHelper(startId, 1, depthLimit, maxResults, visited, result);
    return result;
}

std::vector<std::pair<int, int>> Graph::recommendDijkstra(int startId, int maxResults) const {
    std::vector<std::pair<int, int>> result;
    if (!productExists(startId)) return result;

    std::unordered_map<int, int> dist;
    for (const auto& kv : products) dist[kv.first] = INT_MAX;
    dist[startId] = 0;

    std::priority_queue<std::pair<int, int>,
                        std::vector<std::pair<int, int>>,
                        std::greater<std::pair<int, int>>> pq;
    pq.push({0, startId});

    while (!pq.empty()) {
        std::pair<int, int> top = pq.top();
        pq.pop();
        int d = top.first;
        int u = top.second;

        if (d > dist[u]) continue;

        for (const Edge& e : adjList.at(u)) {
            int cost = 11 - e.weight;
            if (d + cost < dist[e.neighbor]) {
                dist[e.neighbor] = d + cost;
                pq.push({dist[e.neighbor], e.neighbor});
            }
        }
    }

    for (const auto& kv : dist) {
        if (kv.first != startId && kv.second != INT_MAX) {
            result.push_back({kv.first, kv.second});
        }
    }

    std::sort(result.begin(), result.end(),
              [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                  if (a.second != b.second) return a.second < b.second;
                  return a.first < b.first;
              });

    if ((int)result.size() > maxResults) result.resize(maxResults);
    return result;
}

std::vector<std::pair<int, int>> Graph::trendingProducts(int topN) const {
    std::vector<std::pair<int, int>> degrees;
    for (const auto& kv : adjList) {
        degrees.push_back({kv.first, (int)kv.second.size()});
    }

    std::sort(degrees.begin(), degrees.end(),
              [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                  if (a.second != b.second) return a.second > b.second;
                  return a.first < b.first;
              });

    if ((int)degrees.size() > topN) degrees.resize(topN);
    return degrees;
}

static std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

int Graph::loadProducts(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return -1;

    int loaded = 0;
    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string idStr, name, category, priceStr;
        std::getline(ss, idStr, ',');
        std::getline(ss, name, ',');
        std::getline(ss, category, ',');
        std::getline(ss, priceStr, ',');

        try {
            Product p;
            p.id = std::stoi(trim(idStr));
            p.name = trim(name);
            p.category = trim(category);
            p.price = std::stod(trim(priceStr));
            if (addProduct(p)) loaded++;
        } catch (...) {
            std::cout << "Skipped bad product line: " << line << "\n";
        }
    }
    return loaded;
}

int Graph::loadEdges(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return -1;

    int loaded = 0;
    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string aStr, bStr, wStr;
        std::getline(ss, aStr, ',');
        std::getline(ss, bStr, ',');
        std::getline(ss, wStr, ',');

        try {
            int a = std::stoi(trim(aStr));
            int b = std::stoi(trim(bStr));
            int w = std::stoi(trim(wStr));
            if (addEdge(a, b, w)) loaded++;
            else std::cout << "Skipped invalid connection: " << line << "\n";
        } catch (...) {
            std::cout << "Skipped bad connection line: " << line << "\n";
        }
    }
    return loaded;
}

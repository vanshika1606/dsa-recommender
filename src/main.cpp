#include <cstdlib>
#include <iostream>
#include <string>
#include "graph.h"

void checkInputEnded() {
    if (std::cin.eof()) {
        std::cout << "\nInput ended. Exiting.\n";
        std::exit(0);
    }
}

int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(10000, '\n');
            return value;
        }
        checkInputEnded();
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Please enter a whole number.\n";
    }
}

double readDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(10000, '\n');
            return value;
        }
        checkInputEnded();
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Please enter a number.\n";
    }
}

std::string readLine(const std::string& prompt) {
    std::string text;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, text);
        checkInputEnded();
        if (!text.empty()) return text;
        std::cout << "This cannot be empty.\n";
    }
}

void printProductLine(const Graph& g, int id) {
    const Product& p = g.getProduct(id);
    std::cout << "[" << p.id << "] " << p.name << " (" << p.category
              << ", Rs. " << p.price << ")\n";
}

bool askValidProduct(const Graph& g, int& id) {
    id = readInt("Enter product ID: ");
    if (!g.productExists(id)) {
        std::cout << "No product with ID " << id << ".\n";
        return false;
    }
    std::cout << "Selected: ";
    printProductLine(g, id);
    return true;
}

void showBFS(const Graph& g) {
    int id;
    if (!askValidProduct(g, id)) return;
    std::cout << "Customers who bought this also bought (BFS):\n";
    std::vector<int> result = g.recommendBFS(id);
    if (result.empty()) std::cout << "  No related products found.\n";
    for (int r : result) {
        std::cout << "  ";
        printProductLine(g, r);
    }
}

void showDFS(const Graph& g) {
    int id;
    if (!askValidProduct(g, id)) return;
    std::cout << "A related chain you might explore (DFS):\n";
    std::vector<int> result = g.recommendDFS(id);
    if (result.empty()) std::cout << "  No related products found.\n";
    for (int r : result) {
        std::cout << "  ";
        printProductLine(g, r);
    }
}

void showDijkstra(const Graph& g) {
    int id;
    if (!askValidProduct(g, id)) return;
    std::cout << "Strongest overall matches (Dijkstra, lower cost = stronger match):\n";
    std::vector<std::pair<int, int>> result = g.recommendDijkstra(id);
    if (result.empty()) std::cout << "  No related products found.\n";
    for (const auto& r : result) {
        std::cout << "  cost " << r.second << " -> ";
        printProductLine(g, r.first);
    }
}

void showTrending(const Graph& g) {
    std::cout << "Trending products (most connections):\n";
    for (const auto& t : g.trendingProducts()) {
        std::cout << "  " << t.second << " connections -> ";
        printProductLine(g, t.first);
    }
}

void addProductFlow(Graph& g) {
    int id = readInt("New product ID: ");
    if (id <= 0) {
        std::cout << "ID must be a positive number.\n";
        return;
    }
    if (g.productExists(id)) {
        std::cout << "A product with ID " << id << " already exists.\n";
        return;
    }
    Product p;
    p.id = id;
    p.name = readLine("Name: ");
    p.category = readLine("Category: ");
    p.price = readDouble("Price: ");
    if (p.price < 0) {
        std::cout << "Price cannot be negative.\n";
        return;
    }
    g.addProduct(p);
    std::cout << "Product added.\n";
}

void addConnectionFlow(Graph& g) {
    int a = readInt("First product ID: ");
    int b = readInt("Second product ID: ");
    int w = readInt("Link strength (1-10): ");

    if (a == b) {
        std::cout << "A product cannot be connected to itself.\n";
    } else if (!g.productExists(a) || !g.productExists(b)) {
        std::cout << "One or both product IDs do not exist.\n";
    } else if (w < 1 || w > 10) {
        std::cout << "Strength must be between 1 and 10.\n";
    } else if (!g.addEdge(a, b, w)) {
        std::cout << "These products are already connected.\n";
    } else {
        std::cout << "Connection added.\n";
    }
}

int main() {
    Graph store;

    int p = store.loadProducts("data/products.txt");
    if (p < 0) {
        std::cout << "Could not open data/products.txt. Run the program from the project folder.\n";
        return 1;
    }
    int e = store.loadEdges("data/edges.txt");
    if (e < 0) {
        std::cout << "Could not open data/edges.txt. Run the program from the project folder.\n";
        return 1;
    }

    std::cout << "=== E-commerce Product Recommendation System ===\n";
    std::cout << "Loaded " << p << " products and " << e << " connections.\n";

    int choice = -1;
    while (choice != 0) {
        std::cout << "\n---------- MENU ----------\n";
        std::cout << "1. View all products\n";
        std::cout << "2. View product connections\n";
        std::cout << "3. Recommendations: customers also bought (BFS)\n";
        std::cout << "4. Recommendations: related chain (DFS)\n";
        std::cout << "5. Recommendations: strongest match (Dijkstra)\n";
        std::cout << "6. Trending products\n";
        std::cout << "7. Add a new product\n";
        std::cout << "8. Add a connection between products\n";
        std::cout << "0. Exit\n";
        choice = readInt("Choice: ");

        switch (choice) {
            case 1: store.printProducts(); break;
            case 2: store.printGraph(); break;
            case 3: showBFS(store); break;
            case 4: showDFS(store); break;
            case 5: showDijkstra(store); break;
            case 6: showTrending(store); break;
            case 7: addProductFlow(store); break;
            case 8: addConnectionFlow(store); break;
            case 0: std::cout << "Goodbye!\n"; break;
            default: std::cout << "Invalid choice. Pick 0-8.\n";
        }
    }
    return 0;
}

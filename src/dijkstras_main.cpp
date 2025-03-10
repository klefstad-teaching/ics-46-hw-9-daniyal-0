#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include "dijkstras.h"

using namespace std;

void testDijkstra(const string& filename) {
    cout << "=== Testing Dijkstra with file: " << filename << " ===" << endl;

    Graph G;
    try {
        file_to_graph(filename, G);
    } catch (const exception& e) {
        cerr << "Error reading file " << filename << ": " << e.what() << endl;
        return;
    }

    vector<int> previous;
    vector<int> distances = dijkstra_shortest_path(G, /*source=*/0, previous);

    for (int v = 0; v < G.numVertices; ++v) {
        vector<int> path = extract_shortest_path(distances, previous, v);
        cout << "Shortest path 0 -> " << v << ": ";
        if (!path.empty()) {
            print_path(path, distances[v]);
        } else {
            cout << "No path found." << endl;
        }
        cout << endl;
    }
    cout << "=== End test for " << filename << " ===\n\n";
}

int main() {
    vector<string> testFiles = {
        "src/small.txt",
        "src/medium.txt",
        "src/large.txt",
        "src/largest.txt"
    };

    for (const auto& file : testFiles) {
        testDijkstra(file);
    }

    return 0;
}

#include "dijkstras.h"
#include <algorithm>
//g++ -std=c++20 -o hw9 src/dijkstras.cpp src/dijkstras_main.cpp

vector<int> dijkstra_shortest_path(const Graph& G, int source, vector<int>& previous)
{
    int numVertices = G.numVertices;
    vector<int> distances(numVertices, INF);
    vector<bool> visited(numVertices, false);

    distances[source] = 0;
    previous.assign(numVertices, -1);

    typedef pair<int, int> Pair;
    priority_queue<Pair, vector<Pair>, greater<Pair> > minHeap;
    minHeap.push(Pair(0, source));

    while (!minHeap.empty())
    {
        Pair current = minHeap.top();
        minHeap.pop();

        int currentDist = current.first;
        int u = current.second;

        if (visited[u])
            continue;

        visited[u] = true;

        for (size_t i = 0; i < G[u].size(); ++i)
        {
            const Edge& edge = G[u][i];
            int v = edge.dst;
            int weight = edge.weight;

            if (!visited[v] && currentDist + weight < distances[v])
            {
                distances[v] = currentDist + weight;
                previous[v] = u;
                minHeap.push(Pair(distances[v], v));
            }
        }
    }

    return distances;
}

vector<int> extract_shortest_path(const vector<int>& distances, 
                                  const vector<int>& previous, 
                                  int destination)
{
    if (destination < 0 || destination >= (int)distances.size() 
        || distances[destination] == INF) {
        return {};
    }

    vector<int> path;
    for (int v = destination; v != -1; v = previous[v]) {
        path.push_back(v);
    }

    reverse(path.begin(), path.end());
    return path;
}

void print_path(const vector<int>& path, int total)
{
    if (path.empty()) {
        if (total != INF) {
            cout << "\nTotal cost is " << total << "\n";
        } else {
            cout << "No path found.\n";
        }
        return;
    }

    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i] << " ";
    }
    cout << "\nTotal cost is " << total << "\n";
}

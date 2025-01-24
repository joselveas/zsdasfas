//Julian Andre Honores Henriquez

#include <iostream>
#include <vector>
#include <fstream>
#include <limits.h> // y esta de aca para usar el INT_MAX (no se porque no me lo reconoce sin esta libreria si si deberia)
#include <cctype> // profe este de aca lo puse para pasar el char ingresado a mayuscula
#include <algorithm> // y este lo puse solo para usar reverse

using namespace std;


class Node {
public:
    char name;
    vector<pair<Node*, int>> connections;

    Node(char _name) {
        name = _name;
    }
};

class Graph {
private:
    vector<Node*> nodes;
    int num_nodes;

public:
    Graph() {
        num_nodes = 0;
    }

    ~Graph() {
        for (Node* node : nodes) {
            delete node;
        }
    }

    void readFile(string file_name) {
        ifstream file(file_name);

        if (!file.is_open()) {
            cout << "Problemas al abrir archivo" << endl;
            return;
        }

        file >> num_nodes;
        nodes.resize(num_nodes);

        for (int i = 0; i < num_nodes; ++i) {
            nodes[i] = new Node('A' + i);
        }

        int actual_nodes = 0;
        for (int i = 0; i < num_nodes; ++i) {
            for (int j = 0; j < num_nodes; ++j) {
                int weight;
                if (!(file >> weight)) {
                    cout << "Error: la matriz no contiene los elmntos suficientes" << endl;
                    return;
                }
                if (weight != 0) {
                    nodes[i]->connections.push_back({nodes[j], weight});
                }
            }
            actual_nodes++;
        }

        if (actual_nodes != num_nodes) {
            cout << "Error: el numero de nodos en la matriz no cuadra con el numero de nodos de la matriz." << endl;
            return;
        }

        file.close();
    }

    void showNodes() {
        for (Node* node : nodes) {
            cout << node->name << " ";
        }
        cout << endl;
    }

    void askNode() {
        showNodes();

        char node;
        cout << "A que nodo quieres llegar? ";
        cin >> node;

        node = toupper(node);

        if (node == 'A') {
            cout << "Ya estas en el nodo A" << endl;
            return;
        }

        int nodeNumeric = node - 'A';
        if (nodeNumeric < 0 || nodeNumeric >= num_nodes) {
            cout << "El nodo no existe" << endl;
            return;
        }

        vector<int> prev = dijkstra(0); // el nodo de inicio siempre sera A
        printPath(prev, nodeNumeric);
        cout << endl;
        printDistance(prev, nodeNumeric);
    }

    int selectMinVertex(vector<int>& value, vector<bool>& processed) {
        int minimum = INT_MAX;
        int vertex = -1;
        for (int i = 0; i < num_nodes; ++i) {
            if (!processed[i] && value[i] < minimum) {
                vertex = i;
                minimum = value[i];
            }
        }
        return vertex;
    }

    vector<int> dijkstra(int start) {
        vector<int> dist(num_nodes, INT_MAX);
        vector<int> prev(num_nodes, -1);
        vector<bool> processed(num_nodes, false);
        dist[start] = 0;

        for (int i = 0; i < num_nodes - 1; ++i) {
            int u = selectMinVertex(dist, processed);
            if (u == -1) break; // Esto de aca es porque no hay mas vertices accesibles
            processed[u] = true;

            for (auto& [v, weight] : nodes[u]->connections) {
                int v_index = v->name - 'A';
                if (!processed[v_index] && dist[u] != INT_MAX && dist[u] + weight < dist[v_index]) {
                    dist[v_index] = dist[u] + weight;
                    prev[v_index] = u;
                }
            }
        }

        return prev;
    }

    void printPath(vector<int>& prev, int node) {
        if (node == -1) return;
        printPath(prev, prev[node]);
        if (prev[node] != -1) cout << "->";
        cout << static_cast<char>('A' + node);
    }

    void printDistance(vector<int>& prev, int node) {
        int distance = 0;
        int current = node;
        while (prev[current] != -1) {
            for (auto& [v, weight] : nodes[prev[current]]->connections) {
                if (v->name - 'A' == current) {
                    distance += weight;
                    break;
                }
            }
            current = prev[current];
        }

        if (distance == 0) {
            cout << "No hay camino" << endl;
            return;

        }

        cout << "\nCosto total: " << distance << endl;
    }
};

int main() {
    Graph *graph = new Graph();

    // con esto generamos nuesta matriz
    string file_name = "matrixTest.txt";
    graph->readFile(file_name);

    // le preguntamos al usuario a que nodo quiere ir
    graph->askNode();

    delete graph;
    return 0;
}
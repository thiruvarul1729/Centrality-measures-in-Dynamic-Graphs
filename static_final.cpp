#include<iostream>
#include<string>
#include<algorithm>
#include<math.h>
#include<limits>
#include<vector>
#include<set>
#include<unordered_set>
#include<unordered_map>
#include<map>
#include <queue>
using namespace std;

class Graph {
    private: int numVertices;
    vector < vector < int >> E;
    public: Graph(int n) {
        numVertices = n;
        for (int i = 0; i < n; i++) {
            vector < int > v;
            E.push_back(v);
        }
    }
    void AddEdge(int rm) {
        E[rm / numVertices].push_back(rm % numVertices);
        E[rm % numVertices].push_back(rm / numVertices);
    }
    vector < double > CC() {
        vector < double > cc;
        for (int i = 0; i < numVertices; i++) {

            unordered_map < int, bool > mark;
            int dist[numVertices];
            dist[i] = 0;
            int sum = 0;
            queue < int > q;
            q.push(i);
            while (!q.empty()) {
                int curr = q.front();
                q.pop();
                if (mark.find(curr) == mark.end()) {
                    sum += dist[curr];
                    mark[curr] = true;
                    for (int i = 0; i < E[curr].size(); i++) {
                        q.push(E[curr][i]);
                        dist[E[curr][i]] = dist[curr] + 1;
                    }
                }
            }
            if (sum == 0) cc.push_back(0); //in case of isolated vertices
            else cc.push_back(1 / (double) sum);
        }
        return cc;
    }
};

int main() {
    int n, m;
    cin >> n >> m;
    Graph G = Graph(n);
    for (int i = 0; i < m; i++) {
        int rm;
        cin >> rm;
        G.AddEdge(rm);
    }
    vector < double > v = G.CC();
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
}
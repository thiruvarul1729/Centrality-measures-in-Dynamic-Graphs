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

#define INF 1000000000

template < class T > struct compare: binary_function < T, T, bool > {
    bool operator()(const pair < T, T > & a,
        const pair < T, T > & b) const {
        if (a.second == b.second) {
            return a.first < b.first;
        }
        return a.second < b.second;
    }
};

class Graph {
    private: int numVertices;
    vector < vector < bool >> adjMat;
    vector < vector < int >> D;
    vector < float > CC;

    public: Graph(int n) {
        numVertices = n;
        for (int i = 0; i < n; i++) {
            vector < bool > v(numVertices, 0);
            adjMat.push_back(v);
            vector < int > d(numVertices, INF);
            D.push_back(d);

            D[i][i] = 0;
            CC.push_back(0);
        }
    }

    void AddEdge(int rm) {
        adjMat[rm / numVertices][rm % numVertices] = 1;
        adjMat[rm % numVertices][rm / numVertices] = 1;
        insertEdgeGrowing(rm / numVertices, rm % numVertices);
    }

    void insertEdgeGrowing(int src, int dest) {
        vector < int > affectedSinks = insertUpdateGrowing(dest, src, src);
        vector < int > affectedSources = insertUpdateGrowing(src, dest, dest);

        for (auto s: affectedSinks) {

            vector < int > dump;
            dump = insertUpdateGrowing(src, dest, s);
        }

        for (auto s: affectedSources) {
            vector < int > dump;
            dump = insertUpdateGrowing(dest, src, s);
        }
    }

    vector < int > insertUpdateGrowing(int src, int dest, int z) {
        queue < pair < int, int >> workset;
        workset.push({
            src,
            dest
        });
        unordered_map < int, bool > visitedVert;
        visitedVert[src] = 1;
        visitedVert[dest] = 1;
        vector < int > affectedVert;

        int ii = 0;
        while (!workset.empty()) {
            pair < int, int > xy = workset.front();
            workset.pop();
            int x = xy.first;
            int y = xy.second;
            if (1 + D[y][z] < D[x][z]) {
                affectedVert.push_back(x);
                float totDist = CC[x];

                if (D[x][z] != INF) {
                    totDist = totDist - D[x][z] + 1 + D[y][z];
                } else {
                    totDist = totDist + 1 + D[y][z];
                }

                CC[x] = totDist;
                D[x][z] = 1 + D[y][z];
                for (int u = 0; u < numVertices; u++) {
                    if (adjMat[u][x] == 1 && (D[x][src] + 1 == D[u][src]) && visitedVert[u] == 0) {
                        workset.push({u,x});
                        visitedVert[u] = 1;

                    }
                }
            }
            ii++;
        }
        return affectedVert;
    }

    void DeleteEdge(int rm) {
        adjMat[rm / numVertices][rm % numVertices] = 0;
        adjMat[rm % numVertices][rm / numVertices] = 0;
        deleteEdgeShrinking(rm / numVertices, rm % numVertices);
    }

    void deleteEdgeShrinking(int src, int dest) {

        set < int > affectedSinks = deleteUpdateShrinking(dest, src, src);
        set < int > affectedSources = deleteUpdateShrinking(src, dest, dest);

        for (auto s: affectedSinks) {
            set < int > dump;
            dump = deleteUpdateShrinking(src, dest, s);
        }
        for (auto s: affectedSources) {
            set < int > dump;
            dump = deleteUpdateShrinking(dest, src, s);
        }
    }

    set < int > deleteUpdateShrinking(int src, int dest, int z) {
        set < int > affectedVert;
        bool at_least_one_exists = 0;
        for (int x = 0; x < numVertices; x++) {
            if (adjMat[x][src] == 1 && x != dest && (D[x][z] + 1 == D[src][z])) {
                at_least_one_exists = 1;
                break;
            }
        }
        if (!at_least_one_exists) {
            queue < int > workset;
            workset.push(src);
            unordered_map < int, bool > visitedVert;
            visitedVert[src] = 1;

            int ii = 0;
            while (!workset.empty()) {
                int u = workset.front();
                workset.pop();
                affectedVert.insert(u);

                for (int x = 0; x < numVertices; x++) {
                    if (adjMat[u][x] == 1) {
                        if ((D[u][z] + 1 == D[x][z]) && visitedVert[x] == 0) {
                            bool at_least_one_non_passing = 0;
                            for (int y = 0; y < numVertices; y++) {
                                if (adjMat[y][x] == 1 && (D[y][z] + 1 == D[x][z]) && affectedVert.find(y) == affectedVert.end()) {
                                    at_least_one_non_passing = 1;
                                }
                            }
                            if (!at_least_one_non_passing) {
                                workset.push(x);
                            }
                        }
                        visitedVert[x] = 1;
                    }
                }
                ii++;
            }

            auto cmp = [](pair < int, int > a, pair < int, int > b) {
                if (a.second == b.second) {
                    return a.first < b.first;
                }
                return a.second < b.second;
            };

            set < pair < int, int > , compare < int >> pq;

            for (auto a: affectedVert) {
                int min_dist = INF;
                for (int b = 0; b < numVertices; b++) {
                    if (adjMat[a][b] == 1 && affectedVert.find(b) == affectedVert.end() && b != dest && (1 + D[b][z] < min_dist)) {
                        min_dist = 1 + D[b][z];
                    }
                }
                pair < int, int > p;
                p.first = a;
                p.second = min_dist;
                pq.insert(p);
                float totDist = CC[a];
                if (D[a][z] != INF) {
                    totDist = totDist - D[a][z];
                }

                D[a][z] = min_dist;

                if (D[a][z] != INF) {
                    totDist = totDist + D[a][z];
                }
                CC[a] = totDist;

            }
            while (!pq.empty()) {

                pair < int, int > p1 = * (pq.begin());
                pq.erase( * (pq.begin()));
                int a = p1.first;

                for (int c = 0; c < numVertices; c++) {

                    pair < int, int > p2 = {c,D[c][z]};

                    if ((adjMat[a][c] == 1) && (1 + D[a][z] < D[c][z])) {
                        float totDist = CC[c];

                        if (D[c][z] != INF) {
                            totDist = totDist - D[c][z];
                        }

                        D[c][z] = 1 + D[a][z];

                        if (D[c][z] != INF) {
                            totDist = totDist + D[c][z];
                        }
                        CC[c] = totDist;

                        if (pq.find(p2) == pq.end()) {
                            pq.insert({c,D[c][z]});
                        } else {
                            pq.erase(p2);
                            pq.insert({c,D[c][z]});
                        }
                    }
                }
            }
        }

        return affectedVert;
    }

    void print() {
        for (int i = 0; i < numVertices; i++) {
            cout << 1 / CC[i] << " ";
        }
        cout << endl;
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
        G.print();
    }
    for (int i = 0; i < m; i++) {
        int rm;
        cin >> rm;
        G.DeleteEdge(rm);
        G.print();
    }
}
#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> parent, rank_, size_;

    DSU(int n) : parent(n), rank_(n, 0), size_(n, 1) {
        iota(parent.begin(), parent.end(), 0); 
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    bool unite(int u, int v) {
        u = find(u); v = find(v);
        if (u == v) return false;          
        if (rank_[u] < rank_[v]) swap(u, v);
        parent[v] = u;
        size_[u] += size_[v];
        if (rank_[u] == rank_[v]) rank_[u]++;
        return true;
    }

    bool connected(int u, int v) { return find(u) == find(v); }
    int  compSize(int x)         { return size_[find(x)]; }
};


//  Problem 1
int countComponents(int n, vector<pair<int,int>>& edges) {
    DSU dsu(n);
    int components = n;
    for (auto& e : edges) {
        int u = e.first, v = e.second;
        if (dsu.unite(u, v))
            components--;
    }                          // <-- was missing
    return components;
}


//  Problem 2
bool hasCycle(int n, vector<pair<int,int>>& edges) {
    DSU dsu(n);
    for (auto& e : edges) {
        int u = e.first, v = e.second;
        if (!dsu.unite(u, v))      
            return true;
    }                          // <-- was missing
    return false;
}


//  Problem 3 
int findProvinces(vector<vector<int>>& isConnected) {
    int n = isConnected.size();
    DSU dsu(n);
    int provinces = n;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)  
            if (isConnected[i][j] == 1)
                if (dsu.unite(i, j))
                    provinces--;
    return provinces;
}


//  Problem 4 
struct GroupResult {
    int totalGroups;
    map<int, vector<int>> groups;
};

GroupResult friendGroups(int n, vector<pair<int,int>>& friendships) {
    DSU dsu(n);
    for (auto& e : friendships) {   // <-- was auto [a, b]
        int a = e.first, b = e.second;
        dsu.unite(a, b);
    }

    map<int, vector<int>> groups;
    for (int i = 0; i < n; i++)
        groups[dsu.find(i)].push_back(i);  

    return { (int)groups.size(), groups };
}


int main() {
    cout << "=== Problem 1: Connected Components ===\n";
    int n1 = 5;
    vector<pair<int,int>> edges1 = {{0,1},{1,2},{3,4}};
    cout << "Components: " << countComponents(n1, edges1) << "\n\n"; 

    cout << "=== Problem 2: Cycle Detection ===\n";
    int n2 = 4;
    vector<pair<int,int>> edges2 = {{0,1},{1,2},{2,0},{2,3}};
    cout << "Has cycle: " << (hasCycle(n2, edges2) ? "yes" : "no") << "\n\n"; 

    vector<pair<int,int>> edges2b = {{0,1},{1,2},{2,3}};
    cout << "Has cycle: " << (hasCycle(n2, edges2b) ? "yes" : "no") << "\n\n"; 

    cout << "=== Problem 3: Provinces ===\n";
    vector<vector<int>> mat = {
        {1,1,0},
        {1,1,0},
        {0,0,1}
    };
    cout << "Provinces: " << findProvinces(mat) << "\n\n";

    cout << "=== Problem 4: Friend Groups ===\n";
    int n4 = 7;
    vector<pair<int,int>> friendships = {{0,1},{1,2},{3,4},{5,6}};
    GroupResult res = friendGroups(n4, friendships);
    cout << res.totalGroups << " Groups\n";
    int idx = 1;
    for (auto& entry : res.groups) {
        auto& members = entry.second;
        cout << "Group " << idx++ << ": {";
        for (int i = 0; i < (int)members.size(); i++) {
            cout << members[i];
            if (i + 1 < (int)members.size()) cout << ",";
        }
        cout << "} size " << members.size() << "\n";
    }

    return 0;
}
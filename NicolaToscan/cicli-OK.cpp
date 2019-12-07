#include <fstream>
#include <vector>
#include <queue>

using namespace std;

int N, M, NREQ;
int *depth;
int *parent;

void treeinator(vector<int> *g, int root)
{
    queue<int> q;
    bool* visited = new bool[N];

    depth = new int[N];
    parent = new int[N];

    for (int i = 0; i < N; i++)
        visited[i] = false;

    visited[root] = true;
    depth[root] = 0;
    parent[root] = -1;

    q.push(root);

    while (!q.empty())
    {
        int node = q.front();
        q.pop();

        for (int n : g[node])
        {
            if (!visited[n])
            {
                visited[n] = true;
                depth[n] = depth[node] + 1;
                parent[n] = node;
                q.push(n);
            }
        }
    }
}

int findDistance(vector<int> *g, int p1, int p2)
{
    int d1 = depth[p1];
    int d2 = depth[p2];
    if (d1 < d2)
    {
        int t = p1;
        p1 = p2;
        p2 = t;

        t = d1;
        d1 = d2;
        d2 = t;
    }

    int dist = d1 - d2;
    for (int i = 0; i < dist; i++)
        p1 = parent[p1];

    if (p1 == p2)
        return dist;

    while (parent[p1] != parent[p2])
    {
        dist += 2;
        p1 = parent[p1];
        p2 = parent[p2];
    }

    for (int n : g[p1])
        if (n == p2 && depth[n] == depth[p1])
            return dist + 1;

    return dist + 2;
}

int main()
{
    ifstream in("input.txt");
    in >> N;
    in >> M;
    in >> NREQ;

    vector<int> *g = new vector<int>[N];
    for (int i = 0; i < M; i++)
    {
        int s, e;
        in >> s;
        in >> e;
        g[s].push_back(e);
        g[e].push_back(s);
    }

    treeinator(g, 0);

    ofstream out("output.txt");
    for (int i = 0; i < NREQ; i++)
    {
        int n1, n2;
        in >> n1;
        in >> n2;
        out << findDistance(g, n1, n2) << endl;
    }

    return 0;
}
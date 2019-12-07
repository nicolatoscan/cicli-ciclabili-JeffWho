#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <utility>
#include <queue>
#include <stack>
#include <iomanip>

using namespace std;

void treeinator(vector<int> *g, int root);
int findDistance(vector<int> *g, int p1, int p2);

int N, M, NREQ;
int nCricche = 0, nBacknbone = 0;
const int MAX_LENGTH = 100000;
int *depth;
int *parent;
bool *visited;

queue<int> q;

void treeinator(vector<int> *g, int root)
{
    visited = new bool[N];
    depth = new int[N];
    parent = new int[N];
    for (int i = 0; i < N; i++)
    {
        visited[i] = false;
        depth[i] = -1;
        parent[i] = -1;
    }

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
    int dist = 0;

    int lastP1 = -1;
    int lastP2 = -2;

    while (p1 != p2)
    {
        lastP1 = p1;
        lastP2 = p2;
        dist++;
        if (depth[p1] > depth[p2])
            p1 = parent[p1];
        else if (depth[p1] < depth[p2])
            p2 = parent[p2];
        else
        {
            p1 = parent[p1];
            p2 = parent[p2];
            dist++;
        }
    }

    //cout << lastP1 << " " << lastP2 << endl;

    if (lastP1 < 0 && lastP2 < 0)
        return dist;

    for (int n : g[lastP1])
    {
        if (n == lastP2 && depth[n] == depth[lastP1])
        {
            dist--;
            break;
        }
    }

    return dist;
}

int main()
{
    clock_t start, end;
    double time_taken;

    ifstream in("input.txt");
    in >> N;
    in >> M;
    in >> NREQ;

    vector<int> *g = new vector<int>[N];

    start = clock();
    for (int i = 0; i < M; i++)
    {
        int s, e;
        in >> s;
        in >> e;
        g[s].push_back(e);
        g[e].push_back(s);
    }
    end = clock();
    cout << "Input grafo : " << fixed << double(end - start) / double(CLOCKS_PER_SEC) << setprecision(5) << endl;

    start = clock();
    treeinator(g, 0);
    end = clock();
    cout << "Creazione albero : " << fixed << double(end - start) / double(CLOCKS_PER_SEC) << setprecision(5) << endl;

    start = clock();
    ofstream out("output.txt");
    for (int i = 0; i < NREQ; i++)
    {
        int p1, p2;
        in >> p1;
        in >> p2;
        out << findDistance(g, p1, p2) << endl;
    }
    end = clock();
    cout << "Calcolo distanze : " << fixed << double(end - start) / double(CLOCKS_PER_SEC) << setprecision(5) << endl;

    return 0;
}
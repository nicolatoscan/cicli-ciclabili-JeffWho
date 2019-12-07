#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <utility>
#include <queue>
#include <stack>
#include <iomanip>

#define WHITE 1
#define BLACK 2

using namespace std;

void printGrafo(vector<int> *g, int len);


struct subset
{
    int parent, rank, ancestor, color;
};

int N, M, NREQ;
int nCricche = 0, nBacknbone = 0;
int *depth;
int *parent;
bool *visited;
struct subset* subsets;

queue<int> q;

void treeinator(vector<int> *g, int root, vector<int>* tree)
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
                tree[node].push_back(n);
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


void makeSet(int i)
{
    subsets[i].color = WHITE;
    subsets[i].parent = i;
    subsets[i].rank = 0;

    return;
}

int findSet(int i)
{
    if (subsets[i].parent != i)
        subsets[i].parent = findSet(subsets[i].parent);

    return subsets[i].parent;
}

void unionSet(int x, int y)
{
    int xroot = findSet(x);
    int yroot = findSet(y);

    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else
    {
        subsets[yroot].parent = xroot;
        (subsets[xroot].rank)++;
    }
}

void tarzan(vector<int> *t, int n, vector<pair<int, int>> *query, int* resposes)
{
    makeSet(n);
    subsets[n].ancestor = n;

    for (int c : t[n])
    {
        tarzan(t, c, query, resposes);
        unionSet(n, c);
        subsets[findSet(n)].ancestor = n;
    }

    subsets[n].color = BLACK;

    for (auto q : query[n]) {
        resposes[q.second] = subsets[findSet(q.first)].ancestor;
    }
}

void initTarzan() {
    for (int i = 0; i < N; i++) {
        subsets[i].color = WHITE;
        subsets[i].parent = parent[i];
        subsets[i].ancestor = i;
        subsets[i].rank = 0;
    }
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
    vector<int> *tree = new vector<int>[N];
    subsets = new subset[N];

    for (int i = 0; i < M; i++)
    {
        int s, e;
        in >> s;
        in >> e;
        g[s].push_back(e);
        g[e].push_back(s);
    }

    treeinator(g, 0, tree);

    start = clock();

    vector<pair<int, int>> *query = new vector<pair<int, int>>[N];
    int* responses = new int[NREQ];
    
    for (int i = 0; i < NREQ; i++)
    {
        int p1, p2;
        in >> p1;
        in >> p2;
        query[p1].push_back(make_pair(p2, i));
    }
    initTarzan();
    tarzan(tree, 0, query, responses);
    end = clock();
    
    ofstream out("output.txt");
    for (int i = 0; i < NREQ; i++)
        cout << responses[i] << endl;

    return 0;
}

void printGrafo(vector<int> *g, int len)
{
    for (int i = 0; i < len; i++)
    {
        cout << i << "\t=> ";
        for (int n : g[i])
            cout << n << " ";
        cout << endl;
    }
    cout << endl;
}

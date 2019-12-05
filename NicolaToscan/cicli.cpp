#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <utility>
#include <queue>
#include <stack>

using namespace std;

void findArticulations(vector<int> *g, int n, bool *visited, int *disc, int *low, int *parent);
void findArticulationsWrapper(vector<int> *g);
void criccaFinder(vector<int> *g, vector<int> *cricca);
bool isBackBone[10000];

int N, M, NREQ;
int nCricche = 0;

int main()
{
    ifstream in("input.txt");
    in >> N;
    in >> M;
    in >> NREQ;

    pair<int, int> req[N];
    vector<int> g[N];
    vector<int> cricca[N];

    for (int i = 0; i < M; i++)
    {
        int s, e;
        in >> s;
        in >> e;
        g[s].push_back(e);
        g[e].push_back(s);
    }

    findArticulationsWrapper(g);
    criccaFinder(g, cricca);

    cout << "BACKBONE" << endl;
    for (int i = 0; i < N; i++)
        if (isBackBone[i])
            cout << i << " ";
    cout << endl << endl;

    cout << "CRICCHE" << endl;
    for (int i = 0; i < nCricche; i++)
    {
        cout << i << " => ";
        for (int n : cricca[i])
            cout << n << " ";
        cout << endl;
    }
}

void findArticulations(vector<int> *g, int n, bool *visited, int *discovered, int *low, int *parent)
{
    static int depth = 0;
    int children = 0;

    depth++;

    visited[n] = true;
    discovered[n] = depth;
    low[n] = depth;

    for (int near : g[n])
    {
        if (!visited[near])
        {
            children++;
            parent[near] = n;
            findArticulations(g, near, visited, discovered, low, parent);

            low[n] = min(low[near], low[n]);

            if (parent[n] == -1 && children > 1)
                isBackBone[n] = true;

            if (parent[n] != -1 && low[near] >= discovered[n])
                isBackBone[n] = true;
        }
        else if (near != parent[n])
        {
            low[n] = min(low[near], discovered[n]);
        }
    }
}

void findArticulationsWrapper(vector<int> *g)
{
    bool visited[N];
    int discovered[N];
    int low[N];
    int parent[N];

    for (int i = 0; i < N; i++)
    {
        visited[i] = false;
        parent[i] = -1;
        isBackBone[i] = false;
    }

    for (int i = 0; i < N; i++)
        if (!visited[i])
            findArticulations(g, i, visited, discovered, low, parent);
}

void criccaFinder(vector<int> *g, vector<int> *cricca)
{
    int visited[N];
    for (int i = 0; i < N; i++)
        visited[i] = isBackBone[i];

    for (int i = 0; i < N; i++)
    {
        if (!visited[i])
        {
            cricca[nCricche].push_back(i);
            for (int near : g[i])
            {
                cricca[nCricche].push_back(near);
                visited[near] = true;
            }
            visited[i] = true;
            nCricche++;
        }
    }
}
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <utility>
#include <queue>
#include <stack>
#include <iomanip>

using namespace std;

void printGrafo(vector<int> *g, int len);

int N, M, NREQ;
int nCricche = 0, nBacknbone = 0;
const int MAX_LENGTH = 100000;
int isBackBone[MAX_LENGTH];

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
    bool *visited = new bool[N];
    int *discovered = new int[N];
    int *low = new int[N];
    int *parent = new int[N];

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
    int j = 0;
    int *visited = new int[N];
    for (int i = 0; i < N; i++)
        visited[i] = isBackBone[i] || (g[i].size() == 0);

    //nCricche = 0;
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

void backBoneGraph(vector<int> *g, vector<int> *backbone)
{
    nBacknbone = 0;
    for (int i = 0; i < N; i++)
        if (isBackBone[i])
            nBacknbone++;

    for (int i = 0; i < N; i++)
        if (isBackBone[i])
            for (int n : g[i])
                if (isBackBone[n])
                    backbone[i].push_back(n);
}

int main()
{

    ifstream in("input.txt");
    in >> N;
    in >> M;
    in >> NREQ;

    vector<int> *g = new vector<int>[N];
    vector<int> *backbone = new vector<int>[N];
    vector<int> *cricca = new vector<int>[200];

    for (int i = 0; i < M; i++)
    {
        int s, e;
        in >> s;
        in >> e;
        g[s].push_back(e);
        g[e].push_back(s);
    }

    do
    {
        findArticulationsWrapper(g);
        criccaFinder(g, cricca);

        cout << "GRAFO" << endl;
        printGrafo(g, N);
        cout << "CRICCA" << endl;
        printGrafo(cricca, nCricche);

        backBoneGraph(g, backbone);
        g = backbone;
        backbone = new vector<int>[N];
    } while (nBacknbone > 1);

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
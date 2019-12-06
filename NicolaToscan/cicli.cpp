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
void criccaFinder(vector<int> *g, vector<int> *cricca, vector<int> *criccaToBackbone);
void backBoneGraph(vector<int> *g);
void DJ(vector<int> *g, int start);

bool isBackBone[10000];

int N, M, NREQ;
int nCricche = 0;

int main()
{

    ifstream in("input.txt");
    in >> N;
    in >> M;
    in >> NREQ;

    vector<int> *g = new vector<int>[N];
    vector<int> *cricca = new vector<int>[N];
    vector<int> *criccaToBackbone = new vector<int>[N];

    for (int i = 0; i < M; i++)
    {
        int s, e;
        in >> s;
        in >> e;
        g[s].push_back(e);
        g[e].push_back(s);
    }

    findArticulationsWrapper(g);
    criccaFinder(g, cricca, criccaToBackbone);
    backBoneGraph(g);

    cout << "BACKBONE" << endl;
    for (int i = 0; i < N; i++)
        if (isBackBone[i])
            cout << i << " ";
    cout << endl
         << endl;

    cout << "CRICCHE" << endl;
    for (int i = 0; i < nCricche; i++)
    {
        cout << i << "\t=> ";
        for (int n : cricca[i])
            cout << n << " ";
        cout << endl;
    }
    cout << endl;

    cout << "CRICCA TO BACKBONE" << endl;
    for (int i = 0; i < nCricche; i++)
    {
        cout << i << "\t=> ";
        for (int n : criccaToBackbone[i])
            cout << n << " ";
        cout << endl;
    }
    cout << endl;

    cout << "BACKBONE GRAPH" << endl;
    for (int i = 0; i < N; i++)
    {
        if (isBackBone[i])
        {
            cout << i << "\t=> ";
            for (int n : g[i])
                cout << n << " ";
            cout << endl;
        }
    }
    cout << endl;

    for (int i = 0; i < N; i++)
    {
        if (isBackBone[i])
        {
            cout << "Distance from " << i << endl;
            DJ(g, i);
        }
    }
    return 0;
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

vector<int> criccaBackboneGraph[100];
void criccaFinder(vector<int> *g, vector<int> *cricca, vector<int> *criccaToBackbone)
{
    int j = 0;
    int *visited = new int[N];
    for (int i = 0; i < N; i++)
        visited[i] = isBackBone[i];

    nCricche = 0;
    for (int i = 0; i < N; i++)
    {
        if (!visited[i])
        {
            cricca[nCricche].push_back(i);
            for (int near : g[i])
            {
                if (isBackBone[near])
                    criccaToBackbone[nCricche].push_back(near);
                else
                    cricca[nCricche].push_back(near);
                visited[near] = true;
            }
            visited[i] = true;
            nCricche++;
        }
    }
}

void backBoneGraph(vector<int> *g)
{
    for (int i = 0; i < N; i++)
    {
        if (!isBackBone[i])
        {
            g[i].clear();
        }
        else
        {
            vector<int> toReplace;
            for (int n : g[i])
                if (isBackBone[n])
                    toReplace.push_back(n);
            g[i] = toReplace;
        }
    }
}

priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q;
int MAX_LENGTH = 100000;
void DJ(vector<int> *g, int start)
{
    bool *visited = new bool[N];
    int *distanceTo = new int[N];
    int *pred = new int[N];
    for (int i = 0; i < N; i++)
    {
        visited[i] = false;
        distanceTo[i] = MAX_LENGTH;
        pred[i] = -1;
    }
    distanceTo[start] = 0;
    q.push(make_pair(0, start));

    while (!q.empty())
    {
        pair<int, int> next = q.top();
        q.pop();

        if (next.first >= MAX_LENGTH)
            break;

        if (!visited[next.second])
        {
            visited[next.second] = true;
            for (int n : g[next.second])
            {
                int alt = 1 + distanceTo[next.second];

                if (alt < distanceTo[n])
                {
                    pred[n] = next.second;
                    distanceTo[n] = alt;
                    q.push(make_pair(alt, n));
                }
            }
        }
    }

    for (int i = 0; i < N; i++)
        if (isBackBone[i])
            cout << i << ":" << distanceTo[i] << "\t";
    cout << endl;
}
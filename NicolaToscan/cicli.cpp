#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <utility>
#include <queue>
#include <stack>
#include <iomanip>

using namespace std;

void findArticulations(vector<int> *g, int n, bool *visited, int *disc, int *low, int *parent);
void findArticulationsWrapper(vector<int> *g);
void criccaFinder(vector<int> *g, vector<int> *cricca, vector<int> *criccaToBackbone);
void backBoneGraph(vector<int> *g, vector<int> *backbone);
void DJ(vector<int> *g, int start, int **distanceTo, bool log);

bool isBackBone[10000];
int nBacknbone = 0;
int fromBackboneToN[10000];
int fromNToBackbone[10000];

int N, M, NREQ;
int nCricche = 0;

const int MAX_LENGTH = 100000;

int main()
{

    ifstream in("input.txt");
    in >> N;
    in >> M;
    in >> NREQ;

    vector<int> *g = new vector<int>[N];
    vector<int> *backbone = new vector<int>[N];
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
    backBoneGraph(g, backbone);

    if (true)
    {
        cout << "BACKBONE" << endl;
        for (int i = 0; i < N; i++)
            if (isBackBone[i])
                cout << i << " ";
        cout << endl
             << endl;

        cout << "CRICCHE" << endl;
        for (int i = 0; i < nCricche; i++)
        {
            cout << i << " => ";
            for (int n : cricca[i])
                cout << n << " ";
            cout << "\t\tB: ";
            for (int n : criccaToBackbone[i])
                cout << n << " ";
            cout << endl;
        }
        cout << endl;

        cout << "BACKBONE GRAPH" << endl;
        for (int i = 0; i < nBacknbone; i++)
        {
            cout << fromBackboneToN[i] << "\t=> ";
            for (int n : backbone[i])
                cout << fromBackboneToN[n] << " ";
            cout << endl;
        }
        cout << endl;
    }

    int **distanceTo = new int *[nBacknbone];
    for (int i = 0; i < nBacknbone; i++)
    {
        distanceTo[i] = new int[nBacknbone];
        for (int j = 0; j < nBacknbone; j++)
            distanceTo[i][j] = i == j ? 0 : MAX_LENGTH;
    }

    for (int i = 0; i < nBacknbone; i++)
        DJ(backbone, i, distanceTo, false);

    for (int i = 0; i < nBacknbone; i++)
    {
        for (int j = 0; j < nBacknbone; j++)
            cout << distanceTo[i][j] << " ";
        cout << endl;
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

void backBoneGraph(vector<int> *g, vector<int> *backbone)
{

    for (int i = 0; i < N; i++)
    {
        if (isBackBone[i])
        {
            fromBackboneToN[nBacknbone] = i;
            fromNToBackbone[i] = nBacknbone;
            nBacknbone++;
        }
    }
    for (int i = 0; i < nBacknbone; i++)
        for (int n : g[fromBackboneToN[i]])
            if (isBackBone[n])
                backbone[i].push_back(fromNToBackbone[n]);
}

priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q;
void DJ(vector<int> *g, int start, int **distanceTo, bool log)
{
    bool *visited = new bool[nBacknbone];
    for (int i = 0; i < nBacknbone; i++)
        visited[i] = false;

    q.push(make_pair(0, start));

    int i = 0;
    while (!q.empty())
    {
        int nextNode;
        pair<int, int> next = q.top();
        q.pop();
        nextNode = next.second;

        if (log)
            cout << "Sto facendo " << fromBackboneToN[nextNode] << endl;

        if (!visited[nextNode])
        {
            visited[nextNode] = true;
            for (int n : g[nextNode])
            {
                if (log)
                    cout << "Lavoro tra " << fromBackboneToN[nextNode] << " e " << fromBackboneToN[n] << endl;

                int alt = 1 + distanceTo[start][nextNode];
                if (log)
                    cout << fromBackboneToN[nextNode] << " è a " << distanceTo[start][nextNode] << endl;
                if (log)
                    cout << fromBackboneToN[n] << " è a " << distanceTo[start][n] << endl;

                if (alt < distanceTo[start][n])
                {
                    distanceTo[start][n] = alt;
                    distanceTo[n][start] = alt;
                    if (log)
                        cout << "Pushing " << fromBackboneToN[n] << " with " << alt << endl;

                    q.push(make_pair(alt, n));
                }
                if (log)
                    cout << endl;
            }
        }
        if (log)
            cout << endl;
    }
    if (log)
    {

        cout << endl
             << "Distance from " << fromBackboneToN[start] << endl;
        for (int i = 0; i < nBacknbone; i++)
            cout << fromBackboneToN[i] << ":" << distanceTo[start][i] << "   ";
        cout << endl;
        for (int i = 0; i < nBacknbone; i++)
            cout << fromBackboneToN[i] << ":" << (visited[i] ? "SI" : "NO") << "   ";

        cout << endl
             << "-----------------------" << endl
             << endl;
    }
}
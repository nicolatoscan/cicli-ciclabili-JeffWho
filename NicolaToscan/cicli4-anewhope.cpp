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
    ofstream out("output.txt");
    double min = 10;
    int minI = 0;

    for (int i = 0; i < 40000; i += 10)
    {

        clock_t start, end;
        double time_taken;

        start = clock();

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

        treeinator(g, i);

        int *i1 = new int[NREQ];
        int *i2 = new int[NREQ];
        for (int i = 0; i < NREQ; i++)
        {
            in >> i1[i];
            in >> i2[i];
        }

        for (int i = 0; i < NREQ; i++)
            i1[i] = findDistance(g, i1[i], i2[i]);

        end = clock();
        //for (int i = 0; i < NREQ; i++)
        //    out << i1[i] << "\n";

        double res = double(end - start) / double(CLOCKS_PER_SEC);
            cout << i << "\t" << fixed << res << setprecision(7) << endl;
        if (res < min)
        {
            min = res;
            minI = i;
            cout << " -- BEST --- " << i << "\t" << fixed << res << setprecision(7) << endl;
            out << i << "\t" << fixed << res << setprecision(7) << endl;
        }
    }

    return 0;
}
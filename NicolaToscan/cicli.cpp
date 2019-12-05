#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <utility>
#include <queue>
#include <stack>

using namespace std;

int N, M, NREQ;

int main()
{

    ifstream in("input.txt");
    in >> N;
    in >> M;
    in >> NREQ;

    pair<int, int> req[100000];
    vector<int> g[100000];

    for (int i = 0; i < M; i++)
    {
        int s, e;
        in >> s;
        in >> e;
        g[s].push_back(e);
        g[e].push_back(s);
    }
    for (int i = 0; i < NREQ; i++)
    {
        in >> req[i].first;
        in >> req[i].second;
    }
}
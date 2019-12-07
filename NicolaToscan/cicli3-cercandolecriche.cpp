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

int criccaFinder(vector<int> *g, vector<int> *cricca)
{
    nCricche = 0;
    int *marker = new int[N];
    for (int i = 0; i < N; i++)
        marker[i] = 0;
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
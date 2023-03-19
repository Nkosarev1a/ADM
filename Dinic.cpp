#include<vector>
#include<iostream>
#include <list>
using namespace std;

int INT_MAX = 2147483647;

struct Rib
{
    int v; 
    int flw;
    int cp;  
    int revID; 
};

class residualGraph
{
    int NumV; 
    int* lvl; 
    vector<Rib>* adj;
public:
    residualGraph(int NumV)
    {
        adj = new vector<Rib>[NumV];
        this->NumV = NumV;
        lvl = new int[NumV];
    }

    void add_rib(int u, int v, int cap)
    {
        Rib a{v, 0, cap, adj[v].size()};
        Rib b{u, 0, 0, adj[u].size()};

        adj[u].push_back(a);
        adj[v].push_back(b); 
    }

    bool BFS(int s, int t);
    int sendFlow(int s, int flw, int t, int ptr[]);
    int DinicMaxflow(int s, int t);
};

bool residualGraph::BFS(int s, int t)
{
    for (int i = 0; i < NumV; i++)
        lvl[i] = -1;

    lvl[s] = 0; 
    list<int> visited;
    visited.push_back(s);

    vector<Rib>::iterator i;
    while (!visited.empty())
    {
        int u = visited.front();
        visited.pop_front();
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
        {
            Rib& e = *i;
            if (lvl[e.v] < 0 && e.flw < e.cp)
            {
                lvl[e.v] = lvl[u] + 1;

                visited.push_back(e.v);
            }
        }
    }
    return lvl[t] < 0 ? false : true;
}

int residualGraph::sendFlow(int u, int flw, int t, int start[])
{
    if (u == t)
        return flw;

    for (; start[u] < adj[u].size(); start[u]++)
    {
        Rib& e = adj[u][start[u]];

        if (lvl[e.v] == lvl[u] + 1 && e.flw < e.cp)
        {
            int curr_flow = min(flw, e.cp - e.flw);

            int temp_flow = sendFlow(e.v, curr_flow, t, start);

            if (temp_flow > 0)
            {
                e.flw += temp_flow;
                adj[e.v][e.revID].flw -= temp_flow;
                return temp_flow;
            }
        }
    }

    return 0;
}

int residualGraph::DinicMaxflow(int s, int t)
{

    if (s == t)
        return -1;

    int max_flow = 0;  
    while (BFS(s, t) == true)
    {
        int* start = new int[NumV + 1]{ 0 };
        while (int flw = sendFlow(s, INT_MAX, t, start))
            max_flow += flw;
    }
    return max_flow;
}

int main()
{
    int n, m, s, t;
    cin >> n >> m;
    cin >> s >> t;
    residualGraph g(n);
    for (int i = 0; i < m; i++) {
        int v1, v2, cap;
        cin >> v1 >> v2 >> cap;
        g.add_rib(v1, v2, cap);
    }
    cout << g.DinicMaxflow(s, t);
}

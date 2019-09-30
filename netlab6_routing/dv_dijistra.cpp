#include<bits/stdc++.h>
using namespace std;

int i,j;

int V,E,u,v;
typedef pair<int,int> pii;
vector < vector <pii>  > adj;
priority_queue <pii, vector<pii>, greater<> > pq;

int src,w,d;
vector <int> dist;

void dijkstra(){
    dist[src] = 0;
    pq.push({0,src});
    while(!pq.empty()){
        u = pq.top().second;
        pq.pop();
        for(auto it:adj[u]){
            v=it.first;
            w=it.second;
	    	if(dist[v]>(dist[u]+w)){
                dist[v] = dist[u]+w;
                pq.push({dist[v],v});
            }
        }
    }
}

int main(){
    cin>>V>>E;
    adj.resize(V);
    dist.resize(V,INT32_MAX);
    while(E--){
        cin>>u>>v>>w;
        adj[u].push_back({v,w});
	    adj[v].push_back({u,w});
    }
    cin>>src;
	dijkstra();
    for(auto it:dist) cout<<it<<endl;
    return 0;
}
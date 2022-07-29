#include "Graph.h"
#include <set>
#include <queue>

Graph::Graph() {};

void Graph::addNode(string trackName, string artist, string genre, float score, vector<int> neigh, string id) {
    Node* n = new Node;
    n->trackName = trackName;
    n->artist = artist;
    n->genre = genre;
    n->score = score;
    n->id = id;
    n->neighbors = neigh;

    graph.push_back(n);
}

//DIJKSTRA

vector<Graph::Node*> Graph::dijkstra(Node* start, Node* finish) {
    int fin = -1; //initialize position of finish node
    vector<int> p; //position of predecessor
    vector<float> d; //distance from start to node at position
    set<int> nodes;
    int strt = -1;

    cout << "in dijkstra" << endl;

    if (start == nullptr || finish == nullptr) return {};

    for (int i = 0; i < graph.size(); i++) {
        if (graph.at(i)->id == start->id) strt = i;
        if (graph.at(i)->id == finish->id) fin = i;
    }

    for (int i = 0; i < graph.size(); i++) //initialize visited/unvisited vector and p and d vectors
    {
        nodes.insert(i);
        p.push_back(-1);
        if (start->id == graph.at(i)->id) d.push_back(0);
        else d.push_back(1000000);
    }

    priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> pq;
    pq.push(make_pair(0, strt));

    int counter = 0;

    while (!nodes.empty())
    {
        int index = pq.top().second;
        pq.pop();

        if (index == -1) break;

        nodes.erase(index);
        if (index == fin) break;

        for (auto iter : graph.at(index)->neighbors)
        {
            int pos = isVisited(iter, nodes);
            if (pos != -1)
            {
                float wt = isEdge(index, iter);
                float a = d.at(index);
                float b = d.at(pos);
                if (d.at(index) + wt < d.at(pos))
                {
                    d.at(pos) = d.at(index) + wt;
                    p.at(pos) = index;
                    pq.push(make_pair(d.at(pos), pos));
                }
            }
        }

    }
    int curr = fin;

    vector < Node* > playlist;

    if (d.at(fin) == 1000000) return {};
    cout << curr << endl;

    while (curr != -1) {
        playlist.push_back(graph.at(curr));
        std::cout << graph.at(curr)->genre << endl;
        curr = p.at(curr);
    }
    cout << " just added start" << endl;

    return playlist;
}

float Graph::isEdge(int n, int m) {
    //returns the weight of the edge if the edge exists
    //returns 0 if they are the same
    //returns 1000000 otherwise;

    if (graph.at(n)->id == graph.at(m)->id) return 0;

    for (auto iter : graph.at(n)->neighbors) {
        if (iter == m) return abs(graph.at(n)->score - graph.at(m)->score);
    }

    return 1000000;
}

int Graph::isVisited(int n, set<int>& nodes) {
    //returns position of node if it is unvisited 
    //returns -1 if it is visited
    //cout << "in isvisited" << endl;

    if (nodes.find(n) != nodes.end()) return n;
    return -1;
}

vector<Graph::Node*> Graph::traceRoute(map<Node*, Node*>& prev, Node* end) {
    stack<Node*> order;
    Node* curr = end;
    vector<Node*> playlist;

    while (curr != nullptr) {
        order.push(curr);
        curr = prev[curr];
    }
    while (!order.empty()) {
        playlist.push_back(order.top());
        order.pop();
    }
    return playlist;
}

//BFS

vector<Graph::Node*> Graph::bfs(Node* start, Node* end) {
    //initializes maps of node + its predecessor
    map<Node*, Node*> prev;
    //checks if end reached
    bool reachedEnd = false;
    //queue to traverse
    queue<Node*> q;
    //set start node to null and push it into queue
    prev[start] = nullptr;
    q.push(start);

    //keep traversing until queue empty or until end reached
    while (!q.empty() && !reachedEnd) {
        //set front of queue as current node and pop it off queue
        Node* curr = q.front();
        q.pop();
        //traverse thru its neighbors to add to queue and map
        for (int i = 0; i < curr->neighbors.size(); i++) {
            //if not visited add to map
            if (prev.find(graph.at(curr->neighbors[i])) == prev.end()) {
                q.push(graph.at(curr->neighbors[i]));
                prev[graph.at(curr->neighbors[i])] = curr;
            }
            //if reached end, break out of while loop
            if (graph.at(curr->neighbors[i]) == end) {
                reachedEnd = true;
                break;
            }
        }
    }
    if (!reachedEnd) {
        cout << "Songs not connected" << endl;
        return {};
    }
    //trace back route
    else {
        vector<Node*> songs = traceRoute(prev, end);
        return songs;
    }
}

Graph::Node* Graph::getNodeID(string songtitle) {
    cout << songtitle << endl;
    int count = 0;
    for (int i = 0; i < graph.size(); i++) {
        //cout << count << endl;
        if (graph[i]->trackName == songtitle) return graph[i];
    }
    cout << "didn't find it" << endl;
    return nullptr;
}

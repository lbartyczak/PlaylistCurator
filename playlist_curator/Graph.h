#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <iomanip>
#include <utility>
#include<stack>
#include <queue>
#include <set>
using namespace std;

class Graph {

public:
    struct Node {
        string trackName = "";
        string artist = "";
        string genre = "";
        float score = 0.0;
        bool accousticness = 0.0;
        string id = "";
        vector<int> neighbors;
        Node() {};
    };

    //public:
    Graph();
    void addNode(string trackName, string artist, string genre, float score, vector<int> neigh, string id);
    vector<Node*> dijkstra(Node* start, Node* finish);

    vector<Node*> bfs(Node* start, Node* end);
    Node* getNodeID(string id);

private:
    vector<Node*> graph;
    float isEdge(int n, int m);
    int isVisited(int n, set<int>& nodes);
    vector<Node*> traceRoute(map<Node*, Node*>& prev, Node* end);
};
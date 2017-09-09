#ifndef EDGE_H
#define EDGE_H
class Edge
{
public:
	int u;    //
	int v;
	int weight;   //;

public:
	Edge();
	Edge(int u, int v);
	Edge(int u, int v, int w);
	~Edge();
};

Edge::Edge(){

}

Edge::Edge(int u, int v){
	this->u = u;
	this->v = v;
	this->weight = 0;
}

Edge::Edge(int u, int v, int w){
	this->u = u;
	this->v = v;
	this->weight = w;
}

Edge::~Edge(){

}

#endif

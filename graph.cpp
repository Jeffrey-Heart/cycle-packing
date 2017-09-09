#include"graph.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<fstream>
using namespace std;
#define MAX_VECTOR 30

double randf()
{
	return (double)(rand() / (double)RAND_MAX);//产生0到1的浮点小数
}

int randi()
{
	return (int)(rand() % 10) + 1;//产生1 ~ 10 的随机整数
}

void Graph::init()
{
	memset(this->date2, 0, MAX_VECTOR*MAX_VECTOR*sizeof(int));
	memset(this->date3, 0, MAX_VECTOR*MAX_VECTOR*MAX_VECTOR*sizeof(int));
	//memset(this->edges, 0, MAX_VECTOR*MAX_VECTOR*sizeof(bool));
	for (int i = 0; i < MAX_VECTOR; i++)
		for (int j = 0; j < MAX_VECTOR; j++)
			this->edges[i][j] = false;

}

void Graph::random_generate_graph(int vsize, double p){
	void init();

	this->vsize = vsize;
	//this->esize = esize;
	srand((unsigned)time(NULL));

	ofstream outfile;
	outfile.open("graph.txt");
	outfile << "edges:" << endl;

	for (int i = 0; i < vsize; i++)
		for (int j = 0; j < vsize; j++){
			if (i == j)
				continue;
			if (randf() <= p){
				this->addEdge(i, j);
				outfile << "<" << i << "," << j << ">" << endl;
			}
		}

	outfile << "2-cycle weight:" << endl;
	for (int i = 0; i < vsize; i++)
		for (int j = 0; j < vsize; j++){
			if (this->isEdge(i, j) && this->isEdge(j, i)){
				if (this->getWeight2(i, j) <= 0){
					int w = randi();
					this->setWeight2(i, j, w);
					this->setWeight2(j, i, w);
					outfile << "(" << i << "," << j << ") " << w << endl;
				}

			}
		}

	outfile << "3-cycle weight:" << endl;
	for (int i = 0; i < vsize; i++)
		for (int j = 0; j < vsize; j++)
			for (int k = 0; k < vsize; k++){
				if (this->isEdge(i, j) && this->isEdge(j, k) && this->isEdge(k, i)){
					if (this->getWeight3(i, j, k) <= 0){
						int w = randi();
						this->setWeight3(i, j, k, w);
						this->setWeight3(i, k, j, w);
						this->setWeight3(j, i, k, w);
						this->setWeight3(j, k, i, w);
						this->setWeight3(k, i, j, w);
						this->setWeight3(k, j, i, w);
						outfile << "(" << i << "," << j << "," << k << ") " << w << endl;
					}

				}
			}
	outfile.close();

	return;
}

void Graph::addEdge(int u, int v){

	if (u<0 || v<0){
		printf("Error! u and v must >=0\n");
		exit(0);
		return;
	}

	if (u>vsize || v > vsize){
		printf("Error! u and v must < vsize\n");
		exit(0);
		return;
	}

	this->edges[u][v] = true;

	return;

}

void Graph::delEdge(int u, int v){

	if (u<0 || v<0){
		printf("Error! u and v must >=0\n");
		exit(0);
		return;
	}

	if (u>vsize || v > vsize){
		printf("Error! u and v must < vsize\n");
		exit(0);
		return;
	}

	this->edges[u][v] = false;
	return;

}

bool Graph::isEdge(int u, int v)
{
	if (u<0 || v<0){
		printf("Error! u and v must >=0\n");
		exit(0);
		return false;
	}

	if (u>vsize || v > vsize){
		printf("Error! u and v must < vsize\n");
		exit(0);
		return false;
	}

	if (this->edges[u][v] == true)
		return true;

	return false;
}

int Graph::getWeight2(int u, int v)
{
	if (u<0 || v<0){
		printf("Error! u and v must >=0\n");
		exit(0);
		return -1;
	}

	if (u>vsize || v > vsize){
		printf("Error! u and v must < vsize\n");
		exit(0);
		return -1;
	}

	return this->date2[u][v];

}

void Graph::setWeight2(int u, int v, int weight)
{
	if (u<0 || v<0){
		printf("Error! u and v must >=0\n");
		exit(0);
		return;
	}

	if (u>vsize || v > vsize){
		printf("Error! u and v must < vsize\n");
		exit(0);
		return;
	}

	this->date2[u][v] = weight;
	return;

}

int Graph::getWeight3(int u, int v, int w)
{
	if (u<0 || v<0 || w<0){
		printf("Error! u, v and w must >=0\n");
		exit(0);
		return -1;
	}

	if (u>vsize || v > vsize || w > vsize){
		printf("Error! u, v and w must < vsize\n");
		exit(0);
		return -1;
	}

	return this->date3[u][v][w];

}

void Graph::setWeight3(int u, int v, int w, int weight)
{
	if (u<0 || v<0 || w<0){
		printf("Error! u, v and w must >=0\n");
		exit(0);
		return;
	}

	if (u>vsize || v > vsize || w > vsize){
		printf("Error! u, v and w must < vsize\n");
		exit(0);
		return;
	}

	this->date3[u][v][w] = weight;

}

void Graph::setVsize(int vsize){
	if (vsize<0){
		printf("Error! vsize must >= 0\n");
		exit(0);
		return;
	}

	this->vsize = vsize;
	return;
}
int Graph::getVsize(){

	return this->vsize;

}
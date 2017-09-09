#include<iostream>
#include<vector>
#include<fstream>
#include<string.h>
#include"edge.h"
#include"graph.h"
#include"match.h"

using namespace std;

#define MAX_SIZE 100

int maxweight = -1;

int min_size_S =MAX_SIZE;

vector<Edge>min_S;

bool getVecY(vector<Edge> edgeY, bool vecY[], int m){

	for (int i = 0; i<m; i++){

		vecY[i] = false;

	}

	for (int i = 0; i<edgeY.size(); i++){

		if (vecY[edgeY[i].u] == true || vecY[edgeY[i].v] == true)   //is not  independent arcs
			return false;

		vecY[edgeY[i].u] = true;

		vecY[edgeY[i].v] = true;
	}

	return true;
}

void generGraphY(Graph *D, Graph &graphY, vector<Edge> edgeY, bool vecY[], int m){

	graphY.setVsize(D->getVsize() + edgeY.size());

	for (int i = 0; i<D->getVsize(); i++)
		for (int j = 0; j<D->getVsize(); j++){

			if (vecY[i]==true || vecY[j]==true)  // i,j is in V\vecY
				continue;

			if (D->isEdge(i, j) && D->isEdge(j, i)){
				graphY.addEdge(i, j);

				graphY.setWeight2(i, j, D->getWeight2(i, j));

				graphY.addEdge(j, i);

				graphY.setWeight2(j, i, D->getWeight2(i, j));
			}
		}

	for (int k = 0; k<D->getVsize(); k++){

		if (vecY[k] == false){   //  V(G)\V(D)

			for (int i = 0; i<edgeY.size(); i++){

				if (D->isEdge(k, edgeY[i].u) && D->isEdge(edgeY[i].v, k)){

					graphY.addEdge(k, i + D->getVsize());

					graphY.setWeight2(k, i + D->getVsize(),
						D->getWeight3(k, edgeY[i].u, edgeY[i].v));

					graphY.addEdge(i + D->getVsize(), k);

					graphY.setWeight2(i + D->getVsize(), k,
						D->getWeight3(k, edgeY[i].u, edgeY[i].v));
				}

			}
		}
	}
}

int cycle_packing(Graph *D, vector<Edge>S){

	//if (S.size() <= 0)

	//	return 0;

	bool vecY[MAX_SIZE];

	if (getVecY(S, vecY, D->getVsize()) == false)  // is not independent arcs
		return 0;

	printf("vector Y:\n");

	for (int i = 0; i < D->getVsize(); i++)
		if (vecY[i]>0)
			printf("%d ", i);

	printf("\n");

	Graph graphY = Graph();

	generGraphY(D, graphY, S, vecY, D->getVsize()); //graphY is undirected graph

	vector<Edge>edgesY;

	printf("Edge:\n");
	for (int i = 0; i < graphY.getVsize(); i++)
		for (int j = i + 1; j < graphY.getVsize(); j++)
			if (graphY.isEdge(i, j)){

				printf("<%d,%d>\n", i, j);
				edgesY.push_back(Edge(i, j, graphY.getWeight2(i, j)));

			}


	// Remove isolated points
	int offset[MAX_SIZE];

	memset(offset, -1, MAX_SIZE*sizeof(int));

	for (int i = 0; i < edgesY.size(); i++){

		offset[edgesY[i].u] = 1;

		offset[edgesY[i].v] = 1;

	}

	int count = 0;

	for (int i = 0; i < MAX_SIZE; i++){

		if (offset[i] == 1)
			count++;
	}

	int flag[MAX_SIZE];

	memset(flag, -1, MAX_SIZE * sizeof(int));

	for (int i = 0,j=1; i < MAX_SIZE; i++){
		if (offset[i] == 1){
			flag[j] = i;
			j++;
		}
	}

	memset(offset, -1, MAX_SIZE * sizeof(int));

	for (int i = 1; i <= count; i++){

		if (flag[i] >= 0)
			offset[flag[i]] = i;

	}

	printf("check:\n");

	for (int i = 1; i <= count; i++){

		printf("%d %d %d\n", i, flag[i], offset[flag[i]]);

	}

	printf("count:%d\n", count);

	printf("New edges2:\n");

	for (int i = 0; i < edgesY.size(); i++){

		edgesY[i].u = offset[edgesY[i].u];

		edgesY[i].v = offset[edgesY[i].v];

		printf("(%d %d %d)\n", edgesY[i].u, edgesY[i].v, edgesY[i].weight);

	}

	if (edgesY.size() <= 0)  // G[Y] has no edge
		return 0;

	int result[MAX_SIZE];

	int tot_weight = mwmatching(count, edgesY, result);

	if (tot_weight <= maxweight)
		return tot_weight;

	for (int i = 0; i < edgesY.size(); i++){
		edgesY[i].u = flag[edgesY[i].u];
		edgesY[i].v = flag[edgesY[i].v];
	}

	int result2[MAX_SIZE];

	memset(result2, -1, MAX_SIZE * sizeof(int));

	for (int i = 1; i <= count; i++){

		if (result[i]>0)
			result2[flag[i]] = flag[result[i]];

	}

	for (int i = 0; i < graphY.getVsize(); i++){

		printf("max weight matech %d %d\n", i, result2[i]);

	}

	int flag1[MAX_SIZE];

	memset(flag1, -1, MAX_SIZE*sizeof(int));

	if (tot_weight > maxweight){

		printf("cycle packing!\n");

		maxweight = tot_weight;

		ofstream outfile;
		outfile.open("output.txt");
		
		for (int i = 0; i < D->getVsize(); i++){

			if (result2[i] >= D->getVsize()){

				int nnn = result2[i] - D->getVsize();
				//printf("%d %d %d\n", i, S[nnn].u, S[nnn].v);
				outfile << "(" << i << "," << S[nnn].u << "," << S[nnn].v << ")" << endl;

			}
			else if (result2[i] >= 0){

				if (flag1[i] != 1){
					//printf("%d %d\n", i, result2[i]);
					outfile << "(" << i << "," << result2[i] << ")" << endl;

					flag1[i] = 1;

					flag1[result2[i]] = 1;

				}
			}
		}
		outfile.close();
	}
	
	/*
	printf("Edge:\n");
	for (int i = 0; i < graphY.getVsize(); i++)
		for (int j = i + 1; j < graphY.getVsize(); j++)
			if (graphY.isEdge(i, j)){
				printf("<%d,%d>\n", i, j);
				edgesY.push_back(Edge(i+1, j+1, graphY.getWeight2(i, j)));
			}
	int result[MAX_SIZE];
	memset(result, -1, MAX_SIZE * sizeof(int));

	int tot_weight = mwmatching(graphY.getVsize(), edgesY, result);

	int flag1[1000];
	memset(flag1, -1, MAX_SIZE*sizeof(int));

	if (tot_weight > maxweight){

		maxweight = tot_weight;

		ofstream outfile;

		outfile.open("output.txt");

		printf("cycle packing!\n");

		for (int i = 0; i < D->getVsize(); i++){

			if (result[i+1]-1 >= D->getVsize()){

				int nnn = result[i+1] -1- D->getVsize();

				//printf("%d %d %d\n", i, S[nnn].u, S[nnn].v);

				outfile << "(" << i << "," << S[nnn].u << "," << S[nnn].v << ")" << endl;

			}
			else if (result[i+1]-1 >= 0){
				if (flag1[i] != 1){
					//printf("%d %d\n", i, result2[i]);
					outfile << "(" << i << "," << result[i+1]-1 << ")" << endl;
					flag1[i] = 1;
					flag1[result[i+1]-1] = 1;
				}
			}
		}
		
		outfile.close();
	}
	*/
	return tot_weight;
}

void brute_force_search(Graph *D, vector<Edge> S){
	bool flag = false; // find 3-cycle
	int nver = D->getVsize();
	for (int i = 0; i<nver&&!flag; i++){
		for (int j = 0; j <nver&& !flag; j++)
			for (int k = 0; k < nver&& !flag; k++){
				if (D->isEdge(i, j) && D->isEdge(j, k) && D->isEdge(k, i)){
					flag = true;
					//printf("i,j,k :%d %d %d\n",i,j,k);
					S.push_back(Edge(i, j));
					D->delEdge(i, j);
					brute_force_search(D, S);
					S.pop_back();
					D->addEdge(i, j);
					/**/
					D->delEdge(j, k);
					S.push_back(Edge(j, k));
					brute_force_search(D, S);
					S.pop_back();
					D->addEdge(j, k);

					D->delEdge(k, i);
					S.push_back(Edge(k, i));
					brute_force_search(D, S);
					S.pop_back();
					D->addEdge(k, i);

					return;
				}
			}
	}
	if (S.size() > 0 && S.size()<min_size_S && flag == false){
		min_size_S = S.size();
		vector<Edge> temp;
		min_S.swap(temp);

		printf("S:\n");
		for (int i = 0; i < S.size(); i++){
			printf("(%d,%d)\n", S[i].u, S[i].v);
			min_S.push_back(Edge(S[i].u, S[i].v));
		}
			

		
	}
	return;
}



void subSet(Graph *D, vector<Edge>S){
	int size = S.size();
	vector<Edge>sub_S;
	int x[MAX_SIZE];

	for (int i = 0; i < (1 << size); i++){
		for (int j = 0; j < size; j++)
		{
			if ((i & (1 << j)) == 0)
				x[j] = 0;
			else
				x[j] = 1;
		}
		for (int j = 0; j < size; j++){
			if (x[j] ==1){
				sub_S.push_back(Edge(S[j].u,S[j].v,S[j].weight));
			}
			//cout << x[j];
		}
		cycle_packing(D, sub_S);
		vector<Edge>temp;
		sub_S.swap(temp);
		//cout << x[j];
		//cout << endl;
	}
}

int main(){

	Graph* graph = new Graph();

	int verNum = 10;	// 随机生成图的点数

	double p = 0.3;  //生成每条边的概率

	graph->random_generate_graph(verNum, p);

	vector<Edge>S;

	brute_force_search(graph, S);  //Finding a set S of minimum size satisfying the conditions

	if (min_S.size() == 0){   
		// no 3-cycle    

		cycle_packing(graph, min_S);

	}
	else{

		subSet(graph, min_S);

	}
	

	delete graph;
	system("pause");
	return 0;
}


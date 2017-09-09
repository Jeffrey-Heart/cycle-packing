#ifndef GRAPH_H
#define GRAPH_H
#define MAX_VECTOR 30
#include<stdio.h>
#include <iostream>
#include<stdlib.h>
#include<time.h>
#include<fstream>
using namespace std;

class Graph               //图类
{
public:
    int  vsize;           //记录顶点个数
    int  esize;           //记录边个数
    bool edges[MAX_VECTOR][MAX_VECTOR];
    int date2[MAX_VECTOR][MAX_VECTOR];
    int date3[MAX_VECTOR][MAX_VECTOR][MAX_VECTOR]; 
//public:
 //    Graph();
 //    ~Graph();
public:
	
    void random_generate_graph(int vsize,double p);   //创建图
    void addEdge(int u,int v);  //添加边
    void delEdge(int u,int v);
    bool isEdge(int start,int end);
    int getWeight2(int u,int v);   //查看指定边权值
    int getWeight3(int u,int v,int w);
    void setWeight2(int u,int v, int weight);   
    void setWeight3(int u,int v,int w,int weight);
    void setVsize(int vsize);
    int getVsize();
	void init();

};



 
#endif

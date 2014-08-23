
#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <vector>
using namespace std;

#include "Edge.h"

template <class T>
class Graph;

#define NOT_VISITED 0
#define BEING_VISITED 1
#define DONE_VISITED 2
#define INT_INFINITY INT_MAX

/*
 * ================================================================================================
 * Class Vertex
 * ================================================================================================
 */
template <class T>
class Vertex {
	T info;
	vector<Edge<T>  > adj;
	bool visited;
	bool processing;
	int indegree;
	int dist;
	int set;

	int low;
	int num;

	Vertex<T>* path;
public:

	Vertex(T in): info(in), visited(false), processing(false), indegree(0), dist(0)
	{
		path = NULL;
	}

	void updateEdgeFlow(unsigned int index, float f)
	{
		if (index >= adj.size())
			return;

		adj[index].flow = f;
	}

	void addEdge(Vertex<T> *dest, double w)
	{
		Edge<T> edgeD(dest,w);
		edgeD.orig = this;
		adj.push_back(edgeD);
	}

	void addEdge(Vertex<T> *dest, double w, float f)
	{
		Edge<T> edgeD(dest, w, f);
		edgeD.orig = this;
		adj.push_back(edgeD);
	}

	vector<Edge<T> > getAdj()
	{
		return this->adj;
	}

	bool removeEdgeTo(Vertex<T> *d)
	{
		d->indegree--; //adicionado do exercicio 5
		typename vector<Edge<T> >::iterator it= adj.begin();
		typename vector<Edge<T> >::iterator ite= adj.end();
		while (it!=ite) {
			if (it->dest == d) {
				adj.erase(it);
				return true;
			}
			else it++;
		}
		return false;
	}

	T getInfo() const
	{
		return this->info;
	}

	void setInfo(T info)
	{
		this->info = info;
	}

	int getDist() const
	{
		return this->dist;
	}

	int getIndegree() const
	{
		return this->indegree;
	}

	Vertex* getPath()
	{
		return this->path;
	}

	void setPath(Vertex* path)
	{
		this->path = path;
	}

	bool wasProcessed()
	{
		return this->processing;
	}

	void setProcessed(bool processed)
	{
		this->processing = processed;
	}

	bool operator<(const Vertex& vert) const
	{
	      return this->getDist() < vert.getDist();
	}

	bool operator>(const Vertex& vert) const
	{
		  return this->getDist() > vert.getDist();
	}

	friend class Graph<T>;
};

//Maintained for legacy purposes
template <class T>
struct vertex_greater_than {
    bool operator()(Vertex<T> * a, Vertex<T> * b) const {
        return a->getDist() > b->getDist();
    }
};

#endif

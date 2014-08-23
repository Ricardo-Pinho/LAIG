/*
 * Edge.h
 *
 *  Created on: 2 de Abr de 2011
 *      Author: Tiago
 */

#ifndef EDGE_H_
#define EDGE_H_

template <class T>
class Vertex;

template <class T>
class Graph;

/* ================================================================================================
 * Class Edge
 * ================================================================================================
 */
template <class T>
class Edge {
public:
	Vertex<T> * orig;
	Vertex<T> * dest;
	double weight;
	float flow;

	Edge(Vertex<T> *d, double w, float f=0);
	friend class Graph<T>;
	friend class Vertex<T>;

	bool operator<(const Edge<T> &other) const {
		return this->weight < other.weight;
	}

	bool operator>(const Edge<T> &other) const {
		return this->weight > other.weight;
	}
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w, float f): dest(d), weight(w), flow(f){}


//Maintained for legacy purposes
template <class T>
struct edge_greater_than {
    bool operator()(Edge<T> a, Edge<T>  b) const {
        return a.weight > b.weight;
    }
};

#endif /* EDGE_H_ */

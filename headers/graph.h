// Original code by Gonçalo Leão
// Updated by DA 2024/2025 Team

#ifndef DA_TP_CLASSES_GRAPH
#define DA_TP_CLASSES_GRAPH

#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include "../headers/MutablePriorityQueue.h"

template <class T>
class Edge;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

template <class T>
class Vertex {
public:
    //CHANGED - done
    Vertex(std::string name, int id, std::string code, bool parking);

    // ...
    bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue

    // ADDED - done
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] int getID() const;
    [[nodiscard]] std::string getCode() const;
    [[nodiscard]] bool getParking() const;

    //...
    std::vector<Edge<T> *> getAdj() const;
    bool isVisited() const;
    bool isProcessing() const;
    unsigned int getIndegree() const;
    double getDist() const;
    Edge<T> *getPath() const;
    std::vector<Edge<T> *> getIncoming() const;

    //...
    void setVisited(bool visited);
    void setProcessing(bool processing);

    //...
    void setIndegree(unsigned int indegree);
    void setDist(double dist);
    void setPath(Edge<T> *path);

    //CHANGED - done
    Edge<T> *addEdge(Vertex<T> *d, double distance, std::string label);

    //to be revised
    bool removeEdge(T in);
    void removeOutgoingEdges();

    friend class MutablePriorityQueue<Vertex>;
protected:
    std::vector<Edge<T> *> adj;  // outgoing edges
    std::string name; // place name
    int id; // place id
    std::string code; // place code
    bool hasParking; // place parking

    // auxiliary fields
    bool visited = false; // used by DFS, BFS, Prim ...
    bool processing = false; // used by isDAG (in addition to the visited attribute)
    unsigned int indegree; // used by topsort
    double dist = 0;
    Edge<T> *path = nullptr;

    std::vector<Edge<T> *> incoming; // incoming edges

    int queueIndex = 0; 		// required by MutablePriorityQueue and UFDS

    void deleteEdge(Edge<T> *edge);
};

/********************** Edge  ****************************/

template <class T>
class Edge {
public:

    // ADDED - done
    Edge(Vertex<T> *orig, Vertex<T> *dest, double distance, std::string label);

    Vertex<T> * getDest() const;
    double getWeight() const;
    bool isSelected() const;
    Vertex<T> * getOrig() const;
    Edge<T> *getReverse() const;

    void setSelected(bool selected);
    void setReverse(Edge<T> *reverse);
protected:
    Vertex<T> * dest; // destination vertex
    double weight; // edge weight, can also be used for capacity
    std::string label;

    // auxiliary fields
    bool selected = false;

    // used for bidirectional edges
    Vertex<T> *orig;
    Edge<T> *reverse = nullptr;

};

/********************** Graph  ****************************/

template <class T>
class Graph {
public:
    ~Graph();
    /*
    * Auxiliary function to find a vertex with a given the content.
    */

    //ADDED - done
    Vertex<T> *findVertex(const std::string &in) const;

    /*
     *  Adds a vertex with a given content or info (in) to a graph (this).
     *  Returns true if successful, and false if a vertex with that content already exists.
     */

    //ADDED - done
    bool addVertex(const std::string& name, const int& id, const std::string &code, const bool &hasParking);

    bool removeVertex(const T &in);

    /*
     * Adds an edge to a graph (this), given the contents of the source and
     * destination vertices and the edge weight (w).
     * Returns true if successful, and false if the source or destination vertex does not exist.
     */

    // ADDED - done
    bool addBidirectionalEdge(const std::string &source, const std::string &dest, double distance, std::string label);

    int getNumVertex() const;
    std::vector<Vertex<T> *> getVertexSet() const;

protected:
    std::vector<Vertex<T> *> vertexSet;    // vertex set

    /*
     * Finds the index of the vertex with a given content.
     */
    int findVertexIdx(const T &in) const;
};

void deleteMatrix(int **m, int n);
void deleteMatrix(double **m, int n);

// DONE until this point

/************************* Vertex  **************************/


template<class T>
Vertex<T>::Vertex(std::string name, int id,
    std::string code, bool parking)
    : name(name), id(id), code(code), hasParking(parking) {}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */

// ...
/* template <class T>
Edge<T> * Vertex<T>::addEdge(Vertex<T> *d, double w) {
    auto newEdge = new Edge<T>(this, d, w);
    adj.push_back(newEdge);
    d->incoming.push_back(newEdge);
    return newEdge;
}*/

template <class T>
Edge<T> * Vertex<T>::addEdge(Vertex<T> *d, double distance, std::string label) {
    auto newEdge = new Edge<T>(this, d, distance, label);
    adj.push_back(newEdge);
    d->incoming.push_back(newEdge);
    return newEdge;
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */

// ...
/*
template <class T>
bool Vertex<T>::removeEdge(T in) {
    bool removedEdge = false;
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        Vertex<T> *dest = edge->getDest();
        if (dest->getInfo() == in) {
            it = adj.erase(it);
            deleteEdge(edge);
            removedEdge = true; // allows for multiple edges to connect the same pair of vertices (multigraph)
        }
        else {
            it++;
        }
    }
    return removedEdge;
}
*/

/*
 * Auxiliary function to remove an outgoing edge of a vertex.
 */
template <class T>
void Vertex<T>::removeOutgoingEdges() {
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        it = adj.erase(it);
        deleteEdge(edge);
    }
}

template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
    return this->dist < vertex.dist;
}

//REMOVE
/*
template <class T>
T Vertex<T>::getInfo() const {
    return this->info;
}
*/

template<class T>
std::string Vertex<T>::getName() const {
    return this->name;
}

template<class T>
std::string Vertex<T>::getCode() const {
    return this->code;
}

template<class T>
int Vertex<T>::getID() const {
    return this->id;
}

template<class T>
bool Vertex<T>::getParking() const {
    return this->hasParking;
}

// REMOVE
/*
template <class T>
int Vertex<T>::getLow() const {
    return this->low;
}

template <class T>
void Vertex<T>::setLow(int value) {
    this->low = value;
}

template <class T>
int Vertex<T>::getNum() const {
    return this->num;
}

template <class T>
void Vertex<T>::setNum(int value) {
    this->num = value;
}*/

template <class T>
std::vector<Edge<T>*> Vertex<T>::getAdj() const {
    return this->adj;
}

template <class T>
bool Vertex<T>::isVisited() const {
    return this->visited;
}

template <class T>
bool Vertex<T>::isProcessing() const {
    return this->processing;
}

template <class T>
unsigned int Vertex<T>::getIndegree() const {
    return this->indegree;
}

template <class T>
double Vertex<T>::getDist() const {
    return this->dist;
}

template <class T>
Edge<T> *Vertex<T>::getPath() const {
    return this->path;
}

template <class T>
std::vector<Edge<T> *> Vertex<T>::getIncoming() const {
    return this->incoming;
}

//REMOVE
/*
template <class T>
void Vertex<T>::setInfo(T in) {
    this->info = in;
}*/

template <class T>
void Vertex<T>::setVisited(bool visited) {
    this->visited = visited;
}

template <class T>
void Vertex<T>::setProcessing(bool processing) {
    this->processing = processing;
}

template <class T>
void Vertex<T>::setIndegree(unsigned int indegree) {
    this->indegree = indegree;
}

template <class T>
void Vertex<T>::setDist(double dist) {
    this->dist = dist;
}

template <class T>
void Vertex<T>::setPath(Edge<T> *path) {
    this->path = path;
}

//REMOVE
/*
template <class T>
void Vertex<T>::deleteEdge(Edge<T> *edge) {
    Vertex<T> *dest = edge->getDest();
    // Remove the corresponding edge from the incoming list
    auto it = dest->incoming.begin();
    while (it != dest->incoming.end()) {
        if ((*it)->getOrig()->getInfo() == info) {
            it = dest->incoming.erase(it);
        }
        else {
            it++;
        }
    }
    delete edge;
}*/

/********************** Edge  ****************************/

// REMOVE
//template <class T>
//Edge<T>::Edge(Vertex<T> *orig, Vertex<T> *dest, double w): orig(orig), dest(dest), weight(w) {}

template <class T>
Edge<T>::Edge(Vertex<T> *orig, Vertex<T> *dest, double distance, std::string label): orig(orig), dest(dest), weight(distance), label(label) {}

template <class T>
Vertex<T> * Edge<T>::getDest() const {
    return this->dest;
}

template <class T>
double Edge<T>::getWeight() const {
    return this->weight;
}

template <class T>
Vertex<T> * Edge<T>::getOrig() const {
    return this->orig;
}

template <class T>
Edge<T> *Edge<T>::getReverse() const {
    return this->reverse;
}

template <class T>
bool Edge<T>::isSelected() const {
    return this->selected;
}

/*template <class T>
double Edge<T>::getFlow() const {
    return flow;
}*/

template <class T>
void Edge<T>::setSelected(bool selected) {
    this->selected = selected;
}

template <class T>
void Edge<T>::setReverse(Edge<T> *reverse) {
    this->reverse = reverse;
}

/*template <class T>
void Edge<T>::setFlow(double flow) {
    this->flow = flow;
}*/

/********************** Graph  ****************************/

template <class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
}

template <class T>
std::vector<Vertex<T> *> Graph<T>::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
// ALTERED
/*
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
    for (auto v : vertexSet)
        if (v->getInfo() == in)
            return v;
    return nullptr;
}
*/

template <class T>
Vertex<T> * Graph<T>::findVertex(const std::string &in) const {
    for (auto v : vertexSet) {
        if (v->getCode() == in) {
            return v;
        }
    }
    return nullptr;
}

/*
 * Finds the index of the vertex with a given content.
 */
template <class T>
int Graph<T>::findVertexIdx(const T &in) const {
    for (unsigned i = 0; i < vertexSet.size(); i++)
        if (vertexSet[i]->getInfo() == in)
            return i;
    return -1;
}
/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
/*template <class T>
bool Graph<T>::addVertex(const T &in) {
    if (findVertex(in) != nullptr)
        return false;
    vertexSet.push_back(new Vertex<T>(in));
    return true;
}*/

template<class T>
bool Graph<T>::addVertex(const std::string &name, const int &id, const std::string &code, const bool &hasParking) {
    Vertex<T> *vertex = new Vertex<T>(name, id, code, hasParking);
    vertexSet.push_back(vertex);
    return true;
}


/*
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Returns true if successful, and false if such vertex does not exist.
 */

template <class T>
bool Graph<T>::removeVertex(const T &in) {
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if ((*it)->getInfo() == in) {
            auto v = *it;
            v->removeOutgoingEdges();
            for (auto u : vertexSet) {
                u->removeEdge(v->getInfo());
            }
            vertexSet.erase(it);
            delete v;
            return true;
        }
    }
    return false;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
/*template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, w);
    return true;
}*/


/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
/*template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
    Vertex<T> * srcVertex = findVertex(sourc);
    if (srcVertex == nullptr) {
        return false;
    }
    return srcVertex->removeEdge(dest);
}

template <class T>
bool Graph<T>::addBidirectionalEdge(const T &sourc, const T &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2, w);
    auto e2 = v2->addEdge(v1, w);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}*/

template <class T>
bool Graph<T>::addBidirectionalEdge(const std::string &source, const std::string &dest, double distance, std::string label) {
    auto v1 = findVertex(source);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr) return false;
    auto e1 = v1->addEdge(v2, distance, label);
    auto e2 = v2->addEdge(v1, distance, label);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}


inline void deleteMatrix(int **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete [] m[i];
        delete [] m;
    }
}

inline void deleteMatrix(double **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete [] m[i];
        delete [] m;
    }
}

template <class T>
Graph<T>::~Graph() {
    deleteMatrix(distMatrix, vertexSet.size());
    deleteMatrix(pathMatrix, vertexSet.size());
}

#endif /* DA_TP_CLASSES_GRAPH */
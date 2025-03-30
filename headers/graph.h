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
    /**
     * Constructs a Vertex object.
     * @param name The name of the location.
     * @param id The unique identifier of the location.
     * @param code The code of the location.
     * @param parking Bool that indicates if the location has parking or not.
     */
    Vertex(std::string name, int id, std::string code, bool parking);
    /**
     * Compares two vertices based on their distance.
     * @param vertex The vertex to compare with.
     * @return `true` if this vertex has a smaller distance, `false` otherwise.
     */
    bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue
    /**
    * Gets the name of the vertex.
    * @return The name of the vertex.
    */
    [[nodiscard]] std::string getName() const;
    /**
     * Gets the ID of the vertex.
     * @return The ID of the vertex.
     */
    [[nodiscard]] int getID() const;
    /**
     * Gets the code of the vertex.
     * @return The code of the vertex.
     */
    [[nodiscard]] std::string getCode() const;
    /**
     * Checks if the vertex has parking or not.
     * @return `true` if it has parking, `false` otherwise.
     */
    [[nodiscard]] bool getParking() const;
    /**
    * Finds an edge to a given destination vertex.
    * @param dest The ID of the destination vertex.
    * @return A pointer to the edge if found, `nullptr` otherwise.
    */
    Edge<T> * findEdge(int dest, const std::string &mode);
    /**
     * Gets the outgoing edges of the vertex.
     * @return A vector of pointers to the outgoing edges.
     */
    std::vector<Edge<T> *> getAdj() const;
    /**
     * Checks if the vertex has been visited.
     * @return `true` if visited, `false` otherwise.
     */
    bool isVisited() const;
    /**
     * Checks if the vertex is being processed.
     * @return `true` if processing, `false` otherwise.
     */
    bool isProcessing() const;
    /**
     * Gets the indegree of the vertex.
     * @return The indegree of the vertex.
     */
    unsigned int getIndegree() const;
    /**
     * Gets the distance of the vertex.
     * @return The distance of the vertex.
     */
    double getDist() const;
    /**
     * Gets the path edge leading to the vertex.
     * @return A pointer to the path edge.
     */
    Edge<T> *getPath() const;
    /**
     * Gets the incoming edges of the vertex.
     * @return A vector of pointers to the incoming edges.
     */
    std::vector<Edge<T> *> getIncoming() const;
    /**
     * Sets the visited status of the vertex.
     * @param visited The new visited status.
     */
    void setVisited(bool visited);
    /**
     * Sets the processing status of the vertex.
     * @param processing The new processing status.
     */
    void setProcessing(bool processing);
    /**
     * Sets the indegree of the vertex.
     * @param indegree The new indegree value.
     */
    void setIndegree(unsigned int indegree);
    /**
     * Sets the distance of the vertex.
     * @param dist The new distance value.
     */
    void setDist(double dist);
    /**
     * Sets the path edge leading to this vertex.
     * @param path A pointer to the new path edge.
     */
    void setPath(Edge<T> *path);

    /**
     * Adds an outgoing edge to the vertex.
     * @param d The destination vertex.
     * @param distance The weight of the edge.
     * @param label The label of the edge.
     * @return A pointer to the newly created edge.
     */
    Edge<T> *addEdge(Vertex<T> *d, double distance, std::string label);
    /**
     * Removes an outgoing edge to a given destination vertex.
     * @param in The ID of the destination vertex.
     * @return `true` if the edge was removed, `false` otherwise.
     */
    bool removeEdge(T in);
    /**
    * Removes all outgoing edges of the vertex.
    */
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

    /**
     * Deletes an edge and removes it from the incoming list of the destination vertex.
     * @param edge The edge to delete.
     */
    void deleteEdge(Edge<T> *edge);
};

/********************** Edge  ****************************/

template <class T>
class Edge {
public:
    /**
     * Constructs an Edge object.
     * @param orig The origin vertex.
     * @param dest The destination vertex.
     * @param distance The weight of the edge.
     * @param label The label of the edge.
     */
    Edge(Vertex<T> *orig, Vertex<T> *dest, double distance, std::string label);
    /**
     * Gets the destination vertex of the edge.
     * @return A pointer to the destination vertex.
     */
    Vertex<T> * getDest() const;
    /**
     * Gets the weight of the edge.
     * @return The weight of the edge.
     */
    double getWeight() const;
    /**
     * Checks if the edge is selected.
     * @return `true` if selected, `false` otherwise.
     */
    bool isSelected() const;
    /**
     * Checks if the edge should be avoided.
     * @return `true` if the edge should be avoided, `false` otherwise.
     */
    bool shouldAvoid() const;
    /**
     * Gets the origin vertex of the edge.
     * @return A pointer to the origin vertex.
     */
    Vertex<T> * getOrig() const;
    /**
     * Gets the reverse edge.
     * @return A pointer to the reverse edge.
     */
    Edge<T> *getReverse() const;
    /**
      * Gets the label of the edge.
      * @return The label of the edge.
      */
    std::string getLabel() const;
    /**
     * Sets whether the edge should be avoided.
     * @param avoid The new avoid status.
     */
    void setAvoid(bool avoid);
    /**
     * Sets whether the edge is selected.
     * @param selected The new selected status.
     */
    void setSelected(bool selected);
    /**
     * Sets the reverse edge.
     * @param reverse A pointer to the reverse edge.
     */
    void setReverse(Edge<T> *reverse);
protected:
    Vertex<T> * dest; // destination vertex
    double weight; // edge weight, can also be used for capacity
    std::string label;

    // auxiliary fields
    bool selected = false;
    bool avoid = false;

    // used for bidirectional edges
    Vertex<T> *orig;
    Edge<T> *reverse = nullptr;

};

/********************** Graph  ****************************/

template <class T>
class Graph {
public:
    /**
     * Destructor for the Graph class.
     */
    ~Graph();
    /**
     * Finds a vertex with a given content.
     * @param in The content of the vertex to find.
     * @return A pointer to the vertex if found, `nullptr` otherwise.
     */
    Vertex<T> *findVertex(const std::string &in) const;
    Vertex<T> *findVertex(const int &in) const;
    /**
     * Adds a vertex with a given content to the graph.
     * @param name The name of the location.
     * @param id The unique identifier of the location.
     * @param code The code of the location.
     * @param hasParking Bool that indicates if the location has parking.
     * @return `true` if the vertex was added successfully, `false` otherwise.
     */
    bool addVertex(const std::string& name, const int& id, const std::string &code, const bool &hasParking);
    /**
     * Removes a vertex with a given content from the graph.
     * @param in The content of the vertex to find.
     * @return `true` if the vertex was removed successfully, `false` otherwise.
     */
    bool removeVertex(const T &in);

    /**
     * Adds a bidirectional edge to the graph.
     * @param source The content of the source vertex.
     * @param dest The content of the destination vertex.
     * @param distance The weight of the edge.
     * @param label The label of the edge.
     * @return `true` if the edge was added successfully, `false` otherwise.
     */
    bool addBidirectionalEdge(const std::string &source, const std::string &dest, double distance, std::string label);
    /**
     * Gets the number of vertices in the graph.
     * @return The number of vertices.
     */
    int getNumVertex() const;
    /**
     * Gets the set of vertices in the graph.
     * @return A vector of pointers to the vertices.
     */
    std::vector<Vertex<T> *> getVertexSet() const;

protected:
    std::vector<Vertex<T> *> vertexSet;    // vertex set

    double ** distMatrix = nullptr;   // dist matrix for Floyd-Warshall
    int **pathMatrix = nullptr;   // path matrix for Floyd-Warshall

    /**
     * Finds the index of the vertex with a given content.
     * @param in The content of the vertex to find.
     * @return The index of the vertex if found, `-1` otherwise.
     */
    int findVertexIdx(const T &in) const;
};

/**
 * Deletes a matrix.
 * @param m The matrix to delete.
 * @param n The number of rows in the matrix.
 */
void deleteMatrix(int **m, int n);
void deleteMatrix(double **m, int n);


/************************* Vertex  **************************/


template<class T>
Vertex<T>::Vertex(std::string name, int id,
    std::string code, bool parking)
    : name(name), id(id), code(code), hasParking(parking) {}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
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
template <class T>
bool Vertex<T>::removeEdge(T in) {
    bool removedEdge = false;
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        Vertex<T> *dest = edge->getDest();
        if (dest->getID() == in) {
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

template <class T>
Edge<T> * Vertex<T>::findEdge(const int dest, const std::string &mode) {
    for (auto e : this->adj) {
        if (e->getDest()->getID() == dest && e->getLabel()== mode) {
            return e;
        }
    }
    return nullptr;
}

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

template <class T>
void Vertex<T>::deleteEdge(Edge<T> *edge) {
    Vertex<T> *dest = edge->getDest();
    // Remove the corresponding edge from the incoming list
    auto it = dest->incoming.begin();
    while (it != dest->incoming.end()) {
        if ((*it)->getOrig()->getID() == id) {
            it = dest->incoming.erase(it);
        }
        else {
            it++;
        }
    }
    delete edge;
}

/********************** Edge  ****************************/

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
std::string Edge<T>::getLabel() const {
    return this->label;
}

template <class T>
bool Edge<T>::isSelected() const {
    return this->selected;
}

template<class T>
bool Edge<T>::shouldAvoid() const {
    return this->avoid;
}


template <class T>
void Edge<T>::setSelected(bool selected) {
    this->selected = selected;
}

template <class T>
void Edge<T>::setReverse(Edge<T> *reverse) {
    this->reverse = reverse;
}

template<class T>
void Edge<T>::setAvoid(bool avoid) {
    this->avoid = avoid;
}


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
template <class T>
Vertex<T> * Graph<T>::findVertex(const std::string &in) const {
    for (auto v : vertexSet) {
        if (v->getCode() == in) {
            return v;
        }
    }
    return nullptr;
}

template <class T>
Vertex<T> * Graph<T>::findVertex(const int &in) const {
    for (auto v : vertexSet) {
        if (v->getID() == in) {
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
        if (vertexSet[i]->getID() == in)
            return i;
    return -1;
}
/*
 *  Adds a vertex with a given content to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
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
        if ((*it)->getID() == in) {
            auto v = *it;
            v->removeOutgoingEdges();
            for (auto u : vertexSet) {
                u->removeEdge(v->getID());
            }
            vertexSet.erase(it);
            delete v;
            return true;
        }
    }
    return false;
}

/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
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
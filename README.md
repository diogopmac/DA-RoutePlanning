# DA PROJECT

# Graph.h
## Functions/attributes removed

### - Class Vertex
1. ``T getInfo() const`` & ``void setInfo(T info)`` - as explained in the **_Functions Changed - Class Vertex - 1._**, we are not using the template Vertex declaration, and so these getters and setters are not necessary
2. ``T info`` - this attribute is not needed, for the same reasons as above
3. ``int getLow() const`` & ``void setLow(int value)`` & ``int getNum() const`` & ``void setNum(int value)`` - since Tarjan's algorithm is not useful in this project, all of these functions are not needed
4. ``int low = -1`` & ``int num = -1`` - like explained above, Tarjan's algorithm is not being used so these parameters can be eliminated

### - Class Edge
1. ``double getFlow() const`` & ``void setFlow(double flow)`` - there is no need for us to have a getter and a setter for flow as we don't need to deal with flow-related issues, being unnecessary having those declared in the class
2. ``double flow`` - as mentioned above this attribute is not needed

### - Class Graph
1. ``bool addEdge(const T &sourc, const T &dest, double w)`` - since we are only interested in adding bidirectional edges as paths exist in both ways, this function is not needed
2. ``bool removeEdge(const T &source, const T &dest)`` - for the same reason as above, this function is not necessary for our project
3. ``double ** distMatrix = nullptr`` &rarr; ``int **pathMatrix = nullptr`` - these attributes were only needed if we were implementing the Floyd-Warshall's algorithm, so they can be eliminated

## Functions changed

### - Class Vertex
1. ``Vertex (T in)`` &rarr; ``Vertex(std::string name, int id, std::string code, bool parking)`` - when defining the vertex, we came to the conclusion it would be beneficial to add **_new parameters_** to make the process of identifying the different locations easier, which means there is no need to use template declarations
2. ``Edge<T> * addEdge(Vertex<T> *dest, double w)`` &rarr; ``Edge<T> *addEdge(Vertex<T> *d, double distance, std::string label);`` - we changed this function by adding a parameter called **_label_** to allow us to identify if a path is either _walkable_ or _drivable_. This allows us to filter de edges in terms of type throughout the project

### - Class Edge
1. ``Edge(Vertex<T> *orig, Vertex<T> *dest, double w)`` &rarr; ``Edge(Vertex<T> *orig, Vertex<T> *dest, double distance, std::string label)`` - like previously described in the **_Class Vertex_**, it is beneficial for us to have a **_label_** parameter to allow us to distinguish the edges(paths) that are _walkable_ from those that are _drivable_, so those need to be added to the Edge declaration.

### - Class Graph
1. ``Vertex<T> *findVertex(const T &in) const`` &rarr; ``Vertex<T> *findVertex(const std::string &in) const`` - previously, we said that we are not using template declarations so we need to change this function in order for it to work when the parameter passed is the vertex's code(string)
2. ``bool addVertex(const T &in)`` &rarr; ``bool addVertex(const std::string& name, const int& id, const std::string &code, const bool &hasParking)`` - since new parameters were added to the Edge declaration, these changes into the _addVertex_ function were mandatory
3. ``bool addBidirectionalEdge(const T &sourc, const T &dest, double w)`` &rarr; ``bool addBidirectionalEdge(const std::string &source, const std::string &dest, double distance, std::string label)`` - when adding a bidirectional edge we need to keep track of the edges type therefore, we needed to change this function and add that attribute, in order to correctly define it

## Functions/attributes added

### - Class Vertex
1. ``std::string getName() const`` & ``int getID() const`` & ``std::string getCode() const`` & ``bool getParking() const`` - due to the attributes added into the function **_Vertex()_** we needed to implement getter for all of those


Vertex<T>::Vertex(T in): info(in) {}


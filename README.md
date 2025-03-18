# Proj

# Graph.h
## Functions/attributes removed

### - Class Vertex
1. ``T getInfo() const`` & ``void setInfo(T info)`` - as explained in the **_Functions Changed - Class Vertex - 1._**, we are not using the template Vertex declaration, and so these getters and setters are not necessary
2. ``T info`` - this attribute is not needed, for the same reasons as above
3. ``int getLow() const`` & ``void setLow(int value)`` & ``int getNum() const`` & ``void setNum(int value)`` - since Tarjan's algorithm is not useful in this project, all of these functions are not needed
4. ``int low = -1`` & ``int num = -1`` - like explained above, Tarjan's algorithm is not being used so these parameters can be eliminated

## Functions changed

### - Class Vertex
1. ``Vertex (T in)`` &rarr; ``Vertex(std::string name, int id, std::string code, bool parking)`` - when defining the vertex, we came to the conclusion it would be beneficial to add **_new parameters_** to make the process of identifying the different locations easier
2. ``Edge<T> * addEdge(Vertex<T> *dest, double w)`` &rarr; ``Edge<T> *addEdge(Vertex<T> *d, double distance, std::string label);`` - we changed this function by adding a parameter called **_label_** to allow us to identify if a path is either walkable or drivable. This allows us to filter de edges in terms of type throughout the project

## Functions/attributes added


Vertex<T>::Vertex(T in): info(in) {}


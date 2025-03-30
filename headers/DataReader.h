#ifndef DATAREADER_H
#define DATAREADER_H
#include <string>
#include "graph.h"

class DataReader {
    public:
    /**
     * Constructor for the DataReader class.
     */
    DataReader();
    /**
     * Destructor for the DataReader class.
     */
    ~DataReader();
    /**
     * Reads location data from a file and populates the graph.
     * @param fileName Path to the file containing location data.
     * @param graph Reference to the graph to populate.
     */
    void readLocations(const std::string& fileName, Graph<int>& graph);
    /**
     * Reads distance data from a file and populates the graph with edges.
     * @param fileName Path to the file containing distance data.
     * @param graph Reference to the graph to populate.
     */
    void readDistances(const std::string& fileName, Graph<int>& graph);
    /**
     * Reads input data from a file for batch processing.
     * @param inFile Path to the input file.
     * @param mode Reference to store the transportation mode.
     * @param source Reference to store the source vertex ID.
     * @param destination Reference to store the destination vertex ID.
     * @param avoidNodes Reference to store the list of nodes to avoid.
     * @param avoidSegments Reference to store the list of edges to avoid.
     * @param includeNode Reference to store the include node ID.
     * @param maxWalking Reference to store the maximum walking time allowed.
     */
    void readInputFile(const std::string& inFile, std::string& mode,
        int &source, int &destination, std::vector<int>& avoidNodes,
        std::vector<std::pair<int, int>>& avoidSegments, int& includeNode, int& maxWalking);
};

#endif //DATAREADER_H

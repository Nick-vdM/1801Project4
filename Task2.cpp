//
// Created by nick on 25/9/19.
//
#include <iostream>
#include <array>
#include <limits>
#include <stack>


/*
 * Given a source and target vertex, find the best path
 *
 * Output:
 *      - Number of hops for the shortest path
 *      - Total length of shortest path
 *      - Number of nodeCount added to the structure
 *      - Number of nodeCount popped from the stack
 *      - Time taken to find the shortest path
 *
 * - Iterative search: stack
 * - Breadth first search: queue
 * - Uniform cost search: priority queue
 *
 * 1 mark: implementing required data structures
 * 2 marks: implementing one search algorithm
 * 1 mark: statistics collected
 * 1 mark: other two algorithms
 */

template<typename T>
class search {
public:
    void createGraph(size_t graphSize) {

    }

    void doSearch() {
        clock_t startClock = clock();

        std::array<size_t, nodeCount> parent;
        std::array<size_t, nodeCount> cost;
        cost.fill(std::numeric_limits<size_t>::max());

        heap.push(source);
        size_t current; // Don't replace the space over and over
        while(!heap.empty()){
            current = heap.pop();
            nodesPopped++;

            if(current != target){
                for(int neigh = 0; neigh < nodeCount; neigh++){
                    if(graph[current][neigh] == 0) continue; // Skip no links
                    if (cost[neigh] > cost[current] + graph[current][neigh]){
                        cost[neigh] = cost[current] + graph[current][neigh];
                        parent[neigh] = current;
                        heap.push(neigh);
                        nodesPushed++;
                    }
                }
            }
        }
        timeTaken = (clock() - startClock) / CLOCKS_PER_SEC;
    }

    void printData() {
        std::cout << "Shortest path: " << shortestPath << std::endl
                  << "Number of hops on shortest path: " << hopsOnShortestPath << std::end
                  << "Shortest path length: " << shortestPathLength << std::endl
                  << "Nodes added: " << nodesPushed << std::endl
                  << "Nodes popped: " << nodesPopped << std::endl;
    };

private:
    size_t source;
    size_t target;

    std::stack<size_t> heap;
    static size_t nodeCount;
    std::array<std::array<size_t, nodeCount>, nodeCount> graph;

    std::string shortestPath;
    size_t hopsOnShortestPath = 0;
    size_t shortestPathLength = std::numeric_limits<size_t>::max();
    size_t nodesPushed = 0;
    size_t nodesPopped = 0;
    double timeTaken;

};

int main() {


}
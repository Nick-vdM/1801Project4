//
// Created by nick on 25/9/19.
//
#include <iostream>
#include <array>
#include <limits>
#include <stack>
#include <fstream>
#include <vector>
#include <queue>


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
 * - Iterative Search: stack
 * - Breadth first Search: queue
 * - Uniform cost Search: priority queue
 *
 * 1 mark: implementing required data structures
 * 2 marks: implementing one Search xFirstSearchAlgorithm
 * 1 mark: statistics collected
 * 1 mark: other two algorithms
 */

template<typename T>
class Search {
public:
    Search(int source, int target, std::string fileLocation)
            : source(source), target(target) {
        std::ifstream ifs(fileLocation);
        ifs >> nodeCount;
        for (int i = 0; i < nodeCount; i++) {
            matrix.emplace_back();
            matrix[i].reserve(nodeCount);
            for (int j = 0; j < nodeCount; j++) {
                matrix[i].push_back(0);
            }
        }

        for (int i = 0; i < nodeCount; i++) {
            for (int j = 0; j < nodeCount; j++) {
                ifs >> matrix[i][j];
            }
        }
    }

    void doSearch() {
        clock_t startClock = clock();
        std::vector<int> parent(nodeCount, -1);
        std::vector<int> cost(nodeCount, std::numeric_limits<int>::max());
        cost[source] = 0;
        heap.push(source);

        xFirstSearchAlgorithm(parent, cost);

        shortestPathLength = cost[target];
        setShortestPath(parent);
        timeTaken = double((clock() - startClock)) / CLOCKS_PER_SEC;
    }


    void setShortestPath(std::vector<int> &parent) {
        // The parent is stored such from the destination the index points
        // towards the previous node
        int current = target;
        while (current != source) {
            shortestPath.insert(shortestPath.begin(), current);
            current = parent[current];
            hopsOnShortestPath++;
        }
        shortestPath.insert(shortestPath.begin(), current);
    }

    void printData() {
        std::cout << "Shortest path: ";
        printPath();
        std::cout << std::endl
                  << "Number of hops on shortest path: " << hopsOnShortestPath << std::endl
                  << "Shortest path length: " << shortestPathLength << std::endl
                  << "Nodes added: " << nodesPushed << std::endl
                  << "Nodes popped: " << nodesPopped << std::endl
                  << "Time taken: " << timeTaken << std::endl;
    }

private:
    void printPath() {
        for (auto i : shortestPath) {
            std::cout << i << " ";
        }
    }

    //Due to someone defining these functions
    int popNodeFromHeap(std::queue<int> &queueMode) {
        return queueMode.front();
    }

    int popNodeFromHeap(std::stack<int> &stackMode) {
        return stackMode.top();
    }

    int popNodeFromHeap(std::priority_queue<int> &priorityQueueMode) {
        return priorityQueueMode.top();
    }

    void xFirstSearchAlgorithm(std::vector<int> &parent, std::vector<int> &cost) {
        int current; // Don't replace the space over and over
        while (!heap.empty()) {
            current = popNodeFromHeap(heap);
            heap.pop();
            nodesPopped++;

            if (current != target) {
                for (int neigh = 0; neigh < nodeCount; neigh++) {
                    if (matrix[current][neigh] == 0) continue; // Skip no links
                    if (cost[neigh] > cost[current] + matrix[current][neigh]) {
                        cost[neigh] = cost[current] + matrix[current][neigh];
                        parent[neigh] = current;
                        heap.push(neigh);
                        nodesPushed++;
                    }
                }
            }
        }
    }

    int source;
    int target;
    int nodeCount;

    T heap;
    std::vector<std::vector<int>> matrix;

    std::vector<int> shortestPath;
    int hopsOnShortestPath = 0;
    int shortestPathLength = std::numeric_limits<short>::max();
    int nodesPushed = 0;
    int nodesPopped = 0;
    double timeTaken;
};


int main(int argc, char ** argv) {
    std::string file;
    if(argv[1]){
        file = std::string(argv[1]);
    } else{
        file = "slideMatrix.txt";
    }
    std::cout << "==============STACK============" << std::endl;
    Search<std::stack<int>> search_s{0, 4, file};
    search_s.doSearch();
    search_s.printData();
    std::cout << "==============QUEUE============" << std::endl;
    Search<std::queue<int>> search_q{0, 4, file};
    search_q.doSearch();
    search_q.printData();
    std::cout << "=========PRIORITY QUEUE=========" << std::endl;
    Search<std::priority_queue<int>> search_pq{0, 4, file};
    search_pq.doSearch();
    search_pq.printData();
}
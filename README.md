# MapPathOptimizer
A dynamic routing engine which allows for parsing/processing data from OpenStreetMap files, and determines the optimal path/distance between any two road-based points. Output is generated in the form of a .geojson file which can be visually displayed via any geojson tool(ie. https://geojson.io/).

Routing Logic Summary:
1. The system splits up any parallel edges within the road system to ensure that the system is able to find the optimal solution.
2. A 2 dimensional edge-based graph is generated with appropriate costs (cost is based on geometeric distance computation between points).
3. A modified version of Djikstra's algorithm is applied (using a priority queue implementation) to allow for computing the shortest path between the nodes. The system is designed to be functional with arbritrarily large data sets (ie. all road data in California).

Note: Further optimizations in progress to add functionality for walking routes.

Example (.geojson file) Output:
![route-geojson](https://github.com/aelshako/MapPathOptimizer/assets/22120027/9b9770f6-8c1f-4f2c-a50e-52b9a8ca7c46)



#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include <random>

#include "delaunator.h"

using namespace std;
int main(int, char* argv[])
{
    std::cout << "benchmark: " <<  std::endl;
    
    std::vector<double> coords;

    int num_of_vertices = 1000000;
    
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,1000); // distribution in range [1, 6]
    
    for(int i = 0; i < num_of_vertices; i++)
    {
        double x = dist(rng);
        double y = dist(rng);
        
        coords.push_back(x);
        coords.push_back(y);
    }
    
    auto t_start = chrono::high_resolution_clock::now();
    Delaunator delaunator(coords);
    auto t_end = chrono::high_resolution_clock::now();

    auto milliseconds = chrono::duration_cast<chrono::milliseconds>(t_end - t_start).count();

    std::cout << std::endl;
    std::cout << "  runtime: " << milliseconds << " ms or " << milliseconds / 1000.0 << " sec " <<  std::endl;
    std::cout << std::endl;
    std::cout << "  number of vertices: " << (coords.size() / 2) << std::endl;
    std::cout << "  number of triangles: " <<( delaunator.triangles.size() / 3) << std::endl;
    std::cout << std::endl;

    return 0;
}

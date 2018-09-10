#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <string>
#include <vector>

#include "delaunator.h"

// test data
#include "vertex_points.h"
#include "triangle_indices.h"

using namespace std;

TEST_CASE("regular grid")
{
    std::vector<double> points;
    
    int w = 100;
    int h = 100;
    
    for(int r = 0; r < h; r++)
    {
        for(int c = 0; c < w; c++)
        {
            points.push_back(c);
            points.push_back(r);
        }
    }
    Delaunator dn;
    dn.triangulate(points);
    
     std::cout << "number of triangles: " << dn.triangles.size()/3 << std::endl;
     std::cout << "number of vertices: " << points.size()/2 << std::endl;
     CHECK(dn.triangles.size()/3 == (w - 1) * (h - 1) * 2);
}

TEST_CASE("failure case - missing triangle", "[Delaunator]")
{
    std::vector<double> vertex_list = {892,314,755,69,383,22,616,333,302,4,773,602,87,717,461,119,116,236,513,421,0,0,0,768,1024,0,1024,768};
    std::vector<int> tri_list_cpp_failure = {8,3,7,7,4,9,2,4,7,3,1,7,7,1,2,2,12,4,4,8,7,5,0,3,3,0,1,6,5,9,9,5,3,4,10,8,11,6,8,8,6,9,0,12,1,1,12,2,4,12,10,13,12,0,10,11,8,6,13,5,11,13,6,5,13,0};
    
    std::vector<int> tri_list_js = {9,3,7,7,8,9,2,4,7,3,1,7,7,1,2,2,12,4,4,8,7,5,0,3,3,0,1,6,5,9,9,5,3,4,10,8,11,6,8,8,6,9,0,12,1,1,12,2,4,12,10,13,12,0,10,11,8,6,13,5,11,13,6,5,13,0};
    
    Delaunator dn;
    dn.triangulate(vertex_list);
    
    CHECK(dn.triangles.size() == tri_list_js.size());
    for(int i = 0; i < dn.triangles.size(); i++)
    {
        CHECK(dn.triangles[i] == tri_list_js[i]);
    }
    
}

TEST_CASE("triangles match JS version ouput", "[Delaunator]")
{
    std::vector<double> vertex_list_big;
    unittests::init_vertex_list(vertex_list_big);
    
    Delaunator dn;
    dn.triangulate(vertex_list_big);
    
    std::vector<int> tri_big;
    unittests::init_triangle_list(tri_big);
    
    CHECK(dn.triangles.size() == tri_big.size());
    for(int i = 0; i < dn.triangles.size(); i++)
    {        
        CHECK(dn.triangles[i] == tri_big[i]);
    }
    
    //CHECK(true);
}


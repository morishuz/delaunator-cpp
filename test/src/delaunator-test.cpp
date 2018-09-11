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
    
    std::vector<std::vector<double>> vertex_lists;
    std::vector<std::vector<int>>    tri_lists;
    
    // 01
    vertex_lists.push_back({892,314,755,69,383,22,616,333,302,4,773,602,87,717,461,119,116,236,513,421,0,0,0,768,1024,0,1024,768});
    tri_lists.push_back({9,3,7,7,8,9,2,4,7,3,1,7,7,1,2,2,12,4,4,8,
        7,5,0,3,3,0,1,6,5,9,9,5,3,4,10,8,11,6,8,8,6,9,0,12,1,1,12,
        2,4,12,10,13,12,0,10,11,8,6,13,5,11,13,6,5,13,0});

    // 02
    vertex_lists.push_back({211,424,71,469,314,656,781,693,342,571,501,577,485,588,401,299,275,10,436,618,0,0,0,768,1024,0,1024,768});
    tri_lists.push_back({7,0,4,4,5,7,9,6,4,6,5,4,0,2,4,4,2,9,0,1,2,10,
        1,0,7,8,0,1,11,2,2,3,9,9,3,6,6,3,5,5,3,7,7,
        12,8,11,3,2,8,10,0,1,10,11,11,13,3,3,12,7,13,12,3,8,12,10});
//
    vertex_lists.push_back({487,73,1017,340,610,295,699,752,803,181,362,77,105,14,449,292,557,271,251,693,0,0,0,768,1024,0,1024,768});
    tri_lists.push_back({7,8,0,0,5,7,7,2,8,8,4,0,2,4,8,0,6,5,5,6,7,6,
        9,7,3,1,2,2,1,4,11,9,6,7,3,2,4,12,0,0,12,6,6,12,10,1,12,4,9,
        3,7,10,11,6,9,11,3,11,13,3,3,13,1,1,13,12});

    vertex_lists.push_back({441,271,383,99,887,320,304,707,468,526,773,653,478,156,66,50,300,586,17,320,0,0,0,768,1024,0,1024,768});
    tri_lists.push_back({0,6,1,8,4,0,0,2,6,1,9,0,7,9,1,9,8,0,1,10,7,7,10,
        9,4,2,0,6,12,1,11,3,8,8,3,4,4,5,2,3,5,4,13,12,2,2,12,6,1,
        12,10,9,11,8,3,13,5,10,11,9,11,13,3,5,13,2});

    vertex_lists.push_back({608,648,322,171,732,241,327,72,16,537,467,761,765,560,162,584,296,459,283,355,0,0,0,768,1024,0,1024,768});
    tri_lists.push_back({8,9,7,9,4,7,2,1,9,9,10,4,4,11,7,7,5,8,1,10,
        9,3,10,1,4,10,11,11,5,7,8,2,9,1,2,3,5,0,8,0,2,8,13,6,0,
        0,6,2,11,13,5,5,13,0,6,12,2,13,12,6,2,12,3,3,12,10});

    vertex_lists.push_back({576,330,193,364,269,202,581,322,297,665,404,138,248,24,671,448,509,231,830,345,0,0,0,768,1024,0,1024,768});
    tri_lists.push_back({0,3,8,8,1,0,0,7,3,4,7,0,5,2,8,7,9,3,3,9,
        8,8,12,5,2,1,8,5,6,2,2,10,1,12,6,5,1,4,0,9,12,8,12,
        10,6,6,10,2,1,11,4,7,13,9,9,13,12,4,13,7,10,11,1,4,11,13});

    
    vertex_lists.push_back({336,439,172,100,582,180,969,433,684,179,420,527,884,551,659,561,355,360,495,601,0,0,0,768,1024,0,1024,768});
    tri_lists.push_back({8,0,5,5,2,8,9,7,5,7,2,5,7,4,2,2,1,8,8,1,0,10,1,
        12,13,6,7,7,6,4,6,3,4,0,11,5,5,11,9,9,13,7,1,11,0,12,
        1,2,1,10,11,11,13,9,6,13,3,3,12,4,4,12,2,13,12,3});
//
//    list size: 28
//coords:
//    {489,662,376,618,421,269,682,428,814,106,778,698,653,565,688,29,324,230,876,287,0,0,0,768,1024,0,1024,768};
//
//triangles:
//    {2,8,1,1,3,2,0,6,1,6,3,1,8,11,1,1,11,0,7,10,8,8,10,11,7,8,2,0,5,6,6,5,3,11,5,0,4,7,3,3,7,2,5,9,3,9,4,3,11,13,5,5,13,9,13,12,9,9,12,4,4,12,7,7,12,10};
//
//    list size: 28
//coords:
//    {25,239,29,728,300,67,349,524,502,487,51,452,805,643,147,68,509,655,344,87,0,0,0,768,1024,0,1024,768};
//
//triangles:
//    {4,3,8,8,6,4,0,5,3,3,1,8,5,1,3,8,13,6,5,11,1,1,11,8,4,9,3,12,9,4,9,0,3,2,0,9,5,0,11,2,7,0,11,13,8,6,12,4,2,10,7,7,10,0,0,10,11,13,12,6,9,12,2,2,12,10};
//
//    list size: 28
//coords:
//    {71,542,827,59,852,696,155,528,483,748,875,422,77,610,942,467,279,600,281,749,0,0,0,768,1024,0,1024,768};
//
//triangles:
//    {8,3,6,3,0,6,6,9,8,0,11,6,6,11,9,11,4,9,9,4,8,3,10,0,0,10,11,1,10,3,1,3,8,4,5,8,2,5,4,2,7,5,4,13,2,2,13,7,11,13,4,12,1,5,5,1,8,13,12,7,7,12,5,1,12,10};
//
//    list size: 28
//coords:
//    {150,8,980,27,207,23,355,635,643,337,5,147,559,417,515,699,658,327,17,129,0,0,0,768,1024,0,1024,768};
//
//triangles:
//    {6,4,2,13,8,4,4,8,2,2,5,6,0,9,2,9,5,2,5,3,6,6,13,4,9,10,5,0,10,9,3,7,6,13,1,8,8,1,2,2,12,0,0,12,10,13,12,1,1,12,2,5,11,3,3,11,7,10,11,5,11,13,7,7,13,6};
//
//MoBook:debug_delaunator mo$ node run_cpp_failure_mode
//    list size: 28
//coords:
//    {892,314,755,69,383,22,616,333,302,4,773,602,87,717,461,119,116,236,513,421,0,0,0,768,1024,0,1024,768};
//
//triangles:
//    {9,3,7,7,8,9,2,4,7,3,1,7,7,1,2,2,12,4,4,8,7,5,0,3,3,0,1,6,5,9,9,5,3,4,10,8,11,6,8,8,6,9,0,12,1,1,12,2,4,12,10,13,12,0,10,11,8,6,13,5,11,13,6,5,13,0};
//
//    list size: 28
//coords:
//    {211,424,71,469,314,656,781,693,342,571,501,577,485,588,401,299,275,10,436,618,0,0,0,768,1024,0,1024,768};
//
//triangles:
//    {7,0,4,4,5,7,9,6,4,6,5,4,0,2,4,4,2,9,0,1,2,10,1,0,7,8,0,1,11,2,2,3,9,9,3,6,6,3,5,5,3,7,7,12,8,11,3,2,8,10,0,1,10,11,11,13,3,3,12,7,13,12,3,8,12,10};
//
//    list size: 28
//coords:
//    {487,73,1017,340,610,295,699,752,803,181,362,77,105,14,449,292,557,271,251,693,0,0,0,768,1024,0,1024,768};
//
//triangles:
//    {7,8,0,0,5,7,7,2,8,8,4,0,2,4,8,0,6,5,5,6,7,6,9,7,3,1,2,2,1,4,11,9,6,7,3,2,4,12,0,0,12,6,6,12,10,1,12,4,9,3,7,10,11,6,9,11,3,11,13,3,3,13,1,1,13,12};
//
//    list size: 28
//coords:
//    {441,271,383,99,887,320,304,707,468,526,773,653,478,156,66,50,300,586,17,320,0,0,0,768,1024,0,1024,768};
//
//triangles:
//    {0,6,1,8,4,0,0,2,6,1,9,0,7,9,1,9,8,0,1,10,7,7,10,9,4,2,0,6,12,1,11,3,8,8,3,4,4,5,2,3,5,4,13,12,2,2,12,6,1,12,10,9,11,8,3,13,5,10,11,9,11,13,3,5,13,2};
//
//    list size: 28
//coords:
//    {608,648,322,171,732,241,327,72,16,537,467,761,765,560,162,584,296,459,283,355,0,0,0,768,1024,0,1024,768};
//
//triangles:
//    {8,9,7,9,4,7,2,1,9,9,10,4,4,11,7,7,5,8,1,10,9,3,10,1,4,10,11,11,5,7,8,2,9,1,2,3,5,0,8,0,2,8,13,6,0,0,6,2,11,13,5,5,13,0,6,12,2,13,12,6,2,12,3,3,12,10};
//
//    list size: 28
//coords:
//    {576,330,193,364,269,202,581,322,297,665,404,138,248,24,671,448,509,231,830,345,0,0,0,768,1024,0,1024,768};
//
//triangles:
//    {0,3,8,8,1,0,0,7,3,4,7,0,5,2,8,7,9,3,3,9,8,8,12,5,2,1,8,5,6,2,2,10,1,12,6,5,1,4,0,9,12,8,12,10,6,6,10,2,1,11,4,7,13,9,9,13,12,4,13,7,10,11,1,4,11,13};
//
//    list size: 28
//coords:
//    {336,439,172,100,582,180,969,433,684,179,420,527,884,551,659,561,355,360,495,601,0,0,0,768,1024,0,1024,768};
//
//triangles:
//    {8,0,5,5,2,8,9,7,5,7,2,5,7,4,2,2,1,8,8,1,0,10,1,12,13,6,7,7,6,4,6,3,4,0,11,5,5,11,9,9,13,7,1,11,0,12,1,2,1,10,11,11,13,9,6,13,3,3,12,4,4,12,2,13,12,3};
//
//    list size: 28
//coords:
//    {489,662,376,618,421,269,682,428,814,106,778,698,653,565,688,29,324,230,876,287,0,0,0,768,1024,0,1024,768};
//
//triangles:
//    {2,8,1,1,3,2,0,6,1,6,3,1,8,11,1,1,11,0,7,10,8,8,10,11,7,8,2,0,5,6,6,5,3,11,5,0,4,7,3,3,7,2,5,9,3,9,4,3,11,13,5,5,13,9,13,12,9,9,12,4,4,12,7,7,12,10};
//
//    list size: 28
//coords:
//    {25,239,29,728,300,67,349,524,502,487,51,452,805,643,147,68,509,655,344,87,0,0,0,768,1024,0,1024,768};
//
//triangles:
//    {4,3,8,8,6,4,0,5,3,3,1,8,5,1,3,8,13,6,5,11,1,1,11,8,4,9,3,12,9,4,9,0,3,2,0,9,5,0,11,2,7,0,11,13,8,6,12,4,2,10,7,7,10,0,0,10,11,13,12,6,9,12,2,2,12,10};
//
//    list size: 28
//coords:
//    {71,542,827,59,852,696,155,528,483,748,875,422,77,610,942,467,279,600,281,749,0,0,0,768,1024,0,1024,768};
//
//triangles:
//    {8,3,6,3,0,6,6,9,8,0,11,6,6,11,9,11,4,9,9,4,8,3,10,0,0,10,11,1,10,3,1,3,8,4,5,8,2,5,4,2,7,5,4,13,2,2,13,7,11,13,4,12,1,5,5,1,8,13,12,7,7,12,5,1,12,10};
//
//    list size: 28
//coords:
//    {150,8,980,27,207,23,355,635,643,337,5,147,559,417,515,699,658,327,17,129,0,0,0,768,1024,0,1024,768};
//
//triangles:
//    {6,4,2,13,8,4,4,8,2,2,5,6,0,9,2,9,5,2,5,3,6,6,13,4,9,10,5,0,10,9,3,7,6,13,1,8,8,1,2,2,12,0,0,12,10,13,12,1,1,12,2,5,11,3,3,11,7,10,11,5,11,13,7,7,13,6};
    
    
    
    
    
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
    
//    for(int i = 0; i < dn.triangles.size(); i++)
//    {
//        CHECK(dn.triangles[i] == tri_list_cpp_failure[i]);
//    }
    
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


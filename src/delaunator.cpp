
#include "delaunator.h"
#include <algorithm>
#include <cstdio>
#include <limits>
#include <tuple>
#include <stdexcept>
#include <cmath>
#include <iostream>

using namespace std;

namespace {
    const double max_double = numeric_limits<double>::max();
    double dist(
        const double &ax,
        const double &ay,
        const double &bx,
        const double &by
    ) {
        const double dx = ax - bx;
        const double dy = ay - by;
        return dx * dx + dy * dy;
    }
    double circumradius(
        const double &ax,
        const double &ay,
        const double &bx,
        const double &by,
        const double &cx,
        const double &cy
    ) {
        const double dx = bx - ax;
        const double dy = by - ay;
        const double ex = cx - ax;
        const double ey = cy - ay;

        const double bl = dx * dx + dy * dy;
        const double cl = ex * ex + ey * ey;
        const double d = dx * ey - dy * ex;

        const double x = (ey * bl - dy * cl) * 0.5 / d;
        const double y = (dx * cl - ex * bl) * 0.5 / d;

        if (bl && cl && d) {
            return x * x + y * y;
        } else {
            return max_double;
        }
    }
 
    bool orient(
                 const double px,
                 const double py,
                 const double qx,
                 const double qy,
                 const double rx,
                 const double ry
                 )
    {
        return ( (qy - py) * (rx - qx) - (qx - px) * (ry - qy) ) < 0.0;
    }

    tuple<double, double> circumcenter(
        const double &ax,
        const double &ay,
        const double &bx,
        const double &by,
        const double &cx,
        const double &cy
    ) {
        const double dx = bx - ax;
        const double dy = by - ay;
        const double ex = cx - ax;
        const double ey = cy - ay;

        const double bl = dx * dx + dy * dy;
        const double cl = ex * ex + ey * ey;
        const double d = dx * ey - dy * ex;

        const double x = ax + (ey * bl - dy * cl) * 0.5 / d;
        const double y = ay + (dx * cl - ex * bl) * 0.5 / d;

        return make_tuple(x, y);
    }
    
    double compare(
        const vector<double> &coords,
        uint64_t i,
        uint64_t j,
        double cx,
        double cy
    ) {
        const double d1 = dist(coords[2 * i], coords[2 * i + 1], cx, cy);
        const double d2 = dist(coords[2 * j], coords[2 * j + 1], cx, cy);
        const double diff1 = d1 - d2;
        const double diff2 = coords[2 * i] - coords[2 * j];
        const double diff3 = coords[2 * i + 1] - coords[2 * j + 1];

        if (diff1) {
            return diff1;
        } else if(diff2) {
            return diff2;
        } else {
            return diff3;
        }
    }

    bool in_circle(
        double ax, double ay,
        double bx, double by,
        double cx, double cy,
        double px, double py
    ) {
        const double dx = ax - px;
        const double dy = ay - py;
        const double ex = bx - px;
        const double ey = by - py;
        const double fx = cx - px;
        const double fy = cy - py;

        const double ap = dx * dx + dy * dy;
        const double bp = ex * ex + ey * ey;
        const double cp = fx * fx + fy * fy;

        return (dx * (ey * cp - bp * fy) -
            dy * (ex * cp - bp * fx) +
            ap * (ex * fy - ey * fx)) < 0;
    }
}

Delaunator::Delaunator()
{
    m_epsilon = std::pow(2,-52);
}

Delaunator::Delaunator(const vector<double>& coords) : Delaunator()
{
    const int64_t n = coords.size() >> 1;
    double max_x = -1 * max_double;
    double max_y = -1 * max_double;
    double min_x = max_double;
    double min_y = max_double;
    
    vector<int64_t> ids;
    ids.reserve(n);
    
    for (int64_t i = 0; i < n; i++)
    {
        const double x = coords[2 * i];
        const double y = coords[2 * i + 1];

        if (x < min_x) min_x = x;
        if (y < min_y) min_y = y;
        if (x > max_x) max_x = x;
        if (y > max_y) max_y = y;

        ids.push_back(i);
    }
    const double cx = (min_x + max_x) / 2.0;
    const double cy = (min_y + max_y) / 2.0;
    double min_dist = max_double;
    uint64_t i0;
    uint64_t i1;
    uint64_t i2;

    // pick a seed point close to the centroid
    for (uint64_t i = 0; i < n; i++) {
        const double d = dist(cx, cy, coords[2 * i], coords[2 * i + 1]);
        if (d < min_dist) {
            i0 = i;
            min_dist = d;
        }
    }

    min_dist = max_double;

    // find the point closest to the seed
    for (uint64_t i = 0; i < n; i++) {
        if (i == i0) continue;
        const double d = dist(coords[2 * i0], coords[2 * i0 + 1], coords[2 * i], coords[2 * i + 1]);
        if (d < min_dist && d > 0)
        {
            i1 = i;
            min_dist = d;
        }
    }

    double min_radius = max_double;

    // find the third point which forms the smallest circumcircle with the first two
    for (uint64_t i = 0; i < n; i++)
    {
        if (i == i0 || i == i1) continue;

        const double r = circumradius(
            coords[2 * i0], coords[2 * i0 + 1],
            coords[2 * i1], coords[2 * i1 + 1],
            coords[2 * i], coords[2 * i + 1]);

        if (r < min_radius)
        {
            i2 = i;
            min_radius = r;
        }
    }

    if (min_radius == max_double) {
        throw runtime_error("not triangulation");;
    }
    
    if (orient(
            coords[2 * i0], coords[2 * i0 + 1],
            coords[2 * i1], coords[2 * i1 + 1],
            coords[2 * i2], coords[2 * i2 + 1]))
    {
        const double tmp = i1;
        i1 = i2;
        i2 = tmp;
    }

    const double i0x = coords[2 * i0];
    const double i0y = coords[2 * i0 + 1];
    const double i1x = coords[2 * i1];
    const double i1y = coords[2 * i1 + 1];
    const double i2x = coords[2 * i2];
    const double i2y = coords[2 * i2 + 1];

    tie(m_center_x, m_center_y) = circumcenter(i0x, i0y, i1x, i1y, i2x, i2y);
    
    std::sort(ids.begin(), ids.end(), [&](int64_t i, int64_t j)
    {
        return compare(coords, i, j, m_center_x, m_center_y) < 0;
    });

    m_hash_size = ceil(sqrt(n));
    m_hash.reserve(m_hash_size);
    for (int i = 0; i < m_hash_size; i++) m_hash.push_back(-1);

    m_hl.reserve(coords.size());

    m_hull_index = insert_node(i0,coords);
    int64_t e = m_hull_index;
    hash_edge(e);
    m_hl[e].t = 0;

    e = insert_node(i1, e,coords);
    hash_edge(e);
    m_hl[e].t = 1;

    e = insert_node(i2, e,coords);
    hash_edge(e);
    m_hl[e].t = 2;

    const int64_t max_triangles = 2 * n - 5;
    triangles.reserve(max_triangles * 3);
    halfedges.reserve(max_triangles * 3);
    add_triangle(i0, i1, i2, -1, -1, -1);
    
    double xp = NAN;
    double yp = NAN;
   
    for (int64_t k = 0; k < ids.size(); k++)
    {
        const int64_t i = ids[k];
        const double x = coords[2 * i];
        const double y = coords[2 * i + 1];

        if (k > 0 && std::abs(x - xp) <= m_epsilon && std::abs(y - yp) <= m_epsilon) continue;
        
        xp = x;
        yp = y;
        
        if (
                (x == i0x && y == i0y) ||
                (x == i1x && y == i1y) ||
                (x == i2x && y == i2y)
        ) continue;

        const int64_t start_key = hash_key(x, y);
        int64_t key = start_key;
        int64_t start = -1;
        do {
            start = m_hash[key];
            key = (key + 1) % m_hash_size;
        } while(
            (start < 0 || m_hl[start].removed) &&
            (key != start_key)
        );
        
        start   = m_hl[start].prev;
        e       = start;

        while(!orient(x, y,m_hl[e].x, m_hl[e].y,m_hl[m_hl[e].next].x, m_hl[m_hl[e].next].y))
        {
            e = m_hl[e].next;
            
            if (e == start)
            {
                e = -1;
                break;
            }
        }
        
        if(e == -1)
        {
            continue;
        }

        const bool walk_back = e == start;

        // add the first triangle from the point
        int64_t t = add_triangle(
            m_hl[e].i,
            i,
            m_hl[m_hl[e].next].i,
            -1, -1, m_hl[e].t
        );

        m_hl[e].t = t; // keep track of boundary triangles on the hull
        e = insert_node(i, e,coords);

        // recursively flip triangles from the point until they satisfy the Delaunay condition
        m_hl[e].t = legalize(t + 2,e,coords);
        
        // walk forward through the hull, adding more triangles and flipping recursively
        int64_t q = m_hl[e].next;
        while(orient(
                x, y,
                m_hl[q].x, m_hl[q].y,
                m_hl[m_hl[q].next].x, m_hl[m_hl[q].next].y))
        {
            
            t = add_triangle(
                m_hl[q].i, i,
                m_hl[m_hl[q].next].i, m_hl[m_hl[q].prev].t,
                -1, m_hl[q].t
            );
            
            m_hl[m_hl[q].prev].t    = legalize(t + 2,e,coords);
            m_hull_index            = remove_node(q);
            q                       = m_hl[q].next;
        }
        
        if (walk_back)
        {
            // walk backward from the other side, adding more triangles and flipping
            q = m_hl[e].prev;
            while(
                orient(
                    x, y,
                    m_hl[m_hl[q].prev].x, m_hl[m_hl[q].prev].y,
                    m_hl[q].x, m_hl[q].y))
            {
                t = add_triangle(
                    m_hl[m_hl[q].prev].i, i,
                    m_hl[q].i, -1,
                    m_hl[q].t, m_hl[m_hl[q].prev].t
                );
                
                legalize(t + 2,e,coords);
                m_hl[m_hl[q].prev].t    = t;
                m_hull_index            = remove_node(q);
                q                       = m_hl[q].prev;
            }
        }
        hash_edge(e);
        hash_edge(m_hl[e].prev);
    }

};

int64_t Delaunator::remove_node(int64_t node) {
    m_hl[m_hl[node].prev].next = m_hl[node].next;
    m_hl[m_hl[node].next].prev = m_hl[node].prev;
    m_hl[node].removed = true;
    return m_hl[node].prev;
}

int64_t Delaunator::legalize(int64_t a, int64_t& e,const std::vector<double>& coords)
{
    
    const int64_t b = halfedges[a];
    
    /* if the pair of triangles doesn't satisfy the Delaunay condition
     * (p1 is inside the circumcircle of [p0, pl, pr]), flip them,
     * then do the same check/flip recursively for the new pair of triangles
     *
     *           pl                    pl
     *          /||\                  /  \
     *       al/ || \bl            al/    \a
     *        /  ||  \              /      \
     *       /  a||b  \    flip    /___ar___\
     *     p0\   ||   /p1   =>   p0\---bl---/p1
     *        \  ||  /              \      /
     *       ar\ || /br             b\    /br
     *          \||/                  \  /
     *           pr                    pr
     */

    const int64_t a0 = a - a % 3;
    const int64_t b0 = b - b % 3;

    const int64_t al = a0 + (a + 1) % 3;
    const int64_t ar = a0 + (a + 2) % 3;
    const int64_t bl = b0 + (b + 2) % 3;
    
    if (b == -1) return ar;

    const int64_t p0 = triangles[ar];
    const int64_t pr = triangles[a];
    const int64_t pl = triangles[al];
    const int64_t p1 = triangles[bl];

    const bool illegal = in_circle(
            coords[2 * p0], coords[2 * p0 + 1],
            coords[2 * pr], coords[2 * pr + 1],
            coords[2 * pl], coords[2 * pl + 1],
            coords[2 * p1], coords[2 * p1 + 1]
    );

    if (illegal)
    {
        triangles[a] = p1;
        triangles[b] = p0;
        
        const int64_t hbl = halfedges[bl];
        // edge swapped on the other side of the hull (rare); fix the halfedge reference
        if (hbl == -1)
        {
            e = m_hull_index;
            do
            {
                if (m_hl[e].t == bl)
                {
                    m_hl[e].t = a;
                    break;
                }
                e = m_hl[e].next;
            }
            while (e != m_hull_index);
        }

        link(a, hbl);
        link(b, halfedges[ar]);
        link(ar, bl);

        const int64_t br = b0 + (b + 1) % 3;

        legalize(a,e,coords);
        return legalize(br,e,coords);
    }
    return ar;
}

int64_t Delaunator::insert_node(int64_t i, int64_t prev,const std::vector<double>& coords)
{
    const int64_t node = insert_node(i,coords);
    m_hl[node].next = m_hl[prev].next;
    m_hl[node].prev = prev;
    m_hl[m_hl[node].next].prev = node;
    m_hl[prev].next = node;
    return node;
};

int64_t Delaunator::insert_node(int64_t i,const std::vector<double>& coords)
{
    int64_t node = m_hl.size();
    
    DelaunatorPoint p = {
        .i = i,
        .x = coords[2 * i],
        .y = coords[2 * i + 1],
        .prev = node,
        .next = node,
        .removed = false
    };
    
    m_hl.push_back(move(p));
    return node;
}

double Delaunator::pseudo_angle(const double dx, const double dy)
{
    const double p = dx / ( std::abs(dx) + std::abs(dy) );
    return (dy > 0 ? 3 - p : 1 + p) / 4;
}

int64_t Delaunator::hash_key(const double x,const double y)
{
    int64_t key = (int64_t)(std::floor( pseudo_angle( x - m_center_x, y-m_center_y) * m_hash_size));
    return key % m_hash_size;
}

void Delaunator::hash_edge(int64_t e)
{

    m_hash[hash_key(m_hl[e].x, m_hl[e].y)] = e;
}

int64_t Delaunator::add_triangle(
    int64_t i0, int64_t i1, int64_t i2,
    int64_t a, int64_t b, int64_t c
) {
    const int64_t t = triangles.size();
    triangles.push_back(i0);
    triangles.push_back(i1);
    triangles.push_back(i2);
    link(t, a);
    link(t + 1, b);
    link(t + 2, c);
    return t;
}

void Delaunator::link(int64_t a, int64_t b) {
    int64_t s  = halfedges.size();
    if (a == s) {
        halfedges.push_back(b);
    } else if (a < s) {
        halfedges[a] = b;
    } else {
        throw runtime_error("Cannot link edge");
    }
    if (b != -1) {
        int64_t s  = halfedges.size();
        if (b == s) {
            halfedges.push_back(a);
        } else if (b < s) {
            halfedges[b] = a;
        }  else {
            throw runtime_error("Cannot link edge");
        }
    }
};

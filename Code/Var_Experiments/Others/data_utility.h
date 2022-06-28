#ifndef DATA_UTILITY_H
#define DATA_UTILITY_H
#include "data_struct.h"
#include "pruning.h"

// sort the 2d points clockwise
struct angleCmp
{
    bool operator()(const point_t* lhs, const point_t* rhs) const
    {
        double lhs_angle = lhs->coord[1] == 0 ? PI / 2 : atan(lhs->coord[0] / lhs->coord[1]);
        double rhs_angle = rhs->coord[1] == 0 ? PI / 2 : atan(rhs->coord[0] / rhs->coord[1]);
        return lhs_angle < rhs_angle;
    }
};

struct anglehyperCmp
{
    bool operator()(const hyperplane_t * lhs, const hyperplane_t* rhs) const
    {
        double lhs_angle = lhs->normal->coord[1] == 0 ? PI / 2 : atan(lhs->normal->coord[0] / lhs->normal->coord[1]);
        double rhs_angle = rhs->normal->coord[1] == 0 ? PI / 2 : atan(rhs->normal->coord[0] / rhs->normal->coord[1]);
        return lhs_angle < rhs_angle;
    }
};

struct valueCmp
{
    bool operator()(const point_t* lhs, const point_t* rhs) const
    {
        double lhs_value = lhs->value;
        double rhs_value = rhs->value;
        return lhs_value < rhs_value;
    }
};

// allocate/release memory for points/hyperplanes 
point_t* alloc_point(int dim);
point_t* alloc_point(int dim, int id);
point_t *deepcopy_point(const point_t *p);
void release_point( point_t* &point_v);
point_t* rand_point(int dim);
point_set_t* alloc_point_set(int numberOfPoints);
void release_point_set(point_set_t* &point_set_v, bool clear);
void print_pointset(point_set_t* p_set);

utility_t* alloc_utility();

hyperplane_t* alloc_hyperplane(point_t* normal, double offset);
hyperplane_t* alloc_hyperplane(point_t* p_1, point_t* p_2, double offset);
hyperplane_t* deepcopy_hyperplane(const hyperplane_t *h);
void release_hyperplane(hyperplane_t* &hyperplane_v);

cluster_t* alloc_cluster();
void release_cluster(cluster_t* cluster_v);
s_node_t* alloc_s_node(int dim);
point_count_t* alloc_point_count();

halfspace_t* alloc_halfspace(point_t* p_1, point_t* p_2, double offset, bool direction);
halfspace_t* deepcopy_halfspace(const halfspace_t* h);
void release_halfspace(halfspace_t* &halfspace_v);
halfspace_set_t* alloc_halfspace_set(int dim);
halfspace_set_t* alloc_halfspace_set(halfspace_set_t *hset);
halfspace_set_t* deepcopy_halfspace_set(const halfspace_set_t* hss);
void release_halfspace_set(halfspace_set_t* &halfspace_set_v);

choose_item *deepcopy_choose_item(const choose_item *item);
void release_choose_item(choose_item *&c_i);


// print informaiton
void print_point(point_t* point_v);
void print_point_set(point_set_t* point_set_v);
void print_point_set(std::vector<point_t*> p_set);

#endif
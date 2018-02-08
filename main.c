// KHALID & VIDYA




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_DIM 128
#define N 10000
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
struct kdtree{
    double x[MAX_DIM];
    struct kdtree *left, *right;
};
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//function to find the distance among the points
double distance(struct kdtree *a, struct kdtree *b, int dim)
{
    double t, dist = 0;
    while (dim--) {
        t = a->x[dim] - b->x[dim];
        dist += t * t;
    }
    return dist;
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//function to move the data points according to the build kdtree
void swap(struct kdtree *x, struct kdtree *y) {
    double tmp[MAX_DIM];
    memcpy(tmp,  x->x, sizeof(tmp));
    memcpy(x->x, y->x, sizeof(tmp));
    memcpy(y->x, tmp,  sizeof(tmp));
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//function to find the median in order to check the boundry of the cluster
struct kdtree*
boundry_cluster(struct kdtree *start, struct kdtree *end, int idx)
{
    if (end <= start) return NULL;
    if (end == start + 1)
        return start;
    struct kdtree *p, *array_store, *boundry = start + (end - start) / 2;
    double median;
    while (1) {
        median = boundry->x[idx];
        swap(boundry, end - 1);
        for (array_store = p = start; p < end; p++) {
            if (p->x[idx] < median) {
                if (p != array_store)
                    swap(p, array_store);
                array_store++;
            }
        }
        swap(array_store, end - 1);
        /* median has duplicate values */
        if (array_store->x[idx] == boundry->x[idx])
            return boundry;

        if (array_store > boundry) end = array_store;
        else        start = array_store;
    }
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//function to build the Kdtree
struct kdtree*
build_tree(struct kdtree *t, int len, int i, int side)
{
    int c,d;
    if (!len) return 0;
    printf("\n-----------------------------------------------------------------------\n");
    if(side==0)
        printf("Center of the tree..\n");
    else if (side==1)
        printf("Right side of the parent node..\n");
    else if (side==-1)
        printf("Left side of the parent node..\n");
    printf("dimension (%d) \n", i);
    for(c=0;c<len; c++)
    {
        printf("( ");
        for( d=0; d< MAX_DIM; d++)
        {
            printf("%g  ",t[c].x[d]);
        }
        printf(")\n");
    }
    struct kdtree *n;
    if ((n = boundry_cluster(t, t + len, i))) {
        if(!(len==1))
            i = find_variance(t, len);
        else
            i = (i + 1) % MAX_DIM;
        n->left  = build_tree(t, n - t, i, -1);
        n->right = build_tree(n + 1, t + len - (n + 1), i, 1);
    }
    return n;
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//function to find the mean of datapoints along with each dimension
int find_mean(struct kdtree *t,int len, int dim){
    int divval,e;
    int sum=0;
    for(e=0;e<len;e++){
        sum += t[e].x[dim];
    }
    divval =sum/len;
    return divval;
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//function to find the variance using the mean and pass it for partition
int find_variance(struct kdtree *t,int len){
    int mean = 0,g,h;
    int subval, sqval, variance=0, current_variance=0;
    int sum=0;
    int dim_of_biggest_variance=-1;
    for(g=0;g<MAX_DIM; g++)
    {
        mean= find_mean(t, len, g);
        for(h=0;h<len;h++){
            subval= t[h].x[g] - mean;
            sqval=sqrt(subval);
            sum += sqval;
        }
        current_variance = sum/(len-1);
        if(dim_of_biggest_variance==-1 || current_variance>variance)
        {
            variance=current_variance;
            dim_of_biggest_variance=g;
        }
    }
    return dim_of_biggest_variance;
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
int visited;
//function to search the nearest point from the query point
void search(struct kdtree *root, struct kdtree *query, int i, int dim, struct kdtree **best, double *best_dist)
{
    double d, dx, dx2;
    if (!root) return;
    d = distance(root, query, dim);
    dx = root->x[i] - query->x[i];
    dx2 = dx * dx;
    visited ++;
    if (!*best || d < *best_dist) {
        *best_dist = d;
        *best = root;
    }
    /* if chance of exact match is high */
    if (!*best_dist) return;
    if (++i >= dim) i = 0;
    search(dx > 0 ? root->left : root->right, query, i, dim, best, best_dist);
    if (dx2 >= *best_dist) return;
    search(dx > 0 ? root->right : root->left, query, i, dim, best, best_dist);
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
int main(void)
{
    int m,n,ndata;
    struct kdtree querypoint = {{9, 2}};
    struct kdtree *root, *found, *data_array;
    double best_dist;
    data_array =(struct kdtree*) calloc(N, sizeof(struct kdtree));
    srand(time(NULL));
    for (m = 0; m < N; m++)
    {
        for(n=0; n< MAX_DIM; n++)
        {
            data_array[m].x[n] = rand() % 10 + 1;
        }
        printf("( ");
        for( n=0; n< MAX_DIM; n++)
        {
            printf("%g ",data_array[m].x[n]);
        }
        printf(")\n");
    }
    root = build_tree(data_array, N, 0,0);
    for(n=0; n< MAX_DIM; n++)
    {
        querypoint.x[n] = rand() % 10 + 1;
    }
    visited = 0;
    found = 0;
    search(root, &querypoint, 0, 3, &found, &best_dist);
    printf("\n-----------------------------------------------------------------------\n");
    printf("\nSearching for ( ");
    for(n=0; n< MAX_DIM; n++)
    {
        printf("%g  ",querypoint.x[n]);
    }
    printf(")...\n");
    printf("found near to ( ");
    for(n=0; n< MAX_DIM; n++)
    {
        printf("%g  ",found->x[n]);
    }
    printf(")\nwith distance = %g.\n", sqrt(best_dist));
    printf("Number of visited clusters in searching process = %d",visited);
    return 0;
}
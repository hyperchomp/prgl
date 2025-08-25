#include "mesh.h"
#include "cglm/types.h"

void pr3d_calculate_aabb(
    const float *vertices, int num_vertices, vec3 min_bounds, vec3 max_bounds
)
{
    // Initialize bounds using first vertex
    min_bounds[0] = vertices[0];
    min_bounds[1] = vertices[1];
    min_bounds[2] = vertices[2];

    max_bounds[0] = vertices[0];
    max_bounds[1] = vertices[1];
    max_bounds[2] = vertices[2];

    for (int i = 0; i < num_vertices; i++)
    {
        // Find min bounds from the points in the vertices
        for (int p = 0; p < 3; p++)
        {
            if (vertices[i * 3 + p] < min_bounds[p])
            {
                min_bounds[p] = vertices[i * 3 + p];
            }
        }

        // Find max bounds
        for (int p = 0; p < 3; p++)
        {
            if (vertices[i * 3 + p] > max_bounds[p])
            {
                max_bounds[p] = vertices[i * 3 + p];
            }
        }
    }
}

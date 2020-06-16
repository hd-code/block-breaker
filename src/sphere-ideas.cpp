#define _USE_MATH_DEFINES
#include <cmath>

struct SVector3 {
    int x, y, z;
};

struct SVertex {
    SVector3 position;
};

const float RADIUS = 1.0f;
const float PI_2 = 2 * M_PI;

// -----------------------------------------------------------------------------

template<size_t n>
void makeCircle(float radius, float y, SVertex (&vertices)[n]) {
    float stepSize = PI_2 / n;
    float angle = 0;

    for (int i = 0; i < n; i++) {
        vertices[i].position.x = cos(stepSize) * radius;
        vertices[i].position.y = y;
        vertices[i].position.z = sin(stepSize) * radius;

        angle += stepSize;
    }
}

void makePoles(float radius, SVertex vertices[2]) {
    vertices[0].position.x = 0;
    vertices[0].position.y = radius;
    vertices[0].position.z = 0;

    vertices[1].position.x = 0;
    vertices[1].position.y = -radius;
    vertices[1].position.z = 0;
}

template<size_t n>
void fillAscending(int (&result)[n]) {
    for (int i = 0; i < n; i++) {
        result[i] = i;
    }
}

template<size_t n>
void fillAscending(int (&result)[n], int from) {
    for (int i = 0; i < n; i++) {
        result[i] = from + i;
    }
}

template<size_t n>
void reverseArray(int (&original)[n], int (&result)[n]) {
    for (int i = 0; i < n; i++) {
        result[i] = original[n-1-i];
    }
}

template<size_t n>
void triangulateDome(int centerIndex, int (&circleIndices)[n], int (&triangles)[n][3]) {
    // gluing together start and end of circle
    triangles[0][0] = centerIndex;
    triangles[0][1] = circleIndices[n-1];
    triangles[0][2] = circleIndices[0];

    // connecting all vertices between start and end
    for (int i = 1; i < n; i++) {
        triangles[i][0] = centerIndex;
        triangles[i][1] = circleIndices[i-1];
        triangles[i][2] = circleIndices[i];
    }
}

template<size_t n>
void triangulateBarrel(int topIndices[n], int (&bottomIndices)[n], int (&triangles)[2*n][3]) {
    // gluing together start and end of barrel
    triangles[0][0] = bottomIndices[n-1];
    triangles[0][1] = bottomIndices[0];
    triangles[0][2] = topIndices[0];

    triangles[1][0] = bottomIndices[n-1];
    triangles[1][1] = topIndices[0];
    triangles[1][2] = topIndices[n-1];

    // connecting all vertices between start and end
    for (int i = 1; i < n; i++) {
        triangles[2*i][0] = bottomIndices[i-1];
        triangles[2*i][1] = bottomIndices[i];
        triangles[2*i][2] = topIndices[i];

        triangles[2*i+1][0] = bottomIndices[i-1];
        triangles[2*i+1][1] = topIndices[i];
        triangles[2*i+1][2] = topIndices[i-1];
    }
}

// -----------------------------------------------------------------------------



/*
        *
    * * * * *       => n = 5
  *  *  *  *  *     => n = 5
    * * * * *       => n = 5
        *

=> m = 3
*/
// n..number of vertices per circle
// m..number of circles
template<size_t N, size_t M>
void makeSphere(float radius, int n, int m, SVertex vertices[N*M + 2], int triangles[2*N*M][3]) {
    int numOfVertices = n*m + 2;
    int numOfTriangles = 2*n*m;

    float verticalStepSize = radius / (m + 1);
    float height = radius - verticalStepSize;

    // fill vertices
    for (int i = 0; i < m; i++) {
        float circleRadius = sqrt( pow(radius, 2) - pow(height, 2) );
        makeCircle(circleRadius, height, &vertices[i + n]);
    }
    makePoles(radius, &vertices[numOfVertices - 2]);

    // triangulate
    int indices[numOfVertices];
    fillAscending(numOfVertices, indices);

    for (int i = 1; i < m; i++) {
        triangulateBarrel(&indices[(i-1)*n], &indices[(i)*n], &triangles[(i-1)*2*n]);
    }
    triangulateDome(indices[numOfVertices-2], indices, &triangles[numOfTriangles-2*n]);
    
    int bottomIndices[n];
    fillAscending(numOfVertices - 2 - n, n, bottomIndices);
    triangulateDome(indices[numOfVertices-1], bottomIndices, &triangles[numOfTriangles-1-n]);
}


#ifndef SETUP_RENDER_H
#define SETUP_RENDER_H

#include <vector>
#include "glm/glm.hpp"
#include "glm/gtx/compatibility.hpp"
#include "../../res/data/MarchingCubesData.h"

namespace Render
{
    /*
    * Port of alteredq's ThreeD version of marching cubes to Opengl
    * http://stemkoski.github.io/Three.js/Marching-Cubes.html
    */
    void MarchingCubes(const std::vector<glm::vec3>& points, 
                       const std::vector<float>& values,
                       std::vector<glm::vec3>& position,
                       std::vector<glm::vec3>& normal,
                       std::vector<unsigned int>& indices,
                       std::vector<glm::vec2>& cube_texcoords,
                       int size,float isolevel = 0                     
    )
    {
        int vertexIndex = 0;
        int size2 = size*size;
        std::vector<glm::vec3> vlist;
        vlist.reserve(12);

        glm::vec2 face[3]={
            // front
            glm::vec2(0.0, 0.0),
            glm::vec2(1.0, 0.0),
            glm::vec2(1.0, 1.0),
        };
        
        for (int z = 0; z < size - 1; z++)
        for (int y = 0; y < size - 1; y++)
        for (int x = 0; x < size - 1; x++)
        {
            // index of base point, and also adjacent points on cube
            float p    = x + size * y + size2 * z,
                px   = p   + 1,
                py   = p   + size,
                pxy  = py  + 1,
                pz   = p   + size2,
                pxz  = px  + size2,
                pyz  = py  + size2,
                pxyz = pxy + size2;

            // store scalar values corresponding to vertices
            float value0 = values[ p    ],
                value1 = values[ px   ],
                value2 = values[ py   ],
                value3 = values[ pxy  ],
                value4 = values[ pz   ],
                value5 = values[ pxz  ],
                value6 = values[ pyz  ],
                value7 = values[ pxyz ];

            // place a "1" in bit positions corresponding to vertices whose
            //   isovalue is less than given constant.
            
            float isolevel = 0;
            
            int cubeindex = 0;
            if ( value0 < isolevel ) cubeindex |= 1;
            if ( value1 < isolevel ) cubeindex |= 2;
            if ( value2 < isolevel ) cubeindex |= 8;
            if ( value3 < isolevel ) cubeindex |= 4;
            if ( value4 < isolevel ) cubeindex |= 16;
            if ( value5 < isolevel ) cubeindex |= 32;
            if ( value6 < isolevel ) cubeindex |= 128;
            if ( value7 < isolevel ) cubeindex |= 64;  

            // bits = 12 bit number, indicates which edges are crossed by the isosurface
            int bits = MC::edgeTable[ cubeindex ];  

            // if none are crossed, proceed to next iteration
            if ( bits == 0 ) continue;

            // check which edges are crossed, and estimate the point location
            //    using a weighted average of scalar values at edge endpoints.
            // store the vertex in an array for use later.
            float mu = 0.5; 

            // bottom of the cube
            if ( bits & 1 )
            {		
                mu = ( isolevel - value0 ) / ( value1 - value0 );
                vlist[0] = glm::lerp(points[p],points[px], mu );
            }
            if ( bits & 2 )
            {
                mu = ( isolevel - value1 ) / ( value3 - value1 );
                vlist[1] = glm::lerp(points[px],points[pxy], mu );
            }
            if ( bits & 4 )
            {
                mu = ( isolevel - value2 ) / ( value3 - value2 );
                vlist[2] = glm::lerp(points[py],points[pxy], mu );
            }
            if ( bits & 8 )
            {
                mu = ( isolevel - value0 ) / ( value2 - value0 );
                vlist[3] = glm::lerp(points[p],points[py], mu );
            }
            // top of the cube
            if ( bits & 16 )
            {
                mu = ( isolevel - value4 ) / ( value5 - value4 );
                vlist[4] = glm::lerp(points[pz],points[pxz], mu );
            }
            if ( bits & 32 )
            {
                mu = ( isolevel - value5 ) / ( value7 - value5 );
                vlist[5] = glm::lerp(points[pxz],points[pxyz], mu );
            }
            if ( bits & 64 )
            {
                mu = ( isolevel - value6 ) / ( value7 - value6 );
                vlist[6] = glm::lerp(points[pyz],points[pxyz], mu );
            }
            if ( bits & 128 )
            {
                mu = ( isolevel - value4 ) / ( value6 - value4 );
                vlist[7] = glm::lerp(points[pz],points[pyz], mu );
            }
            // vertical lines of the cube
            if ( bits & 256 )
            {
                mu = ( isolevel - value0 ) / ( value4 - value0 );
                vlist[8] = glm::lerp(points[p],points[pz], mu );
            }
            if ( bits & 512 )
            {
                mu = ( isolevel - value1 ) / ( value5 - value1 );
                vlist[9] = glm::lerp(points[px],points[pxz], mu );
            }
            if ( bits & 1024 )
            {
                mu = ( isolevel - value3 ) / ( value7 - value3 );
                vlist[10] = glm::lerp(points[pxy],points[pxyz], mu );
            }
            if ( bits & 2048 )
            {
                mu = ( isolevel - value2 ) / ( value6 - value2 );
                vlist[11] = glm::lerp(points[py],points[pyz], mu );
            }

            // construct triangles -- get correct vertices from triTable.
            int i = 0;
            cubeindex <<= 4;  // multiply by 16... 
            // "Re-purpose cubeindex into an offset into triTable." 
            //  since each row really isn't a row.
            
            // the while loop should run at most 5 times,
            //   since the 16th entry in each row is a -1.
            while ( MC::triTable[ cubeindex + i ] != -1 ) 
            {
                int index1 = MC::triTable[cubeindex + i];
                int index2 = MC::triTable[cubeindex + i + 1];
                int index3 = MC::triTable[cubeindex + i + 2];

                position.push_back(vlist[index1]);
                position.push_back(vlist[index2]);
                position.push_back(vlist[index3]);

                //Indices
                indices.push_back(vertexIndex);
                indices.push_back(vertexIndex+1);
                indices.push_back(vertexIndex+2);

                //texcord
                cube_texcoords.insert(cube_texcoords.end(), &face[0], &face[3]);

                glm::vec3 edge1 = vlist[index2] - vlist[index1];
                glm::vec3 edge2 = vlist[index3] - vlist[index1];

                normal.push_back((glm::cross(edge1, edge2)));

                vertexIndex+=3;
                i += 3;
            }
        }
    }
}
#endif //SETUP_RENDER_H
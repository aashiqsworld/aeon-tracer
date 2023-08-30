//
// Created by Aashiq Shaikh on 8/26/23.
//


#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "hittable.h"
#include "vec3.h"
#include "int3.h"
#include "common.h"

using namespace std;

struct triangle
{
    int3 vIndices;
    int nIndex;
    vec3* triVertices[3]; // pointers to positions in vertex array
    vec3 n;


    triangle() {
        vIndices = int3();
        nIndex = 0;
        triVertices[0] = nullptr;
        triVertices[1] = nullptr;
        triVertices[2] = nullptr;
        n = vec3();
    }

    friend inline ostream& operator<<(std::ostream &strm, const triangle &tri) {
        return strm << fixed << setprecision(1) << "Triangle: " << endl <<
            "v0(" << *tri.triVertices[0] << ")  v1(" << *tri.triVertices[1] << ")  v2(" << *tri.triVertices[2] << ")  " << endl <<
            "n(" << tri.n << ")" << endl;

    }

    static bool rayTriangleIntersection(const ray& r, const triangle& tri, interval ray_t, vec3& hit_pos)
    {

        vec3 vertex0 = *tri.triVertices[0];
        vec3 vertex1 = *tri.triVertices[1];
        vec3 vertex2 = *tri.triVertices[2];
        vec3 edge1, edge2, h, s, q;
        float a, f, u, v;
        edge1 = vertex1 - vertex0;
        edge2 = vertex2 - vertex0;
        h = cross(r.direction(),edge2);
        a = dot(edge1,h);

        if(a > -EPSILON && a < EPSILON)
            return false; // This ray is parallel to this triangle

        f = 1.0 / a;
        s = r.origin() - vertex0;
        u = f * dot(s,h);

        if(u < 0.0 || u > 1.0)
            return false;

        q = cross(s, edge1);
        v = f * dot(r.direction(),q);

        if(v < 0.0 || u + v > 1.0)
            return false;

        // At this stage we can compute t to find out where the intersection point is on the line

        float t = f * dot(edge2,q);

        if(t > EPSILON)
        {
            hit_pos = r.origin() + r.direction() * t;
            return true;
        }
        else // This means there is a line intersection but not a ray intersection
            return false;
    }
};

class mesh : public hittable
{
public:
    mesh() {}
    mesh(shared_ptr<material> m) : mat_ptr(m) { }

public:
    point3 pos;
    vector<vec3> vertices;
    vector<triangle> triangles;
    vector<vec3> vertexNormals;
    vector<int> faceNormals;
    shared_ptr<material> mat_ptr;


    virtual bool loadObjModel(char* filename);
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
    virtual void printObjModel();


private:
};

bool mesh::loadObjModel(char *filename) {
    string line;
    ifstream objFile (filename);
    if(objFile.is_open())
    {
        // process every line in the file
        while(getline(objFile, line))
        {
            string lineType;
            istringstream iss(line);

            iss >> lineType;
            float xf, yf, zf;
            string xs, ys, zs;

            if(lineType == "v")
            {
                auto tempVertex = vec3();
                iss >> tempVertex[0] >> yf >> zf;
                vertices.emplace_back(xf, yf, zf);
            }
            else if(lineType == "vn")
            {
                iss >> xf >> yf >> zf;
                vertexNormals.emplace_back(xf, yf, zf);
            }
            else if(lineType == "f")
            {
                auto tempFace = triangle();
                string faceValues[3];
                iss >> faceValues[0] >> faceValues[1] >> faceValues[2];


                // iterate through face line
                for(int j=0;j<3;j++)
                {
                    stringstream ss(faceValues[j]);
                    vector<string> result;

                    // iterate through 1 set of vertex properties of face
                    while( ss.good() )
                    {
                        string substr;
                        getline( ss, substr, '/' );
                        result.push_back( substr );
                    }

                    if(!result.empty() && !result[0].empty()) // vertex
                    {
                        tempFace.vIndices[j] = stoi(result[0]);
                    }
                    if (result.size() > 1 && !result[1].empty()) // texture coordinate
                    { }
                    if (result.size() > 2 && !result[2].empty()) // vertex normal
                    {
                        tempFace.nIndex = stoi(result[2]);
                    }
                }
                triangles.emplace_back(tempFace);
            }
        }
        objFile.close();

        // add pointers to relevant vertices for each triangle
        for(auto & t : triangles)
        {
            t.triVertices[0] = &vertices[t.vIndices[0]];
            t.triVertices[1] = &vertices[t.vIndices[1]];
            t.triVertices[2] = &vertices[t.vIndices[2]];
            t.n = vertexNormals[t.nIndex];
        }
    }
    else cout << "Unable to open file.";
    return true;
}

bool mesh::hit(const ray& r, interval ray_t, hit_record& rec) const
{
    vector<vec3> hits;
    for(auto & t : triangles)
    {
        vec3 temp_pos;
        if(triangle::rayTriangleIntersection(r, t, ray_t, temp_pos))
        {
            hits.emplace_back(temp_pos);
        }
    }

    if(hits.empty())
    {
        return false;
    }

    int max_index = 0;
    for(int i = 0; i < hits.size(); i++)
    {
        if(hits[i].length() > hits[max_index].length())
        {
            max_index = i;
        }
    }

    rec.p = hits[max_index];
    rec.mat_ptr = mat_ptr;

    return true;
}



void mesh::printObjModel() {
//    for(const auto & v : vertices) // print vertices
//    {
//        cout << fixed << setprecision(1) << "v  " <<
//             v.x() << "  " <<
//             v.y() << "  " <<
//             v.z() << "  " << endl;
//    }
//    cout << endl;
//    for(const auto & vn : vertexNormals) //print vertex normals
//    {
//        cout << fixed << setprecision(1) << "vn  " <<
//             vn.x() << "  " <<
//             vn.y() << "  " <<
//             vn.z() << "  " << endl;
//    }
//    cout << endl;
    for(const auto & f : triangles) // print face data
    {
        cout << f << endl;
    }
}

#endif

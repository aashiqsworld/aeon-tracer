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
#include "triangle.h"

using namespace std;



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

    vector<int3> vertexIndices;
    vector<int> normalIndices;

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
                auto tempVertexIndices = int3();
                auto tempNormalIndex = 0;
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
                        tempVertexIndices[j] = stoi(result[0]);
                    }
                    if (result.size() > 1 && !result[1].empty()) // texture coordinate
                    { }
                    if (result.size() > 2 && !result[2].empty()) // vertex normal
                    {
                        tempNormalIndex = stoi(result[2]);
                    }
                }
                vertexIndices.emplace_back(tempVertexIndices);
                normalIndices.emplace_back(tempNormalIndex);
            }
        }
        objFile.close();

        // add pointers to relevant vertices for each triangle
        int c = 0;
        for(int i = 0; i < vertexIndices.size(); i++)
        {
            auto t = triangle(vertices[vertexIndices[i].x()], vertices[vertexIndices[i].y()], vertices[vertexIndices[i].z()]);
            t.setNormal(vertexNormals[normalIndices[i]]);
            triangles.emplace_back(t);
        }
    }
    else cout << "Unable to open file.";
    return true;
}

bool mesh::hit(const ray& r, interval ray_t, hit_record& rec) const
{
    vector<hit_record> hits;
    for(auto & t : triangles)
    {
        hit_record temp_rec;
        if(t.hit(r, ray_t, temp_rec))
        {
            hits.emplace_back(temp_rec);
        }
    }

    if(hits.empty())
    {
        return false;
    }

    int max_index = 0;
    for(int i = 0; i < hits.size(); i++)
    {
        if(hits[i].p.length() > hits[max_index].p.length())
        {
            max_index = i;
        }
    }

    rec.p = hits[max_index].p;
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

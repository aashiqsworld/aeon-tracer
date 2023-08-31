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
#include "OBJ_Loader.h"

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
    virtual bool loadObjModelAlt(char* filename);
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
            if(!vertexNormals.empty())
                t.setNormal(vertexNormals[normalIndices[i]]);
            triangles.emplace_back(t);
        }
    }
    else cout << "Unable to open file.";
    return true;
}

bool mesh::loadObjModelAlt(char* filename)
{
    cout << "Loading " << filename << endl;

    objl::Loader Loader;

    bool loaded = Loader.LoadFile(filename);

    if(loaded)
    {
        // Create/Open e1Out.txt
        std::ofstream file("e1Out.txt");

        // Go through each loaded mesh and out its contents
        for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
        {
            // Copy one of the loaded meshes to be our current mesh
            objl::Mesh curMesh = Loader.LoadedMeshes[i];

            // Print Mesh Name
            cout << "Mesh " << i << ": " << curMesh.MeshName << "\n";

            // Print Vertices
            cout << "Vertices:\n";

            // Go through each vertex and print its number,
            //  position, normal, and texture coordinate
            for (int j = 0; j < curMesh.Vertices.size(); j++)
            {
                file << "V" << j << ": " <<
                     "P(" << curMesh.Vertices[j].Position.X << ", " << curMesh.Vertices[j].Position.Y << ", " << curMesh.Vertices[j].Position.Z << ") " <<
                     "N(" << curMesh.Vertices[j].Normal.X << ", " << curMesh.Vertices[j].Normal.Y << ", " << curMesh.Vertices[j].Normal.Z << ") " <<
                     "TC(" << curMesh.Vertices[j].TextureCoordinate.X << ", " << curMesh.Vertices[j].TextureCoordinate.Y << ")\n";
            }

            // Print Indices
            file << "Indices:\n";

            // Go through every 3rd index and print the
            //	triangle that these indices represent
            for (int j = 0; j < curMesh.Indices.size(); j += 3)
            {
                file << "T" << j / 3 << ": " << curMesh.Indices[j] << ", " << curMesh.Indices[j + 1] << ", " << curMesh.Indices[j + 2] << "\n";
            }

            // Print Material
            file << "Material: " << curMesh.MeshMaterial.name << "\n";
            file << "Ambient Color: " << curMesh.MeshMaterial.Ka.X << ", " << curMesh.MeshMaterial.Ka.Y << ", " << curMesh.MeshMaterial.Ka.Z << "\n";
            file << "Diffuse Color: " << curMesh.MeshMaterial.Kd.X << ", " << curMesh.MeshMaterial.Kd.Y << ", " << curMesh.MeshMaterial.Kd.Z << "\n";
            file << "Specular Color: " << curMesh.MeshMaterial.Ks.X << ", " << curMesh.MeshMaterial.Ks.Y << ", " << curMesh.MeshMaterial.Ks.Z << "\n";
            file << "Specular Exponent: " << curMesh.MeshMaterial.Ns << "\n";
            file << "Optical Density: " << curMesh.MeshMaterial.Ni << "\n";
            file << "Dissolve: " << curMesh.MeshMaterial.d << "\n";
            file << "Illumination: " << curMesh.MeshMaterial.illum << "\n";
            file << "Ambient Texture Map: " << curMesh.MeshMaterial.map_Ka << "\n";
            file << "Diffuse Texture Map: " << curMesh.MeshMaterial.map_Kd << "\n";
            file << "Specular Texture Map: " << curMesh.MeshMaterial.map_Ks << "\n";
            file << "Alpha Texture Map: " << curMesh.MeshMaterial.map_d << "\n";
            file << "Bump Map: " << curMesh.MeshMaterial.map_bump << "\n";

            // Leave a space to separate from the next mesh
            file << "\n";
        }
    }
    else
    {
        cout << "ERROR::Failed to load " << filename << endl;
        return false;
    }



    cout << "Loaded " << filename << endl;
    return true;
}


bool mesh::hit(const ray& r, interval ray_t, hit_record& rec) const
{
    vector<hit_record> hits;
    for(auto & t : triangles)
    {
        hit_record temp_rec;
        temp_rec.mat_ptr = rec.mat_ptr;
        if(t.hit(r, ray_t, temp_rec))
        {
            hits.emplace_back(temp_rec);
        }
    }

    if(hits.empty())
    {
        return false;
    }

    int min_index = 0;
    for(int i = 0; i < hits.size(); i++)
    {
        if(((hits[i].p - r.origin())).length() < (hits[min_index].p - r.origin()).length())
        {
            min_index = i;
        }
    }

    rec.p = hits[min_index].p;
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

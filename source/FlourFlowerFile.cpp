/** File: FlourFlowerFile.cpp
    Created on: 09-Sept-09
    Author: Robin Southern "betajaen"

    Copyright (c) 2009 Robin Southern

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#include "FlourFlowerFile.h"
#include "Flour.h"

#include "NxOgre.h"

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>

namespace N = NxOgre;

FlourFlowerFile::FlourFlowerFile()
: FlourFile(".flower", "Flower for meshes", ".nxs", FlourFile::FileType_Mesh)
{
}

FlourFlowerFile::~FlourFlowerFile()
{
}


NxOgre::MeshData* FlourFlowerFile::loadMesh(const std::string& path)
{
 NxOgre::MeshData* mesh = NxOgre_New(NxOgre::MeshData)();
 FlowerToMeshData(path, mesh);
 return mesh;
}

void  FlourFlowerFile::saveMesh(const std::string& path, NxOgre::MeshData* data)
{
 if (data == 0)
  return;
 
 MeshDataToFlower(path, data);
}

void FlourFlowerFile::getLine(NxOgre::Resource* resource, NxOgre::Buffer<char>& buffer)
{
 buffer.clear();
 while(1)
 {
  char c = resource->readChar();
  if (resource->atEnd() || c == '\r' || c == '\n')
   break;
  buffer.append(c);
 }
 buffer.append(0);
}

void FlourFlowerFile::FlowerToMeshData(const std::string& path, NxOgre::MeshData* mesh)
{
 
 N::ArchiveResourceIdentifier ari = Flour::getInstance()->getARI(Flour::getInstance()->getArchive(path), path);
 
 N::Resource* resource = N::ResourceSystem::getSingleton()->open(ari, N::Enums::ResourceAccess_ReadOnly);
 
 NxOgre::Buffer<char> buffer;
 unsigned int line = 0;
 while(1)
 {
  
  getLine(resource, buffer);
  line++;
  if (buffer.size() == 0) // Skip empty lines.
   continue;
  if (buffer[0] == '#') // Skip full comment lines.
   continue;
  
  // Cut off any inline-comments.
  for (unsigned int i=0; i < buffer.size();i++)
   if (buffer[i] == '#')
    buffer[i] = 0;
  
  if (boost::istarts_with(buffer.first(), "vertices"))
  {
    // vertices
    // 12345678
    std::string working_string(buffer.first() + 8);
    
    std::vector<std::string> float_strings;
    boost::algorithm::split(float_strings, working_string, boost::is_any_of(","));
    if (float_strings.size() % 3 != 0)
    {
     std::cout << "[Warning] Vertices count at line " << line << " is incorrect. Should be divisible by 3! Line is ignored." << std::endl;
     continue;
    }
    for (unsigned int i=0; i < float_strings.size(); i++)
    {
     boost::trim(float_strings[i]);
     mesh->mVertices.append(boost::lexical_cast<float, std::string>(float_strings[i]));
    }
    
    continue;
  }
  
  if (boost::istarts_with(buffer.first(), "indices") || boost::istarts_with(buffer.first(), "indexes"))
  {
   // indices indexes
   // 1234567 1234567
   std::string working_string(buffer.first() + 7);
   
   std::vector<std::string> uint_strings;
   boost::algorithm::split(uint_strings, working_string, boost::is_any_of(","));
   
   for (unsigned int i=0; i < uint_strings.size(); i++)
   {
    boost::trim(uint_strings[i]);
    unsigned int index = boost::lexical_cast<unsigned int, std::string>(uint_strings[i]);
    mesh->mIndexes.append(index);
   }
   
   continue;
  }
  
  if (boost::istarts_with(buffer.first(), "normals"))
  {
   
   // normals
   // 1234567
   std::string working_string(buffer.first() + 7);
   
   std::vector<std::string> float_strings;
   boost::algorithm::split(float_strings, working_string, boost::is_any_of(","));
   if (float_strings.size() % 3 != 0)
   {
    std::cout << "[Warning] Normal count at line " << line << " is incorrect. Should be divisible by 3!" << std::endl;
    continue;
   }
    for (unsigned int i=0; i < float_strings.size(); i++)
    {
     boost::trim(float_strings[i]);
     mesh->mNormals.append(boost::lexical_cast<float, std::string>(float_strings[i]));
    }

   continue;
  }
  
  if (boost::istarts_with(buffer.first(), "texturecoords"))
  {
   
   // texturecoords
   // 1234567890123
   std::string working_string(buffer.first() + 13);
   
   std::vector<std::string> float_strings;
   boost::algorithm::split(float_strings, working_string, boost::is_any_of(","));
   if (float_strings.size() % 2 != 0)
   {
    std::cout << "[Warning] Texture Coordinate count at line " << line << " is incorrect. Should be divisible by 3!" << std::endl;
    continue;
   }
   
   for (unsigned int i=0; i < float_strings.size(); i++)
   {
    boost::trim(float_strings[i]);
    mesh->mTextureCoordinates.append(boost::lexical_cast<float, std::string>(float_strings[i]));
   }
   
   continue;
  }
  
  if (resource->atEnd())
   break;
  
 } // end-while
  
  
}

void FlourFlowerFile::MeshDataToFlower(const std::string& path, NxOgre::MeshData* data)
{
 
 N::ArchiveResourceIdentifier ari = Flour::getInstance()->getARI(Flour::getInstance()->getArchive(path), path);
 
 N::Resource* resource = N::ResourceSystem::getSingleton()->open(ari, N::Enums::ResourceAccess_WriteOnly);
 
 
 NxOgre::SharedStringStream flower;
 
 // Write type;
 if (data->mType == NxOgre::Enums::MeshType_Convex)
  flower << "type convex\n";
 else if (data->mType == NxOgre::Enums::MeshType_Triangle)
  flower << "type triangle\n";
 else if (data->mType == NxOgre::Enums::MeshType_SoftBody)
  flower << "type softbody\n";
 else if (data->mType == NxOgre::Enums::MeshType_Cloth)
 {
  flower << "type cloth\n";
  flower << "weldingdistance " << data->mClothWeldingDistance << "\n";
 }
 
 // Vertices.
 if (data->mVertices.size())
  flower << "\n# " << data->mVertices.size() / 3 << " Vertices\n";
 for (unsigned int i=0; i < data->mVertices.size(); i+=3)
  flower << "vertices " << data->mVertices[i] << ", " << data->mVertices[i+1] << ", " << data->mVertices[i+2] << "\n";
 
 // Indices.
 if (data->mIndexes.size())
  flower << "\n# " << data->mIndexes.size() << " Indices / " << data->mIndexes.size() / 3 << " Triangles\n";
 for (unsigned int i=0; i < data->mIndexes.size(); i+=3)
  flower << "indexes " << data->mIndexes[i] << ", " << data->mIndexes[i+1] << ", " << data->mIndexes[i+2] << "\n";
 
 // Normals.
 if (data->mNormals.size())
  flower << "\n# " << data->mNormals.size() / 3 << " Normals\n";
 for (unsigned int i=0; i < data->mNormals.size(); i+=3)
  flower << "normals " << data->mNormals[i] << ", " << data->mNormals[i+1] << ", " << data->mNormals[i+2] << "\n";
 
 // TextureCoords.
 if (data->mTextureCoordinates.size())
  flower << "# " << data->mTextureCoordinates.size() / 2<< " Texture coordinates\n";
 for (unsigned int i=0; i < data->mNormals.size(); i+=2)
  flower << "texturecoords " << data->mTextureCoordinates[i] << ", " << data->mTextureCoordinates[i+1] << "\n";
 
 // Tetrahedra
 if (data->mTetrahedra.size())
  flower << "# " << data->mTetrahedra.size() / 4 << " Tetrahedra\n";
 for (unsigned int i=0; i < data->mTetrahedra.size(); i+=4)
  flower << "tetrahedra " << data->mTetrahedra[i] << ", " << data->mTetrahedra[i+1] << ", " <<  data->mTetrahedra[i+2] << ", " << data->mTetrahedra[i+3] << "\n";
 
 // Materials
 if (data->mMaterials.size())
  flower << "# " << data->mMaterials.size() << " Materials indices\n";
 for (unsigned int i=0; i < data->mMaterials.size(); i+=3)
  flower << "materials " << data->mMaterials[i] << ", " << data->mMaterials[i+1] << ", " << data->mMaterials[i+2] << "\n";
 
 // Flags
 if (data->mFlags.size())
  flower << "# " << data->mFlags.size() << " Vertex flags\n";
 for (unsigned int i=0; i < data->mFlags.size(); i+=3)
  flower << "vertexflags " << data->mFlags[i] << ", " << data->mFlags[i+1] << ", " << data->mFlags[i+2] << "\n";
 
 // Masses
 if (data->mMasses.size())
  flower << "# " << data->mMasses.size() << " Vertex masses\n";
 for (unsigned int i=0; i < data->mMasses.size(); i+=3)
  flower << "vertexmasses " << data->mMasses[i] << ", " << data->mMasses[i+1] << ", " << data->mMasses[i+2] << "\n";
 
 resource->write(flower.get(), flower.length());
 
 N::ResourceSystem::getSingleton()->close(resource);
}


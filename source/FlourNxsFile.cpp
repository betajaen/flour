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

#include "FlourNxsFile.h"
#include "Flour.h"

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <vector>

FlourNxsFile::FlourNxsFile()
: FlourFile(".nxs", "PhysX Shape File", ".flower", FlourFile::FileType_Mesh)
{
}

FlourNxsFile::~FlourNxsFile()
{
}


NxOgre::MeshData* FlourNxsFile::loadMesh(const std::string& path)
{
 open(path, false);
 
 std::stringstream ari;
 ari << mWorkingArchiveName << ":" << mWorkingFileName;

 NxOgre::Mesh* mesh = NxOgre::MeshManager::getSingleton()->load(NxOgre::ArchiveResourceIdentifier(ari.str().c_str()));
 
 NxOgre::MeshData* data = mesh->getMeshData();
 
 close();
 
 return data;
}

void  FlourNxsFile::saveMesh(const std::string& path, NxOgre::MeshData* data)
{
 if (data == 0)
  return;

 open(path, false);
 
 std::cout << data->mType << "\n";
 
 NxOgre::ManualMesh mm;
 mm.begin(data->mType, data->mVertices.size(), data->mIndexes.size());
 
 // Vertices
 for (unsigned int i=0;i < data->mVertices.size();i+=3)
  mm.vertex(data->mVertices[i], data->mVertices[i+1], data->mVertices[i+2]);
 
 // Indices
 for (unsigned int i=0;i < data->mIndexes.size();i++)
  mm.index(data->mIndexes[i]);
 
 // Normals
 for (unsigned int i=0;i < data->mNormals.size();i+=3)
  mm.normal(data->mNormals[i], data->mNormals[i+1], data->mNormals[i+2]);
 
 // TextureCoords
 for (unsigned int i=0;i < data->mTextureCoordinates.size();i+=2)
  mm.textureCoordinate(data->mTextureCoordinates[i], data->mTextureCoordinates[i+1]);
 
 //
 boost::filesystem::path pathname(path);
 std::string basename = pathname.filename();
 std::stringstream ari_stream;
 ari_stream << mWorkingArchiveName << ":" << basename;

 NxOgre::ArchiveResourceIdentifier ari(ari_stream.str().c_str());

 mm.endCookOnly(true, ari);

 close();
}

NxOgre::Mesh* FlourNxsFile::get(const std::string& path)
{
 
 NxOgre::Archive* archive = Flour::getInstance()->createOrGetArchive(path);
 
 boost::filesystem::path pathname(path);
 
 std::cout << pathname.filename().c_str() << std::endl;
 
 NxOgre::SharedStringStream stream;
 stream << archive->getName() << ":" << pathname.filename().c_str();
 return NxOgre::MeshManager::getSingleton()->load(NxOgre::ArchiveResourceIdentifier(stream.get()));
 
}


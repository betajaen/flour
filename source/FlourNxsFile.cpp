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

namespace N = NxOgre;

FlourNxsFile::FlourNxsFile()
: FlourFile(".nxs", "PhysX Shape File", ".flower", FlourFile::FileType_Mesh)
{
}

FlourNxsFile::~FlourNxsFile()
{
}

NxOgre::MeshData* FlourNxsFile::loadMesh(const std::string& path)
{
 
 // Compile ARI.
 N::ArchiveResourceIdentifier ari = Flour::getInstance()->getARI(Flour::getInstance()->getArchive(path), path);
 
 // Load Mesh using ARI.
 N::Mesh* mesh = NxOgre::MeshManager::getSingleton()->load(ari);
 
 // Mesh to MeshData
 N::MeshData* data = mesh->getMeshData();
 
 // Done. 
 return data;
}

void FlourNxsFile::saveMesh(const std::string& path, NxOgre::MeshData* data)
{
 
 // Don't do anything on bad meshdatas
 if (data == 0)
  return;
 
 // Work out the ARI.
 N::ArchiveResourceIdentifier ari = Flour::getInstance()->getARI(Flour::getInstance()->getArchive(path), path);
 
 // And cook it.
 data->cook(ari);
 
}

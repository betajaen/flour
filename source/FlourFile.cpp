/** File: FlourFile.cpp
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

#include "FlourFile.h"


#include <iostream>
#include <sstream>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

FlourFile::FlourFile(const std::string& extension, const std::string& description, const std::string& opposite, FileType ft)
: mExtension(extension), mDescription(description), mOpposite(opposite), mType(ft)
{
}

FlourFile::~FlourFile()
{
}


NxOgre::MeshData* FlourFile::loadMesh(const std::string& path)
{
 return 0;
}

void  FlourFile::saveMesh(const std::string& path, NxOgre::MeshData*)
{
}

NxOgre::ManualHeightField* FlourFile::loadHeightfield(const std::string& path)
{
 return 0;
}

void FlourFile::saveHeightfield(const std::string& path, NxOgre::HeightFieldData*)
{
}

NxOgre::Mesh* FlourFile::get(const std::string& path)
{
 return 0; // virtual
}

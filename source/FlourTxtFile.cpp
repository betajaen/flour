/** File: FlourTxtFile.cpp
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

#include "FlourTxtFile.h"

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>

FlourTxtFile::FlourTxtFile()
: FlourFile(".txt", "Flour text file for meshes", ".nxs", FlourFile::FileType_Mesh)
{
}

FlourTxtFile::~FlourTxtFile()
{
}


NxOgre::MeshData* FlourTxtFile::loadMesh(const std::string& path)
{
 open(path, true, NxOgre::Enums::ResourceAccess_ReadOnly);
 
 if (mWorkingResource->getStatus() != NxOgre::Enums::ResourceStatus_Opened)
 {
  close();
  return 0;
 }
 
 NxOgre::MeshData* mesh = NxOgre_New(NxOgre::MeshData)();
 
 NxOgre::Buffer<char> buffer;
 unsigned int line = 0;
 while(1)
 {
  getLine(buffer);
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
  
  if (mWorkingResource->atEnd())
   break;

 } // end-while
 
 close();
 return mesh;
 
}

void  FlourTxtFile::saveMesh(const std::string& path, NxOgre::MeshData* data)
{
 if (data == 0)
  return;

 open(path, true, NxOgre::Enums::ResourceAccess_WriteOnly);
 
 NxOgre::SharedStringStream line;
 
 // Write type;
 line << "type ";
 if (data->mType == NxOgre::Enums::MeshType_Convex)
  line << "convex";
 else if (data->mType == NxOgre::Enums::MeshType_Triangle)
  line << "triangle";
 else if (data->mType == NxOgre::Enums::MeshType_SoftBody)
  line << "softbody";
 else if (data->mType == NxOgre::Enums::MeshType_Cloth)
  line << "cloth";
 
 writeLine(line);
 
 // Vertices.
 if (data->mVertices.size())
 {
  std::stringstream ss;
  ss << "\n# " << data->mVertices.size() / 3 << " Vertices";
  line.clear();
  line.add(ss.str().c_str());
  writeLine(line);
 }
 for (unsigned int i=0; i < data->mVertices.size(); i+=3)
 {
  std::stringstream ss;
  ss << "vertices " << data->mVertices[i] << ", " << data->mVertices[i+1] << ", " << data->mVertices[i+2];
  line.clear();
  line.add(ss.str().c_str());
  writeLine(line);
 }
 
 // Indices.
 if (data->mIndexes.size())
 {
  std::stringstream ss;
  ss << "\n# " << data->mIndexes.size() << " Indices / " << data->mIndexes.size() / 3 << " Triangles";
  line.clear();
  line.add(ss.str().c_str());
  writeLine(line);
 }
 for (unsigned int i=0; i < data->mIndexes.size(); i+=3)
 {
  std::stringstream ss;
  ss << "indexes " << data->mIndexes[i] << ", " << data->mIndexes[i+1] << ", " << data->mIndexes[i+2];
  line.clear();
  line.add(ss.str().c_str());
  writeLine(line);
 }


 // Normals.
 if (data->mNormals.size())
 {
  std::stringstream ss;
  ss << "\n# " << data->mNormals.size() / 3 << " Normals";
  line.clear();
  line.add(ss.str().c_str());
  writeLine(line);
 }
 for (unsigned int i=0; i < data->mNormals.size(); i+=3)
 {
  std::stringstream ss;
  ss << "normals " << data->mNormals[i] << ", " << data->mNormals[i+1] << ", " << data->mNormals[i+2];
  line.clear();
  line.add(ss.str().c_str());
  writeLine(line);
 }



  
 // TextureCoords.
 if (data->mTextureCoordinates.size())
 {
  std::stringstream ss;
  ss << "# " << data->mTextureCoordinates.size() / 2<< " TextureCoords";
  line.clear();
  line.add(ss.str().c_str());
  writeLine(line);
 }
 for (unsigned int i=0; i < data->mNormals.size(); i+=2)
 {
  std::stringstream ss;
  ss << "texturecoords " << data->mTextureCoordinates[i] << ", " << data->mTextureCoordinates[i+1];
  line.clear();
  line.add(ss.str().c_str());
  writeLine(line);
 }
 
 close();
}

void FlourTxtFile::getLine(NxOgre::Buffer<char>& buffer)
{
 buffer.clear();
 while(1)
 {
  char c = mWorkingResource->readChar();
  if (mWorkingResource->atEnd() || c == '\r' || c == '\n')
   break;
  buffer.append(c);
 }
 buffer.append(0);
}

void FlourTxtFile::writeLine(NxOgre::SharedStringStream& line)
{
 mWorkingResource->write(line.get(), line.length());
 mWorkingResource->writeChar('\n');
 line.clear();
}

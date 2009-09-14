/** File: FlourToolTransform.cpp
    Created on: 12-Sept-09
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

#include "FlourToolTransform.h"
#include "Flour.h"
#include "FlourFile.h"

#include <iostream>

#include <NxOgre.h>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

FlourTransform::FlourTransform(TransformType type)
 : FlourTool("version", "Prints version information"),
   mTransformType(type)
{
 
 mOptionsDescription.add_options()
    ("help", "Show this")
    ("file,f", boost::program_options::value< std::vector<std::string> >(), "File(s) to process")
    ("x", boost::program_options::value<float>(), "X value")
    ("y", boost::program_options::value<float>(), "Y value")
    ("z", boost::program_options::value<float>(), "Z value")
 ;
 
 mPositionalOptions.add("file", -1);
 
 add_error(ERROR_NoFile, "Need at least one file.");
 add_error(ERROR_NoMeshData, "File contains no mesh data.");
 add_error(ERROR_UnrecongisedFileformat, "Unrecongised file format.");
 
}

FlourTransform::~FlourTransform()
{
}

void FlourTransform::process()
{

 // Need at least one file.
 if (mVariablesMap.count("file") == 0)
 {
  error(ERROR_NoFile, true);
  return;
 }
 
 float transform_x = 0.0f,
       transform_y = 0.0f,
       transform_z = 0.0f;
 
 if (mVariablesMap.count("x"))
  transform_x = mVariablesMap["x"].as<float>();
 
 if (mVariablesMap.count("y"))
  transform_y = mVariablesMap["y"].as<float>();
 
 if (mVariablesMap.count("z"))
  transform_z = mVariablesMap["z"].as<float>();
 
 std::cout << "XYZ:" << transform_x << " " <<  transform_y << " " <<  transform_z << "\n";
 
 Flour::getInstance()->initNxOgre();
 
 std::vector<std::string> files = mVariablesMap["file"].as<std::vector<std::string>>();
 
 for (unsigned int i=0; i < files.size(); i++)
 {
  
  std::string file = files[i];
  boost::filesystem::path pathname(file);
  std::string extension = pathname.extension();
  
  FlourFile* in_file = Flour::getInstance()->getFile(extension);
  if (in_file == 0)
  {
   error(ERROR_UnrecongisedFileformat, true);
   return;
  }
  
  NxOgre::MeshData* mesh = in_file->loadMesh(file);
  if (mesh == 0)
  {
   error(ERROR_NoMeshData, true);
   return;
  }
  
  if (mTransformType == TransformType_Move)
  {
   for (unsigned int i=0; i < mesh->mVertices.size(); i+=3)
   {
    mesh->mVertices[i]   += transform_x;
    mesh->mVertices[i+1] += transform_y;
    mesh->mVertices[i+2] += transform_z;
   }
  }
  else if (mTransformType == TransformType_Rotate)
  {
  }
  else if (mTransformType == TransformType_Scale)
  {
   
   if (transform_x <= 0.0f)
    transform_x = 1.0f;
   
   if (transform_y <= 0.0f)
    transform_y = 1.0f;
   
   if (transform_z <= 0.0f)
    transform_z = 1.0f;
   
   for (unsigned int i=0; i < mesh->mVertices.size(); i+=3)
   {
    mesh->mVertices[i]   *= transform_x;
    mesh->mVertices[i+1] *= transform_y;
    mesh->mVertices[i+2] *= transform_z;
   }
  }
  
  in_file->saveMesh(file, mesh);
  
  if (mTransformType == TransformType_Move)
  {
   std::cout << "Transform " << file << " by " << transform_x << ", " << transform_y << ", " << transform_z << std::endl;
  }
  else if (mTransformType == TransformType_Rotate)
  {
  }
  else if (mTransformType == TransformType_Scale)
  {
  }
  
 }
}

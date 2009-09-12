/** File: FlourToolConvert.cpp
    Created on: 06-Sept-09
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

#include "FlourToolConvert.h"
#include "Flour.h"
#include "FlourFile.h"

#include <NxOgre.h>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

std::string get_description(FlourConvert::ConversionType type)
{
 if (type == FlourConvert::ConversionType_Convex)
  return std::string("Convert a mesh into a PhysX Convex mesh");
 else if (type == FlourConvert::ConversionType_Triangle)
  return std::string("Convert a mesh into a PhysX Triangle mesh");
 else if (type == FlourConvert::ConversionType_Cloth)
  return std::string("Convert a mesh into a PhysX Cloth mesh or a NxOgre Extended Cloth mesh");
 else if (type == FlourConvert::ConversionType_Heightfield)
  return std::string("Convert a RAW image into NxOgre Heightfield");
 else
  return std::string("I have no idea");
}

std::string get_name(FlourConvert::ConversionType type)
{
 if (type == FlourConvert::ConversionType_Convex)
  return std::string("convex");
 else if (type == FlourConvert::ConversionType_Triangle)
  return std::string("triangle");
 else if (type == FlourConvert::ConversionType_Cloth)
  return std::string("cloth");
 else if (type == FlourConvert::ConversionType_Heightfield)
  return std::string("heightfield");
 else
  return std::string("king_donald_of_the_starfish");
}

FlourConvert::FlourConvert(ConversionType type)
: FlourTool(get_name(type), get_description(type)), mConversionType(type)
{
 
 add_error(ERROR_NoFile, "Need at least one file.");
 add_error(ERROR_NoMeshData, "File contains no mesh data.");
 add_error(ERROR_UnrecongisedFileformat, "Unrecongised file format.");

 if (mConversionType == ConversionType_Triangle)
 {
  mOptionsDescription.add_options()
     ("help", "Show this")
     ("flip_normals", "Flip the normals of the mesh")
     ("16bit_indices", "Use 16-bit indices")
     ("hardware", "Make this a hardware mesh")
     ("file,f", boost::program_options::value< std::vector<std::string> >(), "File(s) to process")
  ;
  
  mPositionalOptions.add("file", -1);
 }
 else if (mConversionType == ConversionType_Convex)
 {
  mOptionsDescription.add_options()
     ("help", "Show this")
     ("flip_normals", "Flip the normals of the mesh")
     ("16bit_indices", "Use 16-bit indices")
     ("compute_convex", "Automatically recomputes the hull from the vertices. If this flag is not set, you must provide the entire geometry manually.")
     ("inflate_convex", "Inflates the convex object according to skin width. Use with --compute_convex only.")
     ("uncompress_normals", "Instructs cooking to save normals uncompressed. The cooked hull data will be larger, but will load faster.")

     ("file,f", boost::program_options::value< std::vector<std::string> >(), "File(s) to process")
  ;
  
  mPositionalOptions.add("file", -1);
 }
}

FlourConvert::~FlourConvert()
{
}

void FlourConvert::process()
{
 
 // Need at least one file.
 if (mVariablesMap.count("file") == 0)
 {
  error(ERROR_NoFile, true);
  return;
 }
 
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

  if (mConversionType == ConversionType_Convex)
   mesh->mType = NxOgre::Enums::MeshType_Convex;
  else if (mConversionType == ConversionType_Triangle)
   mesh->mType = NxOgre::Enums::MeshType_Triangle;
  else if (mConversionType == ConversionType_Cloth)
   mesh->mType = NxOgre::Enums::MeshType_Cloth;
  else
   mesh->mType = NxOgre::Enums::MeshType_Unknown;
  
  std::string opposite = in_file->getOpposite();
  
  std::string out_filename = pathname.filename();
  boost::replace_last(out_filename, extension, opposite);
  
  FlourFile* out_file = Flour::getInstance()->getFile(opposite);
  
  out_file->saveMesh(boost::filesystem::path(pathname.parent_path() / out_filename).string(), mesh);
  
  std::cout << "Saved " << file << " as " << out_filename << std::endl;

  NxOgre_Delete(mesh);
  
 }
  
}

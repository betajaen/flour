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

#include <iostream>
#include <fstream>
#include <iterator>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include <NxOgre.h>

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(std::cout, "\n")); 
    return os;
}

void read_line(NxOgre::Resource* resource, NxOgre::Buffer<char> buffer)
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
 
 std::vector<std::string> files = mVariablesMap["file"].as<std::vector<std::string>>();
 
 for (unsigned int i=0; i < files.size(); i++)
 {
  if (mConversionType == ConversionType_Convex)
   convertConvex(files[i]);
  else if (mConversionType == ConversionType_Triangle)
   convertTriangle(files[i]);
  else if (mConversionType == ConversionType_Heightfield)
   convertHeightfield(files[i]);
  else if (mConversionType == ConversionType_Cloth)
   convertCloth(files[i]);
 }
}


void FlourConvert::convertConvex(const std::string& file)
{

 Flour::getInstance()->initNxOgre();

 NxOgre::String out_ari("internal-cwd:flour_out.nxs"); 
 SimpleMesh mesh;

 if (boost::iends_with(file, ".txt"))
  parseTextFile(file, mesh);


 if (mesh.Vertices.size())
 {
  NxOgre::ManualMesh mm;
  mm.begin(NxOgre::Enums::MeshType_Convex, mesh.Vertices.size());
  
  // Vertices
  for (unsigned int i=0;i < mesh.Vertices.size();i++)
   mm.vertex(mesh.Vertices[i]);

  // Indices
  if (mesh.Indices.size())
  {
   for (unsigned int i=0;i < mesh.Indices.size();i++)
    mm.index(mesh.Indices[i]);
   
  }
  mm.endCookOnly(true, NxOgre::ArchiveResourceIdentifier(out_ari.c_str()));
 }
 else
 {
  error(1001, true);
  return;
 }

/*
 NxOgre::ManualMesh mesh;
 mesh.begin(NxOgre::Enums::MeshType_Convex, 12);
 mesh.vertex(1,1,1);
 mesh.vertex(1,0,1);
 mesh.vertex(3,1,1);
 mesh.vertex(1,1,3);
 mesh.vertex(1,4,1);
 mesh.vertex(1,-4,1);
 mesh.endCookOnly(true, NxOgre::ArchiveResourceIdentifier("internal-cwd:test.nxs"));
*/
}

void FlourConvert::convertTriangle(const std::string&)
{
 std::cout << "triangle" << std::endl;
}

void FlourConvert::convertHeightfield(const std::string&)
{
 std::cout << "hf" << std::endl;
}

void FlourConvert::convertCloth(const std::string&)
{
 std::cout << "cloth" << std::endl;
}

void FlourConvert::parseTextFile(const std::string& file, SimpleMesh& mesh)
{
 boost::filesystem::path pathname(file);
    std::string dirname  = pathname.parent_path().string();
    std::string basename = pathname.filename();
 
 
 if (dirname.size() == 0)
  dirname = ".";

 NxOgre::SharedStringStream string_stream;
 string_stream << "file:" << dirname.c_str();

 NxOgre::Archive* archive = NxOgre::ResourceSystem::getSingleton()->openArchive(
      "flour_textfile_working", NxOgre::UniformResourceIdentifier(string_stream));
 
 
 string_stream.clear();
 string_stream << "flour_textfile_working:" << basename.c_str();
 
 NxOgre::Resource* resource = NxOgre::ResourceSystem::getSingleton()->open(NxOgre::ArchiveResourceIdentifier(string_stream), NxOgre::Enums::ResourceAccess_ReadOnly);
 
 if (resource->getStatus() == NxOgre::Enums::ResourceStatus_Opened)
 {
  NxOgre::Buffer<char> buffer;
  unsigned int line = 0;
  while(1)
  {
   read_line(resource, buffer);
   //std::cout << buffer.first() << std::endl;
   line++;
   if (buffer.size() == 0)
    continue;
   
   if (buffer[0] == '#') // Skip full-line comments.
    continue;
   
   // "Trim" down the string if it has comments. Basically adding a null-terminator where the comment starts.
   for (unsigned int i=0; i < buffer.size(); i++)
   {
    if (buffer[i] == '#')
     buffer[i] = 0;
   }

   if (boost::istarts_with(buffer.first(), "vertices"))
   {
    
    // vertices
    // 12345678
    std::string working_string(buffer.first() + 8);
    
    std::vector<std::string> float_strings;
    boost::algorithm::split(float_strings, working_string, boost::is_any_of(","));
    if (float_strings.size() % 3 != 0)
    {
     std::cout << "[Warning] Vertices count at line " << line << " is incorrect. Should be divisible by 3!" << std::endl;
     continue;
    }
    for (unsigned int i=0; i < float_strings.size(); i+=3)
    {
     NxOgre::Vec3 vec;
     boost::trim(float_strings[i]);
     boost::trim(float_strings[i+1]);
     boost::trim(float_strings[i+2]);
     vec.x = boost::lexical_cast<float, std::string>(float_strings[i]);
     vec.y = boost::lexical_cast<float, std::string>(float_strings[i+1]);
     vec.z = boost::lexical_cast<float, std::string>(float_strings[i+2]);
     mesh.Vertices.insert(vec);
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
     mesh.Indices.insert(index);
    }
    
    continue;
   }

   if (resource->atEnd())
    break;
  }
  
 }
 else
 {
  error(ERROR_NoFile, true);
  return;
 }

 archive->close(resource);
 
 
 // Close the archive.
 NxOgre::ResourceSystem::getSingleton()->closeArchive("flour_textfile_working");
}

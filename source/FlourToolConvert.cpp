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

void read_line(NxOgre::Resource* resource, NxOgre::SharedStringStream stream)
{
 while(false)
 {
  std::cout << resource->readChar();
  if (resource->at() == resource->getSize())
   return;
 }
/*
 line_length=0;
 while (resource->atEnd() == false) // || line_length < 2048)
 {
  line[line_length] = resource->readChar();

  std::cout << line[line_length];// << resource->atEnd();

  if (line[line_length] == '\n')
   break;

  line_length++;
 }
 line[line_length + 1] = 0;*/
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
   for (unsigned int i=0;i < mesh.Indices.size();i++)
    mm.index(mesh.Indices[i]);

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

void FlourConvert::parseTextFile(const std::string& file, SimpleMesh&)
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
  //unsigned int i = resource->getSize();
  std::cout << "At:" << resource->at() << std::endl;
  std::cout << "End:" << resource->atEnd() << std::endl;
  resource->seek(170 + 1);
  std::cout << "At:" << resource->at() << std::endl;
  std::cout << "End:" << resource->atEnd() << std::endl;


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

/** \brief Convert
*/

#ifndef FLOUR_TOOL_CONVERT_H
#define FLOUR_TOOL_CONVERT_H

#include "FlourTool.h"
#include "NxOgre.h"

struct SimpleMesh
{
 NxOgre::Array<NxOgre::Vec3>  Vertices;
 NxOgre::Array<NxOgre::Vec3>  Normals;
 NxOgre::Array<unsigned int>  Indices;
 NxOgre::Array<NxOgre::Vec2>  TextureCoords;
 NxOgre::Array<NxOgre::Vec4>  Tetrahedra;
};

class FlourConvert : public FlourTool
{
 public:
  
  enum Errors
  {
   ERROR_NoFile = 1000,
   ERROR_NoMeshData = 1001,
  };
  
  enum ConversionType
  {
   ConversionType_Convex,
   ConversionType_Triangle,
   ConversionType_Cloth,
   ConversionType_Heightfield,
  };

  FlourConvert(ConversionType);
  
 ~FlourConvert();
  
  void process();
  
  void convertConvex(const std::string&);
  
  void convertTriangle(const std::string&);
  
  void convertHeightfield(const std::string&);
  
  void convertCloth(const std::string&);
  
  void parseTextFile(const std::string& file, SimpleMesh&);

 protected:
  
  ConversionType mConversionType;
  
};

#endif
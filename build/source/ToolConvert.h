/** 
    
    This file is part of Flour.
    
    Copyright (c) 2010 Robin Southern, http://www.nxogre.org
    
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

#include "Flour.h"

namespace NxOgre
{



class ToolConvert : public Tool
{
  
 public:
  
  bool process(Args& args)
  {
   
   if (args.flags.has("help") || args.flags.has("h"))
   {
    puts << "Converts .flower meshes files into NxOgre or PhysX meshes\n";
    puts << "Typical Usage:\n > flour convert myMesh.flower\n";
    puts << "Forcing a specific type:\n > flour convert --type=cloth myMesh.flower\n\n";
    return false;
   }

   if (args.files.size() == 0)
   {
    puts << "Need at least one file to convert to\n";
    return false;
   }
   
   Flour::getSingleton()->getWorld();
   NxOgre::Enums::MeshType meshType = NxOgre::Enums::MeshType_Unknown;
   
   if (args.properties.has("type"))
   {
    std::string type = args.properties.at("type");
    NxOgre::Strings::lower(type);
    if (type == "triangle" || type == "mesh")
     meshType = NxOgre::Enums::MeshType_Triangle;
    else if (type == "convex")
     meshType = NxOgre::Enums::MeshType_Convex;
    else if (type == "cloth")
     meshType = NxOgre::Enums::MeshType_Cloth;
    else if (type == "skeleton")
     meshType = NxOgre::Enums::MeshType_Skeleton;
    else
    {
     puts << "Unknown type '" << args.properties.at("type") << "'. I only understand triangle/mesh, convex, cloth or skeleton.\n";
     return false; 
    }
   }

   
   for (NxOgre::vector_iterator<std::string> it = args.files.elements(); it != it.end(); it++)
   {
    std::string path;
    path.append((*it));
    
    NxOgre::Resource* flower = NxOgre::ResourceSystem::getSingleton()->open("file://" + path);
    NxOgre::MeshData* meshData = NxOgre::GC::safe_new0<NxOgre::MeshData>(NXOGRE_GC_THIS);
    NxOgre::Serialisation::Flower::load(meshData, flower);
    
    NxOgre::Strings::slice_to_last_of(path, '.'); // Remove extension.
    
    if (meshType != NxOgre::Enums::MeshType_Unknown)
     meshData->mType = meshType;
     
    if (meshData->mType == NxOgre::Enums::MeshType_Cloth)
     path.append(".xm");
    else if (meshData->mType == NxOgre::Enums::MeshType_Convex)
     path.append(".nxs");
    else if (meshData->mType == NxOgre::Enums::MeshType_Skeleton)
     path.append(".xm");
    else if (meshData->mType == NxOgre::Enums::MeshType_SoftBody)
     path.append(".xm");
    else if (meshData->mType == NxOgre::Enums::MeshType_Triangle)
     path.append(".nxs");
    else
    {
     puts << "Unknown mesh type given in the flower file. To override or choose your own do: \n";
     puts << "  flour convert --type=convex " << (*it) << "\n";
     return false;
    }
    
    meshData->cook("file://" + path);
    puts << ">> Converted " << (*it) << " into " << path << "\n";
    
   }

   return true;
  }
  
  
}; // class
} // namespace.


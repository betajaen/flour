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



class ToolDisassemble : public Tool
{
  
 public:
  
  bool process(Args& args)
  {
   
   if (args.flags.has("help") || args.flags.has("h"))
   {
    puts << "Converts a PhysX or NxOgre mesh into a flower file\n";
    puts << "Typical Usage:\n > flour disassemble myMesh.nxs\n";
    return false;
   }

   if (args.files.size() == 0)
   {
    puts << "Need at least one file to convert to\n";
    return false;
   }
   
   Flour::getSingleton()->getWorld();
   NxOgre::Enums::MeshType meshType = NxOgre::Enums::MeshType_Unknown;
    
   for (NxOgre::vector_iterator<std::string> it = args.files.elements(); it != it.end(); it++)
   {
    std::string path;
    path.append((*it));
    
    NxOgre::Mesh* mesh = NxOgre::MeshManager::getSingleton()->load("file://" + path);
    NxOgre::MeshData* meshData = mesh->getMeshData();
    
    NxOgre::Strings::slice_to_last_of(path, '.'); // Remove extension.
    path.append(".flower"); // Add ".flower"
    
    NxOgre::Resource* flower = NxOgre::ResourceSystem::getSingleton()->open("file://" + path, NxOgre::Enums::ResourceAccess_WriteOnly);
    NxOgre::Serialisation::Flower::save(meshData, flower);
    
    puts << ">> Dissassembled " << (*it) << " into " << path << "\n";
    
   }

   return true;
  }
  
  
}; // class
} // namespace.


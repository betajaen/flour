/** File: FlourTxtFile.h
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

#ifndef FLOUR_TXT_FILE_H
#define FLOUR_TXT_FILE_H

#include "FlourFile.h"

#include "NxOgre.h"
#include <string>

class FlourTxtFile : public FlourFile
{
  
 public:
  
                                      FlourTxtFile();
  
                                     ~FlourTxtFile();
  
  NxOgre::MeshData*                   loadMesh(const std::string& path);
  
  void                                saveMesh(const std::string& path, NxOgre::MeshData*);
  
  NxOgre::ManualHeightField*          loadHeightfield(const std::string& path);
  
  void                                saveHeightfield(const std::string& path, NxOgre::HeightFieldData*);
  
};

#endif
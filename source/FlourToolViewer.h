/** File: FlourToolViewer.h
    Created on: 10-Sept-09
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

#ifndef FLOUR_TOOL_VIEWER_H
#define FLOUR_TOOL_VIEWER_H

#include "FlourTool.h"
#include "FlourOpenGL.h"

#include "NxOgre.h"

class FlourViewer : public FlourTool, public OpenGL
{
 public:
  
  enum Errors
  {
   ERROR_NoFile = 1000,
   ERROR_NoMeshData,
   ERROR_UnrecongisedFileformat
  };
  
  FlourViewer();
  
 ~FlourViewer();
  
  void        process();
  
  void        onPostFrame();
  
  void        onKeyEvent(char key);
  
  void        onMouseDragEvent(int ButtonID, int deltaX, int deltaY);
  
  void        onMouseButtonEvent(int ButtonID, int x, int y);
  
 protected:
  
  float mYaw, mPitch, mDistance;
  
};

#endif
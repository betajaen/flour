/** File: FlourOpenGL.h
    Created on: 11-Sept-09
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

#ifndef FLOUR_OPENGL_H
#define FLOUR_OPENGL_H

#include "FlourTool.h"
#include "NxOgre.h"

#include <AntTweakBar.h>

struct                 OpenGL_Triangle
{
 NxOgre::Vec3  a,  b, c, normal;
 
 void                          calculateNormal();
};

struct Body
{
  
  NxOgre::Actor*                mActor;
  
  NxOgre::SceneGeometry*        mSceneGeometry;
  
  NxOgre::Matrix44              mPose;
  
  NxOgre::Mesh*                 mMesh;
  
  unsigned int                  mID;
  
  float                         mRed, mGreen, mBlue;
  
  std::vector<OpenGL_Triangle>  mTriangles;
  
  void                          draw();
  
  inline void                   drawTriangle();
  
  void                          parse(NxOgre::MeshData*);
  
};

/////////////////////////////////////////////////////////////////////////

void TW_CALL callQuitTW(void*);

/////////////////////////////////////////////////////////////////////////

class OpenGL
{
  
 public:
  
#ifdef _WIN32
  enum WindowIcon
  {
   WindowIcon_NxOgreCube = 100,
   WindowIcon_Cake       = 101,
   WindowIcon_Brick      = 102,
  };
#else
  enum WindowIcon
  {
   WindowIcon_NxOgreCube = 0,
   WindowIcon_Cake       = 0,
   WindowIcon_Brick      = 0,
  };
#endif
  OpenGL();
  
 ~OpenGL();
  
  void                createWindow(const std::string& window_caption, unsigned int width = 1024, unsigned int height = 768, WindowIcon = WindowIcon_NxOgreCube);
  
  void                createScene(const NxOgre::Vec3& gravity = NxOgre::Vec3(0.0f,-9.8f,0.0f), bool useHardware = false, bool groundPlane = true);
  
  NxOgre::Mesh*       createMesh(const std::string& mesh_path);

  Body*               createBody(const std::string& mesh_path, float mass, const NxOgre::Matrix44& = Matrix44_Identity);
  
  void                startRendering();
  
  void                resetBodies();
  
  void                orbitCamera(float& yaw, float& pitch, float& distance);
  
  void                renderOnce();
  
  void                drawGrid();
  
  virtual void        onPreFrame();
  
  virtual void        onPostFrame();
  
  virtual void        onKeyEvent(char key);
  
  virtual void        onMouseDragEvent(int ButtonID, int deltaX, int deltaY);
  
  virtual void        onMouseButtonEvent(int ButtonID, int x, int y);

  void                onKeyCallback(char key);
  
  NxOgre::Actor*      getSelectedActor();
  
 protected:
  
  NxOgre::Vec3        mCamera;
  
  NxOgre::Vec3        mCameraTarget;
  
  NxOgre::World*      mWorld;
  
  NxOgre::Scene*      mScene;
  
  std::vector<Body*>  mBodies;
  
  unsigned int        mNextCreateOrder;
  
  int                 mWindowHandle;
  
  std::vector<unsigned int> mColours;
  
  unsigned int        mNextColour;
  
  unsigned int        mWorkingBodyIndex;
  
  TwBar*              mBar;
  
  unsigned            mNextBodiesID;
};

#endif
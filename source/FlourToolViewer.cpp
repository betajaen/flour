/** File: FlourToolFiles.cpp
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

#include "FlourToolViewer.h"
#include "Flour.h"
#include "FlourFile.h"

#include <iostream>

#include <NxOgre.h>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <windows.h>								// Header File For Windows
#include <GL/glut.h>

void RenderFunc();
void IdleFunc();
void ExitFunc();
void KeyboardCallback(unsigned char key, int x, int y);
void renderWireFrameCubeGrid(int k);
void MouseCallback(int gButton, int state, int x, int y);
void MotionCallback(int x, int y);
void updateCam();

FlourViewer* gFlourViewer = 0;
int          gMouseX = 0;
int          gMouseY = 0;
bool         gButtonDown = false;
bool         gButtonJustDown = false;
int          gButton = 0;
bool         gWireframe = true;
bool         gVertexColours = true;

FlourViewer::FlourViewer()
: FlourTool("viewer", "Shows a 3D representation of a mesh"), mCameraPitch(0.33f), mCameraYaw(0.33f), mCameraDistance(2.0f)
{
 
 add_error(ERROR_NoFile, "Need at least one file.");
 add_error(ERROR_NoMeshData, "File contains no mesh data.");
 add_error(ERROR_UnrecongisedFileformat, "Unrecongised file format.");
 
 mOptionsDescription.add_options()
     ("file,f", boost::program_options::value< std::vector<std::string> >(), "File(s) to process")
     ("wireframe", "Add wireframe reference")
  ;

 mPositionalOptions.add("file", -1);
}

FlourViewer::~FlourViewer()
{
}

void FlourViewer::process()
{
 gFlourViewer = this;
 
 Flour::getInstance()->initNxOgre();
 mWorld = NxOgre::World::getWorld();

 // Need at least one file.
 if (mVariablesMap.count("file") == 0)
 {
  error(ERROR_NoFile, true);
  return;
 }

 if (mVariablesMap.count("wireframe"))
  gWireframe = true;
 
 Flour::getInstance()->initNxOgre();
 
 std::vector<std::string> files = mVariablesMap["file"].as<std::vector<std::string>>();
 
 std::stringstream window_stream;
 window_stream << "Flour Viewer - ";

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
   mMeshes.push_back(mesh);

  window_stream << pathname.filename();
  
  if (i == files.size() - 2)
   window_stream << " and ";
  else if (i == files.size() - 1)
   ;
  else
   window_stream << ", ";
  i++;

 }

 
 createWindow(window_stream.str());
 
 glutMainLoop();
}

void FlourViewer::createWindow(const std::string& caption)
{
 glutInitWindowSize(1024, 768);
 glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
 mWindowHandle = glutCreateWindow(caption.c_str());
 glutSetWindow(mWindowHandle);
 glutDisplayFunc(RenderFunc);
 glutIdleFunc(IdleFunc);
 glutKeyboardFunc(KeyboardCallback);
 glutMouseFunc(MouseCallback);
 glutMotionFunc(MotionCallback);
 
 atexit(ExitFunc);
 
 glClearColor(0.1337f, 0.1337f, 0.1337f, 1.0);
 glEnable(GL_DEPTH_TEST);
 glEnable(GL_COLOR_MATERIAL);
 
 updateCam();
 
 mCameraTarget.x = 0;
 mCameraTarget.y = 0;
 mCameraTarget.z = 0;
 
}


void FlourViewer::renderFrame()
{
 
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 
 // Setup projection matrix
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 gluPerspective(60.0f, (float)glutGet(GLUT_WINDOW_WIDTH)/(float)glutGet(GLUT_WINDOW_HEIGHT), 0.1f, 1000.0f);
 gluLookAt(mCamera.x, mCamera.y, mCamera.z, mCameraTarget.x, mCameraTarget.y, mCameraTarget.z, 0.0f, 1.0f, 0.0f);

  // Setup modelview matrix
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 
 ///////////////////////////////////////////////////////////////
 
 renderWireFrameCubeGrid(40);
 
 ///////////////////////////////////////////////////////////////
 
 for (std::vector<NxOgre::MeshData*>::iterator it = mMeshes.begin(); it != mMeshes.end(); it++)
 {
  NxOgre::MeshData* m = (*it);
  
  glPushMatrix();

  glBegin(GL_TRIANGLES);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  for (unsigned int i=0; i < m->mIndexes.size();i++)
  {
   unsigned int index = m->mIndexes[i];
   NxOgre::Vec3 vec;
   vec.x = m->mVertices[(index * 3)];
   vec.y = m->mVertices[(index * 3) + 1];
   vec.z = m->mVertices[(index * 3) + 2];

   if (gVertexColours)
   {
    NxOgre::Vec3 col = vec;
    col.normalise();
    glColor3f(col.x, col.y, col.z);
   }
   else
   {
    glColor3f(1.0f, 1.0f, 1.0f);
   }

   glVertex3f(vec.x, vec.y, vec.z);
  }
  glEnd();
  glPopMatrix();

  if (gWireframe)
  {
    // glDisable(GL_DEPTH_TEST);
    glPushMatrix();
    glBegin(GL_LINES);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
    
    for (unsigned int i=0; i < m->mIndexes.size();i++)
    {
     unsigned int index = m->mIndexes[i];
     NxOgre::Vec3 vec;
     vec.x = m->mVertices[(index * 3)];
     vec.y = m->mVertices[(index * 3) + 1];
     vec.z = m->mVertices[(index * 3) + 2];
     glColor3f(0,0,0);
     glVertex3f(vec.x, vec.y, vec.z);
    }
    glEnd();
    glPopMatrix();
    // glEnable(GL_DEPTH_TEST);
  }

 }
 
 ///////////////////////////////////////////////////////////////
 
 glutSwapBuffers();

}

void RenderFunc()
{
 gFlourViewer->renderFrame();
}

void IdleFunc()
{
 glutPostRedisplay();
}

void ExitFunc()
{
 delete Flour::getInstance();
}

void KeyboardCallback(unsigned char key, int x, int y)
{
 if (key == 27)
 {
  exit(0);
  return;
 }
 
 if (key == 'w')
  gFlourViewer->mCameraDistance -= 0.05f;
 else if (key == 's')
  gFlourViewer->mCameraDistance += 0.05f;
 
 if (gFlourViewer->mCameraDistance < 0.25f)
  gFlourViewer->mCameraDistance = 0.25f;

 if (key == 'a')
  gFlourViewer->mCameraYaw += 0.05f;
 else if (key == 'd')
  gFlourViewer->mCameraYaw -= 0.05f;
 
 if (key == 'q')
  gFlourViewer->mCameraPitch += 0.05f;
 else if (key == 'z')
  gFlourViewer->mCameraPitch -= 0.05f;
 
 updateCam();
 
}

void MouseCallback(int b, int state, int x, int y)
{
 
 if (b == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
 {
  gButtonDown     = true;
  gButtonJustDown = true;
  gButton = GLUT_LEFT_BUTTON;
 }
 else if (b == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
 {
  gButtonDown     = true;
  gButtonJustDown = true;
  gButton = GLUT_RIGHT_BUTTON;
 }
 else
 {
  gButtonDown = false;
 }
 
}

void MotionCallback(int x, int y)
{
  
  if (gButtonDown && gButtonJustDown)
  {
   gButtonJustDown = false;
   gMouseX = x;
   gMouseY = y;
   return;
  }
  
  if (gButtonDown && gButton == GLUT_LEFT_BUTTON)
  {
   int dx = gMouseX - x;
   int dy = gMouseY - y;
   
   gFlourViewer->mCameraYaw += ((float(dx) * 0.005f));
   gFlourViewer->mCameraPitch += ((float(dy) * -0.005f)); 
   
   updateCam();
   
   gMouseX = x;
   gMouseY = y;
  }
  
  if (gButtonDown && gButton == GLUT_RIGHT_BUTTON)
  {
   int dx = gMouseX - x;
   int dy = gMouseY - y;
   
   gFlourViewer->mCameraDistance += ((float(dy) * 0.005f));
   
   updateCam();
   
   gMouseX = x;
   gMouseY = y;
  }
}

void renderWireFrameCubeGrid(int k)
{
  glPushMatrix();
  
  int halfK = k / 2;
  
  glBegin(GL_LINES);
  
  for(int i=0;i<=k;i++)
  {
   
   if (i==halfK)
    glColor3f(.6,.3,.3);
   else
    glColor3f(.2,.2,.2);
   
   glVertex3f(-halfK + i,0,-halfK);
   glVertex3f(-halfK + i,0,halfK);
   
   if (i==halfK)
    glColor3f(.3,.3,.6);
   else
    glColor3f(.2,.2,.2);
   
   glVertex3f(-halfK,0,-halfK + i);
   glVertex3f(halfK,0,-halfK + i);
   
  }
  
  glEnd();
  glPopMatrix();
}


void updateCam()
{
 gFlourViewer->mCamera.x = sin(gFlourViewer->mCameraYaw) * gFlourViewer->mCameraDistance;
 gFlourViewer->mCamera.z = cos(gFlourViewer->mCameraYaw) * gFlourViewer->mCameraDistance;
 gFlourViewer->mCamera.y = tan(gFlourViewer->mCameraPitch)  * gFlourViewer->mCameraDistance;
}
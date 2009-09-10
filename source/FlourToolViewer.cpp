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

FlourViewer* viewerInst = 0;

FlourViewer::FlourViewer() : FlourTool("viewer", "Shows a 3D representation of a mesh")
{
 add_error(ERROR_NoFile, "Need at least one file.");
 add_error(ERROR_NoMeshData, "File contains no mesh data.");
 add_error(ERROR_UnrecongisedFileformat, "Unrecongised file format.");

 mOptionsDescription.add_options()
     ("file,f", boost::program_options::value< std::vector<std::string> >(), "File(s) to process")
  ;
  
  mPositionalOptions.add("file", -1);
}

FlourViewer::~FlourViewer()
{
}

void FlourViewer::process()
{
 viewerInst = this;
 
 Flour::getInstance()->initNxOgre();
 mWorld = NxOgre::World::getWorld();


 // Need at least one file.
 if (mVariablesMap.count("file") == 0)
 {
  error(ERROR_NoFile, true);
  return;
 }
 
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
 glutInitWindowSize(512, 512);
 glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
 mWindowHandle = glutCreateWindow(caption.c_str());
 glutSetWindow(mWindowHandle);
 glutDisplayFunc(RenderFunc);
 glutIdleFunc(IdleFunc);
 atexit(ExitFunc);
 
 glClearColor(0.3f, 0.4f, 0.5f, 1.0);
 glEnable(GL_DEPTH_TEST);
 glEnable(GL_COLOR_MATERIAL);
 
 // Setup lighting
 // Setup lighting
 glEnable(GL_LIGHTING);
 float ambientColor[]  = { 0.0f, 0.1f, 0.2f, 0.0f };
 float diffuseColor[]  = { 1.0f, 1.0f, 1.0f, 0.0f };
 float specularColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
 float position[]      = { 100.0f, 100.0f, 400.0f, 1.0f };
 glLightfv(GL_LIGHT0, GL_AMBIENT, ambientColor);
 glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseColor);
 glLightfv(GL_LIGHT0, GL_SPECULAR, specularColor);
 glLightfv(GL_LIGHT0, GL_POSITION, position);
 glEnable(GL_LIGHT0);

 mCamera.x = 5;
 mCamera.y = 5;
 mCamera.z = 5;
 
 mCameraTarget.x = 0;
 mCameraTarget.y = 0;
 mCameraTarget.z = 0;

 mRotAngle = 0;
}


void FlourViewer::renderFrame()
{
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

 // Setup projection matrix
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 gluPerspective(60.0f, (float)glutGet(GLUT_WINDOW_WIDTH)/(float)glutGet(GLUT_WINDOW_HEIGHT), 1.0f, 10000.0f);
 gluLookAt(mCamera.x, mCamera.y, mCamera.z, mCameraTarget.x, mCameraTarget.y, mCameraTarget.z, 0.0f, 1.0f, 0.0f);

 // Setup modelview matrix
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();


 for (std::vector<NxOgre::MeshData*>::iterator it = mMeshes.begin(); it != mMeshes.end(); it++)
 {
  NxOgre::MeshData* m = (*it);
  

  glPushMatrix();
  glRotatef(mRotAngle,0.0f,1.0f,0.0f);			// Rotate The Cube On X, Y & Z
  mRotAngle += 0.5f;

  glBegin(GL_TRIANGLES);
  for (unsigned int i=0; i < m->mIndexes.size();i++)
  {
   unsigned int index = m->mIndexes[i];
   NxOgre::Vec3 vec;
   vec.x = m->mVertices[(index * 3)];
   vec.y = m->mVertices[(index * 3) + 1];
   vec.z = m->mVertices[(index * 3) + 2];
   NxOgre::Vec3 col = vec;
   col.normalise();
   glColor3f(col.x, col.y, col.z);
   glVertex3f(vec.x, vec.y, vec.z);
  }
  glEnd();
  glPopMatrix();
 }
 
 
 glutSwapBuffers();
}

void RenderFunc()
{
 viewerInst->renderFrame();
}

void IdleFunc()
{
 glutPostRedisplay();
}

void ExitFunc()
{
 delete Flour::getInstance();
}

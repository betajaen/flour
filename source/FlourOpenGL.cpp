/** File: FlourOpenGL.cpp
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

#include "FlourOpenGL.h"
#include "Flour.h"
#include "FlourFlowerFile.h"

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include <windows.h>
#include <GL/glut.h>

#include <NxOgre.h>
#include <AntTweakBar.h>
namespace N = NxOgre;
namespace E = NxOgre::Enums;

OpenGL*      gOpenGL_WorkingTool          = 0;
unsigned int gOpenGL_LastMousePositionX   = 0;
unsigned int gOpenGL_LastMousePositionY   = 0;
unsigned int gOpenGL_LastMouseButton      = 0;
bool         gOpenGL_MouseButtonDown      = false;
bool         gOpenGL_MouseButtonJustDown  = false;

void TW_CALL callQuitTW(void*)
{
 
 TwTerminate();
 delete Flour::getInstance();
}

void OpenGL_RenderCallback()
{
 gOpenGL_WorkingTool->renderOnce();
}

void OpenGL_IdleCallback()
{
 glutPostRedisplay();
}

void OpenGL_ExitCallback()
{
 callQuitTW(0);
}

void OpenGL_KeyboardCallback(unsigned char key, int x, int y)
{
 
 if (TwEventKeyboardGLUT(key, x, y))
  return;
 
 gOpenGL_WorkingTool->onKeyCallback(key);
 gOpenGL_WorkingTool->onKeyEvent(key);
}

void OpenGL_MouseCallback(int button, int state, int x, int y)
{
 
 if (TwEventMouseButtonGLUT(button, state, x, y))
  return;

 gOpenGL_MouseButtonDown = (state == GLUT_DOWN);
 
 if (state == GLUT_DOWN)
 {
  gOpenGL_LastMouseButton = button;
  gOpenGL_LastMousePositionX = x;
  gOpenGL_LastMousePositionY = y;
  gOpenGL_MouseButtonJustDown = true;
  gOpenGL_WorkingTool->onMouseButtonEvent(gOpenGL_LastMouseButton = button, x, y);
 }
 
}

void OpenGL_MotionCallback(int x, int y)
{
 
 if (TwEventMouseMotionGLUT(x,y))
  return;
 
 if (gOpenGL_MouseButtonDown && gOpenGL_MouseButtonJustDown)
 {
  gOpenGL_MouseButtonJustDown = false;
  gOpenGL_LastMousePositionX = x;
  gOpenGL_LastMousePositionY = y;
 }
 
 if (gOpenGL_MouseButtonDown)
 {
   
   int deltaX = gOpenGL_LastMousePositionX - x;
   int deltaY = gOpenGL_LastMousePositionY - y;
   
   gOpenGL_WorkingTool->onMouseDragEvent(gOpenGL_LastMouseButton, deltaX, deltaY);
   
   gOpenGL_LastMousePositionX = x;
   gOpenGL_LastMousePositionY = y;
   
 }
 
}

void OpenGL_PassiveMotionCallback(int x, int y)
{
 
 if (TwEventMouseMotionGLUT(x,y))
  return;
 
 if (gOpenGL_MouseButtonDown && gOpenGL_MouseButtonJustDown)
 {
  gOpenGL_MouseButtonJustDown = false;
  gOpenGL_LastMousePositionX = x;
  gOpenGL_LastMousePositionY = y;
 }
 
 if (gOpenGL_MouseButtonDown)
 {
   
   int deltaX = gOpenGL_LastMousePositionX - x;
   int deltaY = gOpenGL_LastMousePositionY - y;
   
   gOpenGL_WorkingTool->onMouseDragEvent(gOpenGL_LastMouseButton, deltaX, deltaY);
   
   gOpenGL_LastMousePositionX = x;
   gOpenGL_LastMousePositionY = y;
   
 }
 
}


void getColour(unsigned col, float &r, float &g, float &b)
{
 b = float( col & 0xFF)        / 255.0f;
 g = float((col >> 8) & 0xFF)  / 255.0f;
 r = float((col >> 16) & 0xFF) / 255.0f;
}



OpenGL::OpenGL()
: mWorld(0),
  mScene(0),
  mNextColour(0),
  mNextBodiesID(0)
{

mColours.push_back(0xffefd5);
mColours.push_back(0xd2691e);
mColours.push_back(0xfff0f5);
mColours.push_back(0xffdab9);
mColours.push_back(0xff7f50);
mColours.push_back(0x7cfc00);
mColours.push_back(0xf0f8ff);
mColours.push_back(0xdcdcdc);
mColours.push_back(0xfffaf0);
mColours.push_back(0xc71585);
mColours.push_back(0xf5f5f5);
mColours.push_back(0x228b22);
mColours.push_back(0xffe4b5);
mColours.push_back(0x00ffff);
mColours.push_back(0xffd700);
mColours.push_back(0xffdead);
mColours.push_back(0x7fffd4);
mColours.push_back(0xdaa520);
mColours.push_back(0x191970);
mColours.push_back(0xffff00);
mColours.push_back(0xff00ff);
mColours.push_back(0xf5fffa);
mColours.push_back(0xfaebd7);
mColours.push_back(0xf8f8ff);
mColours.push_back(0x000080);
mColours.push_back(0xf0ffff);
mColours.push_back(0x808080);
mColours.push_back(0xfdf5e6);
mColours.push_back(0xf5f5dc);
mColours.push_back(0x008000);
mColours.push_back(0x808000);
mColours.push_back(0xffe4e1);
mColours.push_back(0xee82ee);
mColours.push_back(0x1e90ff);
mColours.push_back(0x00fa9a);
mColours.push_back(0xf5deb3);
mColours.push_back(0xb22222);
mColours.push_back(0x48d1cc);
mColours.push_back(0xffffff);
mColours.push_back(0xffe4c4);
mColours.push_back(0xadff2f);
mColours.push_back(0x6b8e23);
mColours.push_back(0x000000);
mColours.push_back(0xff4500);
mColours.push_back(0x0000ff);
mColours.push_back(0xff69b4);
mColours.push_back(0xda70d6);
mColours.push_back(0x8a2be2);
mColours.push_back(0xcd5c5c);
mColours.push_back(0xeee8aa);
mColours.push_back(0xa52a2a);
mColours.push_back(0x4b0082);
mColours.push_back(0x98fb98);
mColours.push_back(0xa0522d);
mColours.push_back(0xff8c00);
mColours.push_back(0x778899);
mColours.push_back(0xc0c0c0);
mColours.push_back(0x9932cc);
mColours.push_back(0xb0c4de);
mColours.push_back(0x87ceeb);
mColours.push_back(0x8b0000);
mColours.push_back(0xffffe0);
mColours.push_back(0x808080);
mColours.push_back(0x5f9ea0);
mColours.push_back(0xf0e68c);
mColours.push_back(0xdb7093);
mColours.push_back(0x7fff00);
mColours.push_back(0xdeb887);
mColours.push_back(0xfffff0);
mColours.push_back(0xafeeee);
mColours.push_back(0xfff8dc);
mColours.push_back(0xadd8e6);
mColours.push_back(0x778899);
mColours.push_back(0xe6e6fa);
mColours.push_back(0xcd853f);
mColours.push_back(0x6495ed);
mColours.push_back(0xfffacd);
mColours.push_back(0xffc0cb);
mColours.push_back(0xffa500);
mColours.push_back(0xffebcd);
mColours.push_back(0xf0fff0);
mColours.push_back(0x6a5acd);
mColours.push_back(0xe9967a);
mColours.push_back(0x00ff00);
mColours.push_back(0x708090);
mColours.push_back(0x8fbc8f);
mColours.push_back(0x32cd32);
mColours.push_back(0xdda0dd);
mColours.push_back(0xdc143c);
mColours.push_back(0xf08080);
mColours.push_back(0xb0e0e6);
mColours.push_back(0x00ffff);
mColours.push_back(0xe0ffff);
mColours.push_back(0x800080);
mColours.push_back(0x00008b);
mColours.push_back(0xfafad2);
mColours.push_back(0xff0000);
mColours.push_back(0x008b8b);
mColours.push_back(0xd3d3d3);
mColours.push_back(0x006400);
mColours.push_back(0xffb6c1);
mColours.push_back(0xfa8072);
mColours.push_back(0xbc8f8f);
mColours.push_back(0xb8860b);
mColours.push_back(0xfffafa);
mColours.push_back(0x2f4f4f);
mColours.push_back(0xff00ff);
mColours.push_back(0x00ff7f);
mColours.push_back(0x2f4f4f);
mColours.push_back(0x800000);
mColours.push_back(0x4682b4);
mColours.push_back(0x00ced1);
mColours.push_back(0x66cdaa);
mColours.push_back(0xd2b48c);
mColours.push_back(0x9400d3);
mColours.push_back(0x90ee90);
mColours.push_back(0x4169e1);
mColours.push_back(0xa9a9a9);
mColours.push_back(0xd3d3d3);
mColours.push_back(0x8b4513);
mColours.push_back(0xa9a9a9);
mColours.push_back(0xffa07a);
mColours.push_back(0xf4a460);
mColours.push_back(0xbdb76b);
mColours.push_back(0x20b2aa);
mColours.push_back(0x2e8b57);
mColours.push_back(0x8b008b);
mColours.push_back(0x87cefa);
mColours.push_back(0xfff5ee);
mColours.push_back(0x556b2f);
mColours.push_back(0x708090);
mColours.push_back(0x483d8b);
mColours.push_back(0xfaf0e6);
mColours.push_back(0x0000cd);
mColours.push_back(0x008080);
mColours.push_back(0xff1493);
mColours.push_back(0xba55d3);
mColours.push_back(0xd8bfd8);
mColours.push_back(0x00bfff);
mColours.push_back(0x9370db);
mColours.push_back(0xff6347);
mColours.push_back(0x696969);
mColours.push_back(0x3cb371);
mColours.push_back(0x40e0d0);
mColours.push_back(0x696969);
mColours.push_back(0x7b68ee);
mColours.push_back(0x9acd32);

 Flour::getInstance()->initNxOgre();
 mWorld = N::World::getWorld();
}

OpenGL::~OpenGL()
{
 
 if (mScene)
  mWorld->destroyScene(mScene);
 
 // Delete Bodies and Scene here.
 if (mBodies.size())
 {
  for (unsigned int i=0;i < mBodies.size();i++)
   delete mBodies[i];
 }
 
}

void OpenGL::createWindow(const std::string& window_caption, unsigned int width, unsigned int height, WindowIcon icon)
{
 glutInitWindowSize(width, height);
 glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
 mWindowHandle = glutCreateWindow(window_caption.c_str());
 glutSetWindow(mWindowHandle);
 glutDisplayFunc(OpenGL_RenderCallback);
 glutIdleFunc(OpenGL_IdleCallback);
 glutKeyboardFunc(OpenGL_KeyboardCallback);
 glutMouseFunc(OpenGL_MouseCallback);
 glutMotionFunc(OpenGL_MotionCallback);
 glutPassiveMotionFunc(OpenGL_PassiveMotionCallback);
 atexit(OpenGL_ExitCallback);
 
 glClearColor(0.1337f, 0.1337f, 0.1337f, 0.1337f);
 glClearDepth(1.0f);

 glEnable(GL_DEPTH_TEST);
 glDepthFunc(GL_LEQUAL);

 glShadeModel(GL_SMOOTH);

 glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

 // Enable light and set up 2 light sources (GL_LIGHT0 and GL_LIGHT1)
 glEnable(GL_LIGHTING);
 glEnable(GL_LIGHT0);
 glEnable(GL_LIGHT1);

 // We're setting up two light sources. One of them is located
 // on the left side of the model (x = -1.5f) and emits white light. The
 // second light source is located on the right side of the model (x = 1.5f)
 // emitting red light.

 // GL_LIGHT0: the white light emitting light source
 // Create light components for GL_LIGHT0
 float ambientLight0[] = { 0.2f, 0.2f, 0.2f, 1.0f };
 float diffuseLight0[] = { 0.8f, 0.8f, 0.8f, 1.0f };
 float specularLight0[] = { 0.5f, 0.5f, 0.5f, 1.0f };
 float position0[] = { -25, 15, -25, 1.0f };	
 // Assign created components to GL_LIGHT0
 glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
 glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
 glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight0);
 glLightfv(GL_LIGHT0, GL_POSITION, position0);

 mCamera = NxOgre::Vec3::ZERO;
 mCameraTarget = NxOgre::Vec3::ZERO;
 
#ifdef _WIN32
 HWND wnd = WindowFromDC(wglGetCurrentDC());
 HINSTANCE hInst = (HINSTANCE)GetModuleHandle(NULL);
 SetClassLong ( wnd , GCL_HICON, (LONG)LoadIcon (hInst, MAKEINTRESOURCE (icon)));
#else
 // ...
#endif
 
 TwInit(TW_OPENGL, NULL);
 TwWindowSize(width, height);
 
 mBar = TwNewBar("Flour");
 
 TwDefine(" Flour color='32 32 32' alpha=64 text=light ");
 TwDefine(" GLOBAL fontsize=2 ");
 TwDefine(" Flour refresh=0.016 ");
 TwDefine(" Flour iconifiable=false ");
 TwDefine(" GLOBAL fontresizable=false ");
}

void OpenGL::createScene(const N::Vec3& gravity, bool useHardware, bool groundPlane)
{
 N::SceneDescription scene_d;
 scene_d.mUseHardware = useHardware;
 scene_d.mGravity = gravity;
 
 mScene = mWorld->createScene(scene_d);

 mScene->getMaterial(0)->setStaticFriction(0.5);
 mScene->getMaterial(0)->setDynamicFriction(0.5);

 if (groundPlane)
  mScene->createSceneGeometry(new N::PlaneGeometry(0, N::Vec3(0, 1, 0)));
 
}

N::Mesh* OpenGL::createMesh(const std::string &path)
{
 
 boost::filesystem::path mesh_path(path);
 
 if (mesh_path.extension() == ".flower") // Cook the file internally.
  return Flour::getInstance()->getFile(".flower")->get(path);
 else if (mesh_path.extension() == ".nxs")
  return Flour::getInstance()->getFile(".nxs")->get(path);
 else
  return 0;
 
}

Body* OpenGL::createBody(const std::string& mesh_path, float mass, const N::Matrix44& pose)
{
 
 N::Mesh* mesh = createMesh(mesh_path);
 if (mesh == 0)
  return 0;
 
 Body* body = new Body();
 body->mActor = 0;
 body->mSceneGeometry = 0;
 body->mPose = pose;
 body->mMesh = mesh;
 
 N::RigidBodyDescription rbody_d;
 rbody_d.mMass = mass;

 N::Shape* shape = 0;
 if (mesh->getType() == E::MeshType_Convex)
 {
  shape = new N::Convex(mesh);
 }
 else if (mesh->getType() == E::MeshType_Triangle)
 {
  shape = new N::TriangleGeometry(mesh);
  rbody_d.mMass = 0;
 }
 
 if (shape == 0)
  return 0;
 
 if (rbody_d.mMass)
  body->mActor = mScene->createActor(shape, pose, rbody_d);
 else
  body->mSceneGeometry = mScene->createSceneGeometry(shape, pose, rbody_d);
 
 N::MeshData* mesh_data = body->mMesh->getMeshData();
 body->parse(mesh_data);
 delete mesh_data;

 int colour = mColours[mNextColour++];

 if (mNextColour == mColours.size())
  mNextColour = 0;

 body->mID = mNextBodiesID++;
 
 getColour(colour, body->mRed, body->mGreen, body->mBlue);
 
 mBodies.push_back(body);
 return body;
}

void OpenGL::startRendering()
{
 gOpenGL_WorkingTool = this;
 mWorkingBodyIndex = 0;
 glutMainLoop();
}

void OpenGL::renderOnce()
{
 
 onPreFrame();
 
 //////////////////////////////////////////////////////////////
 
 N::TimeController::getSingleton()->advance();
 
 //////////////////////////////////////////////////////////////
 
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 gluPerspective(60.0f, (float)glutGet(GLUT_WINDOW_WIDTH)/(float)glutGet(GLUT_WINDOW_HEIGHT), 0.1f, 1000.0f);
 
 N::Vec3 origin;
 if (mWorkingBodyIndex > mBodies.size())
  mWorkingBodyIndex = 0;
 
 if (mBodies.size())
 {
  Body* body = mBodies[mWorkingBodyIndex];
  if (body->mActor)
   origin = body->mActor->getGlobalPosition();
  else if (body->mSceneGeometry)
   origin = body->mSceneGeometry->getGlobalPosition();
 }
 else
  origin = N::Vec3::ZERO;
 
 gluLookAt(origin.x + mCamera.x, origin.y + mCamera.y, origin.z + mCamera.z, origin.x + mCameraTarget.x, origin.y + mCameraTarget.y, origin.z + mCameraTarget.z, 0.0f, 1.0f, 0.0f);
 
 
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 
 //////////////////////////////////////////////////////////////
 
 glDisable(GL_LIGHTING);
 drawGrid();
 glEnable(GL_LIGHTING);
 
 //////////////////////////////////////////////////////////////
 
 for (unsigned int i=0;i < mBodies.size();i++)
 {
  mBodies[i]->draw();
 }
 
 //////////////////////////////////////////////////////////////
 
 onPostFrame();
 
 //////////////////////////////////////////////////////////////
 
 TwDraw();
 
 //////////////////////////////////////////////////////////////
 
 glutSwapBuffers();
}

void OpenGL::onPreFrame()
{
 // virtual
}

void OpenGL::onPostFrame()
{
 // virtual
}

void OpenGL::resetBodies()
{
 for (unsigned int i=0;i < mBodies.size();i++)
 {
  if (mBodies[i]->mActor)
  {
   mBodies[i]->mActor->setLinearMomentum(N::Vec3(0,0,0));
   mBodies[i]->mActor->setAngularMomentum(N::Vec3(0,0,0));
   mBodies[i]->mActor->setGlobalPose(mBodies[i]->mPose);
  }
 }
}

N::Actor* OpenGL::getSelectedActor()
{
 if (mBodies.size() == 0)
  return 0;
 if (mWorkingBodyIndex > mBodies.size())
  return 0;
 Body* body = mBodies[mWorkingBodyIndex];
 if (body->mSceneGeometry)
  return 0;
 return body->mActor;
}


void OpenGL::orbitCamera(float& yaw, float& pitch, float& distance)
{
 if (yaw < -bml::Pi)
  yaw = bml::Pi;
 else if (yaw > bml::Pi)
  yaw = -bml::Pi;

 if (pitch < 0)
  pitch = 0;
 else if (pitch > 1.2f)
  pitch = 1.2f;

 mCamera.x = sin(yaw) * distance;
 mCamera.y = tan(pitch) * distance;
 mCamera.z = cos(yaw) * distance;
}

void OpenGL::drawGrid()
{
 glPushMatrix();
 glBegin(GL_LINES);
 
 for(int i=0;i<=50;i++)
 {
  
  if (i==25)
   glColor3f(.3,.3,.6);
  else
   glColor3f(.2,.2,.2);
  
  glVertex3f(-25 + i,0,-25);
  glVertex3f(-25 + i,0,25);
  
  if (i==25)
   glColor3f(.6,.3,.3);
  else
   glColor3f(.2,.2,.2);
  
  glVertex3f(-25,0,-25 + i);
  glVertex3f(25,0,-25 + i);
  
 }

 glColor3f(.3,.6,.3);
 glVertex3f(0,0,0);
 glColor3f(.3,.6,.3);
 glVertex3f(0,1,0);
 
 
 glEnd();
 glPopMatrix();
}

void OpenGL::onKeyEvent(char key)
{
 // virtual
}

void OpenGL::onMouseDragEvent(int ButtonID, int deltaX, int deltaY)
{
 // virtual
}

void OpenGL::onMouseButtonEvent(int ButtonID, int x, int y)
{
 // virtual
}


void OpenGL::onKeyCallback(char key)
{
 if (key == 27)
 {
  exit(0);
  return;
 }

 if (key == '[' || key == ']')
 {
  
  if (mBodies.size() == 0 || mWorkingBodyIndex > mBodies.size() - 1)
  {
   mWorkingBodyIndex = 0;
   return;
  }
  
  if (key == '[')
  {
   if (mWorkingBodyIndex == 0)
    mWorkingBodyIndex = mBodies.size() - 1;
   else
    mWorkingBodyIndex--;
  }
  else
  {
   if (mWorkingBodyIndex == mBodies.size() - 1)
    mWorkingBodyIndex = 0;
   else 
    mWorkingBodyIndex++;
  }
  
 }
}

void OpenGL_Triangle::calculateNormal()
{
 
 N::Vec3 n, m;
 
 n.x = a.x - b.x;
 n.y = a.y - b.y;
 n.z = a.z - b.z;
 
 m.x = b.x - c.x;
 m.y = b.y - c.y;
 m.z = b.z - c.z;
 
 normal.x = (n.y * m.z) - (n.z * m.y);
 normal.y = (n.z * m.x) - (n.x * m.z);
 normal.z = (n.x * m.y) - (n.y * m.x);
 
 normal.normalise();
 
}

void Body::parse(N::MeshData* mesh_data)
{
 
 unsigned int index_a = 0, index_b = 0, index_c = 0;
 N::Vec3      vertex_a, vertex_b, vertex_c;

 for (unsigned int i=0; i < mesh_data->mIndexes.size();i+=3)
 {
   
   index_a = mesh_data->mIndexes[i];
   index_b = mesh_data->mIndexes[i+1];
   index_c = mesh_data->mIndexes[i+2];
   
   OpenGL_Triangle triangle;
   
   triangle.a.x = mesh_data->mVertices[(index_a * 3)];
   triangle.a.y = mesh_data->mVertices[(index_a * 3) + 1];
   triangle.a.z = mesh_data->mVertices[(index_a * 3) + 2];
   
   triangle.b.x = mesh_data->mVertices[(index_b * 3)];
   triangle.b.y = mesh_data->mVertices[(index_b * 3) + 1];
   triangle.b.z = mesh_data->mVertices[(index_b * 3) + 2];
   
   triangle.c.x = mesh_data->mVertices[(index_c * 3)];
   triangle.c.y = mesh_data->mVertices[(index_c * 3) + 1];
   triangle.c.z = mesh_data->mVertices[(index_c * 3) + 2];
   
   triangle.calculateNormal();
   
   mTriangles.push_back(triangle);
  }
}

void Body::draw()
{
 glLoadIdentity();
 glPushMatrix();
    float glMat[16];
    
    if (mActor)
     mActor->getGlobalPose().columnMajor(glMat);
    else if (mSceneGeometry)
     mSceneGeometry->getGlobalPose().columnMajor(glMat);
    
    glMultMatrixf(glMat);  
    glBegin(GL_TRIANGLES);

    float no_mat[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
    float mat_ambient_color[] = {mRed, mGreen, mBlue, 1.0f};
    float mat_diffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
    float mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float no_shininess = 0.0f;
    float low_shininess = 5.0f;
    float high_shininess = 100.0f;
    float mat_emission[] = {0.3f, 0.2f, 0.2f, 0.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, high_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

    for (std::vector<OpenGL_Triangle>::iterator it = mTriangles.begin(); it != mTriangles.end(); it++)
    {
     glNormal3f((*it).normal.x, (*it).normal.y, (*it).normal.z);
     glVertex3f((*it).a.x, (*it).a.y, (*it).a.z);
     glVertex3f((*it).b.x, (*it).b.y, (*it).b.z);
     glVertex3f((*it).c.x, (*it).c.y, (*it).c.z);
    }
    
   glEnd();
   glDisable(GL_LIGHTING);
   glBegin(GL_LINES);
     
     glLineWidth(2.0f);
     glEnable(GL_COLOR_MATERIAL);
     glEnable(GL_LINE_SMOOTH);
     
      for (std::vector<OpenGL_Triangle>::iterator it = mTriangles.begin(); it != mTriangles.end(); it++)
      {
       glColor3f(0,0,0);
       glVertex3f((*it).a.x * 1.01f, (*it).a.y * 1.01f, (*it).a.z * 1.01f);
       glVertex3f((*it).b.x * 1.01f, (*it).b.y * 1.01f, (*it).b.z * 1.01f);
       glVertex3f((*it).c.x * 1.01f, (*it).c.y * 1.01f, (*it).c.z * 1.01f);
       glVertex3f((*it).a.x * 1.01f, (*it).a.y * 1.01f, (*it).a.z * 1.01f);

       //glVertex3f((*it).a.x+(*it).normal.x, (*it).a.y+(*it).normal.y, (*it).a.z+(*it).normal.z);
     }
     
     glDisable(GL_LINE_SMOOTH);
     glDisable(GL_COLOR_MATERIAL);
     
   glEnd();
   glEnable(GL_LIGHTING);
   
 glPopMatrix();
}

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

#include "FlourToolCupcake.h"
#include "Flour.h"
#include <iostream>

#include <NxOgre.h>
#include <GL/glut.h>

#include <boost/algorithm/string.hpp>

namespace N = NxOgre;
namespace E = NxOgre::Enums;

FlourCupcake* mCupcake;

unsigned int RigidBodyData_Position        = 0; // X,Y,Z
unsigned int RigidBodyData_Orientation     = 1; // Quat
unsigned int RigidBodyData_LinearVelocity  = 2; // Dir
unsigned int RigidBodyData_AngularVelocity = 3; // Dir

void TW_CALL TW_Scene_Reset(void*)
{
 mCupcake->resetBodies();
}

void TW_CALL TW_RigidBody_Set(const void *value, void *clientData)
{
 
 N::Actor* actor = mCupcake->getSelectedActor();
 
 if (actor == 0)
  return;
 
 unsigned int type = (int) clientData;
 
 if (type == RigidBodyData_Position)
 {
  const float* vec = static_cast<const float*>(value);
  actor->setGlobalPosition(N::Vec3(vec[0], vec[1], vec[2]));
 }
 else if (type == RigidBodyData_Orientation)
 {
  const float* quat = static_cast<const float*>(value);
  actor->setGlobalOrientation(N::Quat(quat[3], quat[0], quat[1], quat[2]));
 }
 else if (type == RigidBodyData_LinearVelocity)
 {
  const float* vec = static_cast<const float*>(value);
  actor->setLinearVelocity(N::Vec3(vec[0], vec[1], vec[2]));
 }
 else if (type == RigidBodyData_AngularVelocity)
 {
  const float* vec = static_cast<const float*>(value);
  actor->setAngularVelocity(N::Vec3(vec[0], vec[1], vec[2]));
 }
 
}

void TW_CALL TW_RigidBody_Get(void *value, void *clientData)
{
 
 N::Actor* actor = mCupcake->getSelectedActor();
 
 if (actor == 0)
  return;
 
 unsigned int type = (int) clientData;
 
 if (type == RigidBodyData_Position)
 {  
  float* vec = static_cast<float*>(value);
  N::Vec3 pos = actor->getGlobalPosition();
  
  vec[0] = pos.x;
  vec[1] = pos.y;
  vec[2] = pos.z;
 }
 else if (type == RigidBodyData_Orientation)
 {
  
  float* quat = static_cast<float*>(value);
  N::Quat orientation = actor->getGlobalOrientation();
  
  quat[0] = orientation.x;
  quat[1] = orientation.y;
  quat[2] = orientation.z;
  quat[3] = orientation.w;
  
 }
 else if (type == RigidBodyData_LinearVelocity)
 {
  float* vec = static_cast<float*>(value);
  N::Vec3 pos = actor->getLinearVelocity();
  
  vec[0] = pos.x;
  vec[1] = pos.y;
  vec[2] = pos.z;
 }
 else if (type == RigidBodyData_AngularVelocity)
 {
  float* vec = static_cast<float*>(value);
  N::Vec3 pos = actor->getAngularVelocity();
  
  vec[0] = pos.x;
  vec[1] = pos.y;
  vec[2] = pos.z;
 }
 
}

FlourCupcake::FlourCupcake()
: FlourTool("cupcake", "Tests saved meshes in a real PhysX Scene"),
  OpenGL(),
  mYaw(-0.785398163 * 2),
  mPitch(0.523598776),
  mDistance(12)
{
 
 mOptionsDescription.add_options()
    ("help", "Show this")
    ("file,f", boost::program_options::value< std::vector<std::string> >(), "File(s) to process")
 ;
 
 mPositionalOptions.add("file", -1);
 
 add_error(ERROR_NoFile, "Need at least one file.");
 add_error(ERROR_NoMeshData, "File contains no mesh data.");
 add_error(ERROR_UnrecongisedFileformat, "Unrecongised file format.");
 
}

FlourCupcake::~FlourCupcake()
{
}

void FlourCupcake::process()
{
 
 mCupcake = this;
 
 // Need at least one file.
 if (mVariablesMap.count("file") == 0)
 {
  error(ERROR_NoFile, true);
  return;
 }

 createWindow("Flour - Cupcake", 1024, 768, OpenGL::WindowIcon_Cake);
 TwAddButton(mBar, "Reset", TW_Scene_Reset, 0, " group=Scene ");
 TwAddVarRW(mBar, "Yaw", TW_TYPE_FLOAT, &mYaw, " group=Camera min=-3.14159265 max=3.14159265 step=0.001 ");
 TwAddVarRW(mBar, "Pitch", TW_TYPE_FLOAT, &mPitch, " group=Camera min=0 max=1.2 step=0.001 ");
 TwAddVarRW(mBar, "Distance", TW_TYPE_FLOAT, &mDistance, " group=Camera min=0.25, max=100, step=0.05 ");
  
 createScene();
 
 TwAddVarCB(mBar, "Position", TW_TYPE_DIR3F, TW_RigidBody_Set, TW_RigidBody_Get, (void*) RigidBodyData_Position, " group=Body ");
 TwAddVarCB(mBar, "Orientation", TW_TYPE_QUAT4F, TW_RigidBody_Set, TW_RigidBody_Get, (void*) RigidBodyData_Orientation, " group=Body ");
 TwAddVarCB(mBar, "Linear Velocity", TW_TYPE_DIR3F, TW_RigidBody_Set, TW_RigidBody_Get, (void*) RigidBodyData_LinearVelocity, " group=Body ");
 TwAddVarCB(mBar, "Angular Velocity", TW_TYPE_DIR3F, TW_RigidBody_Set, TW_RigidBody_Get, (void*) RigidBodyData_AngularVelocity, " group=Body ");
 
 orbitCamera(mYaw, mPitch, mDistance);
 std::vector<std::string> files = mVariablesMap["file"].as<std::vector<std::string>>();
 
 std::stringstream enum_stream;
 enum_stream << " group=Scene enum='";

 for (unsigned int i=0; i < files.size(); i++)
 {
  std::string file_name = files[i];
  
  if (boost::contains(file_name, "*"))
  {
   std::vector<std::string> strings;
   boost::split(strings, file_name, boost::is_any_of("*"));
   file_name = strings[0];
   unsigned int count = boost::lexical_cast<unsigned int, std::string>(strings[1]);
   N::Vec3 pos;
   pos.x = 0;
   pos.y = 2;
   pos.z = 0;
   for (unsigned int j=0;j < count;j++)
   {
    Body* body = createBody(file_name, 100, pos);
    enum_stream << body->mID << " {" << file_name << body->mID << "}";
    enum_stream << ",";
    pos.y += 2;
   }
  }
  else
  {
   Body* body = createBody(file_name, 100);
   enum_stream << body->mID << " {" << file_name << body->mID << "}";
   enum_stream << ",";
  }
 }
 
 std::string enum_str(enum_stream.str());
 
 
 TwType bodies_enum = TwDefineEnum("BodiesType", NULL, 0);
 
 TwAddVarRW(mBar, "Selected", bodies_enum, &mWorkingBodyIndex, std::string(enum_str.substr(0, enum_str.length() - 1) + "' ").c_str());
 
 startRendering();
}

void FlourCupcake::onPreFrame()
{
 orbitCamera(mYaw, mPitch, mDistance);
}

void FlourCupcake::onPostFrame()
{
}

void FlourCupcake::onKeyEvent(char key)
{
 
 if (key == 'j' && mBodies.size())
 {
  Body* body = mBodies[mWorkingBodyIndex];
  if (body->mSceneGeometry)
   return;
  N::Vec3 torque;
  torque.x = (bml::math::random() - 0.5f) * body->mActor->getMass();
  torque.y = (bml::math::random() - 0.5f) * body->mActor->getMass();
  torque.z = (bml::math::random() - 0.5f) * body->mActor->getMass();
  torque *= 1000;
  body->mActor->addTorque(torque);
 }
 
}

void FlourCupcake::onMouseDragEvent(int ButtonID, int dx, int dy)
{
 if (ButtonID == GLUT_LEFT_BUTTON)
 {
  mYaw   += (float(dx) * 0.0075f);
  mPitch -= (float(dy) * 0.0075f);
 }
 
 orbitCamera(mYaw, mPitch, mDistance);
}

void FlourCupcake::onMouseButtonEvent(int ButtonID, int x, int y)
{
}


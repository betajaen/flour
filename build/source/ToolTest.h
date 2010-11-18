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



class ToolTest : public Tool
{
  
 public:
  
  bool process(Args& args)
  {
   
   if (args.flags.has("help") || args.flags.has("h"))
   {
    puts << "Performs a quick unit test on PhysX and NxOgre.\n";
    return false;
   }
   
   puts << " * Creating World\n";
   NxOgre::World* world = Flour::getSingleton()->getWorld();
  
   puts << " * Creating Scene\n";
    NxOgre::Scene* scene = world->createScene();
   
   puts << " * Creating Actor\n";
   NxOgre::Actor* actor = scene->createActor(NxOgre::BoxDescription(1));
   
   puts << " * Simulating for 3600 frames: ";
   size_t j = 0;
   for (size_t i=0;i != 60*60;i++)
   {
    j++;
    if (j == 120)
    {
     j = 0;
     puts << ".";
    }
    actor->addTorque(Vec3(10,0,0));
    world->advance(0.1667f);
   }
   
   puts << "\n * Cleaning Scene\n";
   world->destroyScene(scene);
   
   return true;
  }
  
  
}; // class
} // namespace.


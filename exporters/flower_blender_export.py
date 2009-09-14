#!BPY
# coding: utf-8
""" 
Name: 'Flour (.flower)...'
Blender: 249
Group: 'Export'
Tooltip: 'Export to Flour readable format (.flower).'
"""

__author__ = "Robin Southern"
__url__ = "flour.nxogre.org"

__version__ = "0.1"
__bpydoc__ = """\

Flour Exporter

This script Exports a flower file that is readable by Flour
"""

# File: FlourApp.cpp
# Created on: 06-Sept-09
# Author: Robin Southern "betajaen"
#
# Copyright (c) 2009 Robin Southern
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

import Blender
import bpy
from math import sqrt

def vec_length(v1, v2):
  x = v1.x - v2.x
  y = v1.y - v2.y
  z = v1.z - v2.z
  return sqrt((x * x) + (y * y) + (z * z))

def write_flower(filepath):
  out = file(filepath, 'w')
  sce = bpy.data.scenes.active
  ob = sce.objects.active
  mesh = ob.getData(mesh=1)
  out.write('# %s\n' % mesh.name)
  out.write('# Vertices %i\n' % len(mesh.verts))
  for vert in mesh.verts:
    out.write( 'vertices %f, %f, %f \n' % (vert.co.x, vert.co.y, vert.co.z) )
  
  out.write('\n# Indices %i\n' % (len(mesh.faces) * 6))
  for f in mesh.faces:
    if (len(f) == 3):
      out.write('indices %i, %i, %i \n' % (f.v[0].index, f.v[1].index, f.v[2].index))
    elif (len(f) == 4):
      if (vec_length(f.v[2].co, f.v[0].co) < vec_length(f.v[3].co, f.v[1].co)):
        out.write('indices %i, %i, %i,  %i, %i, %i\n' % (f.v[0].index, f.v[1].index, f.v[2].index,    f.v[0].index, f.v[2].index, f.v[3].index) )
      else:
        out.write('indices %i, %i, %i,  %i, %i, %i\n' % (f.v[0].index, f.v[1].index, f.v[3].index,    f.v[1].index, f.v[2].index, f.v[3].index) )
    else:
      out.write('# Eh?\n')

  out.close()

Blender.Window.FileSelector(write_flower, "Export", Blender.sys.makename(ext='.flower'))

#!BPY
# coding: utf-8
""" 
Name: 'Flour Convex Compound Actor (.flower)...'
Blender: 249
Group: 'Export'
Tooltip: 'Export scene as a compound actor(.flower).'
"""

__author__ = "Robin Southern"
__url__ = "flour.nxogre.org"
__version__ = "0.1"
__bpydoc__ = """\

Flour Compound Exporter

This script Exports a flower file that is readable by Flour
"""
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
import flour
import bpy


Blender.Window.FileSelector(flour.exportCompound, "Export", Blender.sys.makename(ext='.flower'))

    
    
    
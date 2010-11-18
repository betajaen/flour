import Blender
import bpy
from math import sqrt

def exportCompound(filepath):
  out = file(filepath, 'w')
  wroteFirst = False
  Scene = Blender.Scene.GetCurrent()
  for object in Scene.objects:
    if object.type == "Mesh":
      if wroteFirst == False:
        wroteFirst = True
        out.write("# Compound actor\n")
      else:
        out.write("\n---\n\n")
      position = object.getMatrix("worldspace").translationPart()
      rotation = object.getMatrix("worldspace").toQuat()
      exportPartial(out, object.getData(mesh=1), "convex", position, rotation)
  out.close()


def exportFlower(filepath, mesh_type=""):
  out = file(filepath, 'w')
  sce = bpy.data.scenes.active
  ob = sce.objects.active
  mesh = ob.getData(mesh=1)
  exportPartial(out, mesh, mesh_type)
  out.close()


def vec_length(v1, v2):
  x = v1.x - v2.x
  y = v1.y - v2.y
  z = v1.z - v2.z
  return sqrt((x * x) + (y * y) + (z * z))

def exportPartial(out, mesh, mesh_type="", pos=None, orientation=None):
  out.write('# %s\n' % mesh.name)
  if len(mesh_type):
    out.write('\ntype %s\n' % mesh_type)

  if (pos != None):
    out.write('position %f, %f, %f\n' % (pos[0], pos[1], pos[2]) )
  if (orientation != None):
    out.write('orientation %f, %f, %f, %f\n' % (orientation[0], orientation[1], orientation[2], orientation[3]) )

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


def cupcake():
  result = Draw.PupMenu("Convex|Triangle|Cloth|SoftBody")

  if (result == -1):
    return
    
  if (result == 1):
    exportFlower("cupcake.flower", "convex")

import bpy

obj_path = bpy.data.filepath.rpartition('.blend')[0] + '.obj'

bpy.ops.export_scene.obj(filepath = obj_path,
                         use_normals=True,
                         use_triangles=True)

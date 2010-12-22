# Export .blend files to .obj and .mtl

add_custom_target(OBJ_EXPORT)
file(GLOB Models_BLEND "*.blend")
foreach(file ${Models_BLEND})
    set(blend2obj_cmd blender -b ${file} -P ${SCRIPTS_PATH}/obj_export.py)
    if(UNIX)
        # Strange bug in blender not finding scripts directory when running in background mode.
        set(blend2obj_cmd BLENDER_SYSTEM_SCRIPTS=/usr/share/blender/2.55/scripts/ ${blend2obj_cmd})
    endif(UNIX)
    add_custom_command(TARGET OBJ_EXPORT
                       COMMAND ${blend2obj_cmd})
endforeach(file)
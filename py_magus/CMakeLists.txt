
# ===================================================================
# パッケージの検査
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${Python3_INCLUDE_DIRS}
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set ( py_magus_SOURCES
  magus_module.cc
  )


# ===================================================================
#  executable ターゲットの設定
# ===================================================================

ym_add_object_library( py_magus
  ${py_magus_SOURCES}
  )

add_library( py_maguslib
  SHARED
  $<TARGET_OBJECTS:py_magus_obj>
  $<TARGET_OBJECTS:py_magus_equiv_obj>
  $<TARGET_OBJECTS:py_ymbnet_obj>
  $<TARGET_OBJECTS:py_ymcell_obj>
  $<TARGET_OBJECTS:py_ymsat_obj>
  $<TARGET_OBJECTS:py_ymlogic_obj>
  $<TARGET_OBJECTS:py_ymbase_obj>
  ${YM_SUBMODULE_OBJ_LIST}
  )

set_target_properties( py_maguslib
  PROPERTIES
  PREFIX ""
  OUTPUT_NAME "magus"
  )

add_executable( py_magus
  py_magus.cc
  $<TARGET_OBJECTS:py_magus_obj>
  $<TARGET_OBJECTS:py_magus_equiv_obj>
  $<TARGET_OBJECTS:py_ymbnet_obj>
  $<TARGET_OBJECTS:py_ymcell_obj>
  $<TARGET_OBJECTS:py_ymsat_obj>
  $<TARGET_OBJECTS:py_ymlogic_obj>
  $<TARGET_OBJECTS:py_ymbase_obj>
  ${YM_SUBMODULE_OBJ_LIST}
  )

target_compile_options( py_magus
  PRIVATE "-O3"
  )

target_compile_definitions( py_magus
  PRIVATE "-DNODEBUG"
  )

target_link_libraries ( py_magus
  ${YM_LIB_DEPENDS}
  ${Python3_LIBRARIES}
  )

add_executable( py_magus_d
  py_magus.cc
  $<TARGET_OBJECTS:py_magus_obj_d>
  $<TARGET_OBJECTS:py_magus_equiv_obj_d>
  $<TARGET_OBJECTS:py_ymbnet_obj_d>
  $<TARGET_OBJECTS:py_ymcell_obj_d>
  $<TARGET_OBJECTS:py_ymsat_obj_d>
  $<TARGET_OBJECTS:py_ymlogic_obj_d>
  $<TARGET_OBJECTS:py_ymbase_obj_d>
  ${YM_SUBMODULE_OBJ_D_LIST}
  )

target_compile_options( py_magus_d
  PRIVATE "-g"
  )

target_link_libraries ( py_magus_d
  ${YM_LIB_DEPENDS}
  ${Python3_LIBRARIES}
  )


# ===================================================================
# テストターゲット
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${PROJECT_BINARY_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${YM_SUBMODULE_INCLUDE_LIST}
  ${LUA_INCLUDE_DIR}
  ${Readline_INCLUDE_DIR}
  )


# ===================================================================
#  ソースファイルの設定
# ===================================================================
set ( lua_magus_SOURCES
  lua_magus.cc
  LuaMagus.cc
  )


# ===================================================================
#  release ターゲットの設定
# ===================================================================
add_executable ( lua_magus
  ${lua_magus_SOURCES}
  $<TARGET_OBJECTS:magus_equiv_obj>
  $<TARGET_OBJECTS:magus_sbjgraph_obj>
  $<TARGET_OBJECTS:magus_lualutmap_obj>
  $<TARGET_OBJECTS:magus_cellmap_obj>
  $<TARGET_OBJECTS:magus_luaequiv_obj>
  $<TARGET_OBJECTS:ym_luaclib_obj>
  $<TARGET_OBJECTS:ym_luabdd_obj>
  $<TARGET_OBJECTS:ym_luabnet_obj>
  ${YM_SUBMODULE_OBJ_LIST}
  )

target_compile_options ( lua_magus
  PRIVATE "-O3"
  )

target_compile_definitions ( lua_magus
  PRIVATE "-DNDEBUG"
  )

if ( Readline_FOUND )
  target_compile_definitions ( lua_magus
    PRIVATE "-DHAS_READLINE"
    )
endif ()

target_link_libraries ( lua_magus
  ${YM_LIB_DEPENDS}
  )

if ( GPERFTOOLS_FOUND )
  target_link_libraries ( lua_magus
    ${GPERFTOOLS_LIBRARIES}
    )
endif ()


# ===================================================================
#  release with debug info ターゲットの設定
# ===================================================================
add_executable ( lua_magus_p
  ${lua_magus_SOURCES}
  $<TARGET_OBJECTS:magus_equiv_obj_p>
  $<TARGET_OBJECTS:magus_sbjgraph_obj_p>
  $<TARGET_OBJECTS:magus_lualutmap_obj_p>
  $<TARGET_OBJECTS:magus_cellmap_obj_p>
  $<TARGET_OBJECTS:magus_luaequiv_obj_p>
  $<TARGET_OBJECTS:ym_luaclib_obj_p>
  $<TARGET_OBJECTS:ym_luabdd_obj_p>
  $<TARGET_OBJECTS:ym_luabnet_obj_p>
  ${YM_SUBMODULE_OBJ_P_LIST}
  )

target_compile_options ( lua_magus_p
  PRIVATE "-O2" "-g"
  )

target_compile_definitions ( lua_magus_p
  PRIVATE "-DNDEBUG"
  )

if ( Readline_FOUND )
  target_compile_definitions ( lua_magus_p
    PRIVATE "-DHAS_READLINE"
    )
endif ()

target_link_libraries ( lua_magus_p
  ${YM_LIB_DEPENDS}
  )

if ( GPERFTOOLS_FOUND )
  target_link_libraries ( lua_magus_p
    ${GPERFTOOLS_LIBRARIES}
    )
endif ()


# ===================================================================
#  debug ターゲットの設定
# ===================================================================
add_executable (lua_magus_d
  ${lua_magus_SOURCES}
  $<TARGET_OBJECTS:magus_equiv_obj_d>
  $<TARGET_OBJECTS:magus_sbjgraph_obj_d>
  $<TARGET_OBJECTS:magus_lualutmap_obj_d>
  $<TARGET_OBJECTS:magus_cellmap_obj_d>
  $<TARGET_OBJECTS:magus_luaequiv_obj_d>
  $<TARGET_OBJECTS:ym_luaclib_obj_d>
  $<TARGET_OBJECTS:ym_luabdd_obj_d>
  $<TARGET_OBJECTS:ym_luabnet_obj_d>
  ${YM_SUBMODULE_OBJ_D_LIST}
  )

target_compile_options ( lua_magus_d
  PRIVATE "-g"
  )

if ( Readline_FOUND )
  target_compile_definitions ( lua_magus_d
    PRIVATE "-DHAS_READLINE"
    )
endif ()

target_link_libraries ( lua_magus_d
  ${YM_LIB_DEPENDS}
  )

if ( GPERFTOOLS_FOUND )
  target_link_libraries ( lua_magus_d
    ${GPERFTOOLS_LIBRARIES}
    )
endif ()


# ===================================================================
#  インストールターゲットの設定
# ===================================================================

install (
  TARGETS lua_magus lua_magus_p lua_magus_d
  DESTINATION bin
  )


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
set ( magus_lua_SOURCES
  magus_lua.cc
  LuaMagus_Bnet.cc
  )


# ===================================================================
#  release ターゲットの設定
# ===================================================================
add_executable ( magus_lua
  ${magus_lua_SOURCES}
  $<TARGET_OBJECTS:magus_equiv_obj>
  $<TARGET_OBJECTS:magus_sbjgraph_obj>
  $<TARGET_OBJECTS:magus_lutmap_obj>
  $<TARGET_OBJECTS:magus_cellmap_obj>
  $<TARGET_OBJECTS:magus_sbjgraph_obj>
  $<TARGET_OBJECTS:ym_luaclib_obj>
  ${YM_SUBMODULE_OBJ_LIST}
  )

target_compile_options ( magus_lua
  PRIVATE "-O3"
  )

target_compile_definitions ( magus_lua
  PRIVATE "-DNDEBUG"
  )

if ( Readline_FOUND )
  target_compile_definitions ( magus_lua
    PRIVATE "-DHAS_READLINE"
    )
endif ()

target_link_libraries ( magus_lua
  ${YM_LIB_DEPENDS}
  ${LUA_LIBRARIES}
  ${Readline_LIBRARY}
  )

if ( GPERFTOOLS_FOUND )
  target_link_libraries ( magus_lua
    ${GPERFTOOLS_LIBRARIES}
    )
endif ()


# ===================================================================
#  release with debug info ターゲットの設定
# ===================================================================
add_executable ( magus_lua_p
  ${magus_lua_SOURCES}
  $<TARGET_OBJECTS:magus_equiv_obj_p>
  $<TARGET_OBJECTS:magus_sbjgraph_obj_p>
  $<TARGET_OBJECTS:magus_lutmap_obj_p>
  $<TARGET_OBJECTS:magus_cellmap_obj_p>
  $<TARGET_OBJECTS:magus_sbjgraph_obj_p>
  $<TARGET_OBJECTS:ym_luaclib_obj_p>
  ${YM_SUBMODULE_OBJ_P_LIST}
  )

target_compile_options ( magus_lua_p
  PRIVATE "-O2" "-g"
  )

target_compile_definitions ( magus_lua_p
  PRIVATE "-DNDEBUG"
  )

if ( Readline_FOUND )
  target_compile_definitions ( magus_lua_p
    PRIVATE "-DHAS_READLINE"
    )
endif ()

target_link_libraries ( magus_lua_p
  ${YM_LIB_DEPENDS}
  ${LUA_LIBRARIES}
  ${Readline_LIBRARY}
  )

if ( GPERFTOOLS_FOUND )
  target_link_libraries ( magus_lua_p
    ${GPERFTOOLS_LIBRARIES}
    )
endif ()


# ===================================================================
#  debug ターゲットの設定
# ===================================================================
add_executable (magus_lua_d
  ${magus_lua_SOURCES}
  $<TARGET_OBJECTS:magus_equiv_obj_d>
  $<TARGET_OBJECTS:magus_sbjgraph_obj_d>
  $<TARGET_OBJECTS:magus_lutmap_obj_d>
  $<TARGET_OBJECTS:magus_cellmap_obj_d>
  $<TARGET_OBJECTS:magus_sbjgraph_obj_d>
  $<TARGET_OBJECTS:ym_luaclib_obj_d>
  ${YM_SUBMODULE_OBJ_D_LIST}
  )

target_compile_options ( magus_lua_d
  PRIVATE "-g"
  )

if ( Readline_FOUND )
  target_compile_definitions ( magus_lua_d
    PRIVATE "-DHAS_READLINE"
    )
endif ()

target_link_libraries ( magus_lua_d
  ${YM_LIB_DEPENDS}
  ${LUA_LIBRARIES}
  ${Readline_LIBRARY}
  )

if ( GPERFTOOLS_FOUND )
  target_link_libraries ( magus_lua_d
    ${GPERFTOOLS_LIBRARIES}
    )
endif ()


# ===================================================================
#  インストールターゲットの設定
# ===================================================================

install (
  TARGETS magus_lua magus_lua_p magus_lua_d
  DESTINATION bin
  )

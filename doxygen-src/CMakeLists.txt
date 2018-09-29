
# ===================================================================
# doxygen 起動用のコマンド
# ===================================================================

# doxygen の対象となるヘッダファイル(のパタン)を YM_DOXY_SRCS に入れる．
set ( YM_DOXY_SRCS )
foreach ( dir
    "${CMAKE_CURRENT_BINARY_DIR}"
    )
  list ( APPEND YM_DOXY_SRCS ${dir} )
endforeach ()
list ( APPEND YM_DOXY_SRCS "${PROJECT_SOURCE_DIR}/include" )
foreach ( dir
    "ym-bnet"
    "ym-cell"
    "ym-combopt"
    "ym-common"
    "ym-logic"
    "ym-mvn"
    "ym-sat"
    "ym-tclpp"
    "ym-verilog"
    )
  list ( APPEND YM_DOXY_SRCS "${PROJECT_SOURCE_DIR}/${dir}/include" )
endforeach ()

# YM_DOXY_SRCS を Doxyfile の INPUT 用に変換する．
string ( REPLACE ";" " " YM_DOXY_INPUT "${YM_DOXY_SRCS}" )

# Doxyfile の生成
configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in"
  "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
  )

# doxygen 用ターゲット
add_custom_target(dox ALL
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/docs/html/index.html"
  )

# doxygen 起動ルール
add_custom_command(
  COMMAND "${DOXYGEN_EXECUTABLE}" ">" "doxygen.log"
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" ${YM_DOXY_SRCS}
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/docs/html/index.html"
  COMMENT "generating doxygen documents"
  )

# 生成物のインストールルール
install (DIRECTORY
  "${CMAKE_CURRENT_BINARY_DIR}/docs"
  DESTINATION share/magus-${MAGUS_VERSION}/
  )

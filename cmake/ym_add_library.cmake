
# release, relwithdeb, debug ターゲットライブラリの設定
function (ym_add_library)
  foreach ( pos RANGE 0 ${ARGC} )
    if ( ${pos} EQUAL ${ARGC} )
      break()
    endif ()
    list (GET ARGV ${pos} argv)
    if ( ${pos} EQUAL 0 )
      set (_target_name ${argv})
    elseif ( ${pos} EQUAL 1 )
      if ( ${argv} STREQUAL "SHARED" )
	set (_option "SHARED")
      elseif ( ${argv} STREQUAL "STATIC" )
	set (_option "STATIC")
      else ()
	list (APPEND _sources ${argv})
      endif ()
    else ()
      list (APPEND _sources ${argv})
    endif ()
  endforeach ()

  # release モードのターゲット
  add_library (${_target_name}
    ${_option}
    ${_sources}
    )

  target_compile_options (${_target_name}
    PRIVATE "-O3"
    )

  target_compile_definitions (${_target_name}
    PRIVATE "-DNDEBUG"
    )

  # release with debug info モードのターゲット
  # 常に static にする．
  set (_target_name_p ${_target_name}_p)
  add_library (${_target_name_p}
    STATIC
    ${_sources}
    )

  target_compile_options (${_target_name_p}
    PRIVATE "-O2" "-g"
    )

  target_compile_definitions (${_target_name_p}
    PRIVATE "-DNDEBUG"
    )

  # debug モードのターゲット
  set (_target_name_d ${_target_name}_d)
  add_library (${_target_name_d}
    ${_option}
    ${_sources}
    )

  target_compile_options (${_target_name_d}
    PRIVATE "-g"
    )

  target_compile_definitions (${_target_name_d}
    PRIVATE
    )

endfunction ()

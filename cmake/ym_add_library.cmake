
# release, relwithdeb, debug ターゲットライブラリの設定
function (ym_add_library)
  foreach ( pos RANGE 0 ${ARGC} )
    if ( ${pos} EQUAL ${ARGC} )
      break()
    endif ()
    list (GET ARGV ${pos} argv)
    if ( ${pos} EQUAL 0 )
      # ターゲット名の設定
      # - 最適化モード
      # - プロファイルモード (_p)
      # - デバッグモード (_d)
      # の3つがある．
      set (_target_name "${argv}")
      set (_target_name_p "${argv}_p")
      set (_target_name_d "${argv}_d")
    elseif ( ${pos} EQUAL 1 )
      if ( ${argv} STREQUAL "SHARED" )
	set (_option "SHARED")
	# debug モードでは常にスタティック
	set (_dbg_option "STATIC")
      elseif ( ${argv} STREQUAL "STATIC" )
	set (_option "STATIC")
	set (_dbg_option "STATIC")
      elseif ( ${argv} STREQUAL "OBJECT" )
	set (_option "OBJECT")
	set (_dbg_option "OBJECT")
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
  add_library (${_target_name_p}
    ${_option}
    ${_sources}
    )

  target_compile_options (${_target_name_p}
    PRIVATE "-O2" "-g"
    )

  target_compile_definitions (${_target_name_p}
    PRIVATE "-DNDEBUG"
    )

  # debug モードのターゲット
  # 常に static にする．
  add_library (${_target_name_d}
    ${_dbg_option}
    ${_sources}
    )

  target_compile_options (${_target_name_d}
    PRIVATE "-g"
    )

  target_compile_definitions (${_target_name_d}
    PRIVATE
    )

endfunction ()

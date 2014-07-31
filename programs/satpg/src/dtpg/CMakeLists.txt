
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}
  ../../include
  )


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (SOURCES
  bt/BtBase.cc
  bt/BtJust1.cc
  bt/BtJust2.cc
  bt/BtJustBase.cc
  bt/BtSimple.cc
  dop/DopBase.cc
  dop/DopDrop.cc
  dop/DopDummy.cc
  dop/DopList.cc
  dop/DopTvList.cc
  dop/DopVerify.cc
  dop/FopDrop.cc
  main/DtpgDriver.cc
  main/LitMap.cc
  main/SatEngine.cc
  main/SatEngineMulti.cc
  main/SatEngineMulti2.cc
  main/SatEngineSingle.cc
  main/SatEngineSingle2.cc
  main/SmtEngine.cc
  main/SmtEngineSingle.cc
  uop/UopBase.cc
  uop/UopDummy.cc
  uop/UopList.cc
  uop/UopSkip.cc
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
add_library(satpg_dtpg STATIC ${SOURCES})

target_link_libraries(satpg_dtpg
  ym_logic
  )


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}
  include
  )


# ===================================================================
#  ソースファイルの設定
# ===================================================================
set (main_SOURCES
  src/CellFFInfo.cc
  src/CellLatchInfo.cc
  src/display.cc
  )

set (ci_SOURCES
  src/ci/CiBundle.cc
  src/ci/CiBus.cc
  src/ci/CiBusType.cc
  src/ci/CiCell.cc
  src/ci/CiCellHash.cc
  src/ci/CiClass.cc
  src/ci/CiFFCell.cc
  src/ci/CiFsmCell.cc
  src/ci/CiGroup.cc
  src/ci/CiLatchCell.cc
  src/ci/CiLibrary.cc
  src/ci/CiLogicCell.cc
  src/ci/CiLut.cc
  src/ci/CiLutHash.cc
  src/ci/CiLutTemplate.cc
  src/ci/CiPatGraph.cc
  src/ci/CiPatMgr.cc
  src/ci/CiPin.cc
  src/ci/CiPinHash.cc
  src/ci/CiTiming.cc
  )

set (dotlib_SOURCES
  src/dotlib/CellDotlibReader.cc
  src/dotlib/ComplexHandler.cc
  src/dotlib/DefineHandler.cc
  src/dotlib/DotlibAttr.cc
  src/dotlib/DotlibAttrMap.cc
  src/dotlib/DotlibCell.cc
  src/dotlib/DotlibFF.cc
  src/dotlib/DotlibFL.cc
  src/dotlib/DotlibHandler.cc
  src/dotlib/DotlibLatch.cc
  src/dotlib/DotlibLibrary.cc
  src/dotlib/DotlibLut.cc
  src/dotlib/DotlibMgr.cc
  src/dotlib/DotlibMgrImpl.cc
  src/dotlib/DotlibNode.cc
  src/dotlib/DotlibNodeImpl.cc
  src/dotlib/DotlibParser.cc
  src/dotlib/DotlibParserImpl.cc
  src/dotlib/DotlibPin.cc
  src/dotlib/DotlibScanner.cc
#  src/dotlib/DotlibStateTable.cc
  src/dotlib/DotlibTemplate.cc
  src/dotlib/DotlibTiming.cc
  src/dotlib/ExprHandler.cc
  src/dotlib/FhScanner.cc
  src/dotlib/FuncHandler.cc
  src/dotlib/GroupHandler.cc
  src/dotlib/HandlerFactory.cc
  src/dotlib/SimpleHandler.cc
  )

set (libcomp_SOURCES
  src/libcomp/LcClass.cc
  src/libcomp/LcFFMgr.cc
  src/libcomp/LcGroup.cc
  src/libcomp/LcGroupMgr.cc
  src/libcomp/LcLatchMgr.cc
  src/libcomp/LcLogicMgr.cc
  src/libcomp/LcPatMgr.cc
  src/libcomp/LibComp.cc
  )

set (mislib_SOURCES
  src/mislib/CellMislibReader.cc
  src/mislib/MislibBop.cc
  src/mislib/MislibConst.cc
  src/mislib/MislibGate.cc
  src/mislib/MislibList.cc
  src/mislib/MislibMgr.cc
  src/mislib/MislibMgrImpl.cc
  src/mislib/MislibNode.h
  src/mislib/MislibNodeImpl.cc
  src/mislib/MislibNot.cc
  src/mislib/MislibNum.cc
  src/mislib/MislibParser.cc
  src/mislib/MislibParserImpl.cc
  src/mislib/MislibPin.cc
  src/mislib/MislibScanner.cc
  src/mislib/MislibStr.cc
  src/mislib/MislibSymbol.cc
  )

# ===================================================================
#  ターゲットの設定
# ===================================================================
add_library(ym_cell
  ${main_SOURCES}
  ${ci_SOURCES}
  ${dotlib_SOURCES}
  ${libcomp_SOURCES}
  ${mislib_SOURCES}
  )
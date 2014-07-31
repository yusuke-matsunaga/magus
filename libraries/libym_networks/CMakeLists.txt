
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

set (SOURCES
  src/BNetBdnConv.cc
  )

set (SOURCES
  ${SOURCES}
  src/bdn/BdnBlifWriter.cc
  src/bdn/BdnDumper.cc
  src/bdn/BdnMgr.cc
  src/bdn/BdnMgrImpl.cc
  src/bdn/BdnNode.cc
  src/bdn/BdnVerilogWriter.cc
  )

set (SOURCES
  ${SOURCES}
  src/blif/BlifDic.cc
  src/blif/BlifHandler.cc
  src/blif/BlifNetwork.cc
  src/blif/BlifNetworkHandler.cc
  src/blif/BlifNetworkImpl.cc
  src/blif/BlifNodeImpl.cc
  src/blif/BlifParser.cc
  src/blif/BlifScanner.cc
  src/blif/IdHash.cc
  )

set (SOURCES
  ${SOURCES}
  src/bnet/BNetBlifWriter.cc
  src/bnet/BNetDecomp.cc
  src/bnet/BNetEquWriter.cc
  src/bnet/BNetGateDecomp.cc
  src/bnet/BNetManip.cc
  src/bnet/BNetSopDecomp.cc
  src/bnet/BNetVerilogWriter.cc
  src/bnet/BNetwork.cc
  src/bnet/BNode.cc
  src/bnet/BNodeMgr.cc
  src/bnet/StrBNodeMap.cc
  src/bnet/eliminate.cc
  )

set (SOURCES
  ${SOURCES}
  src/cmn/BlifWriterImpl.cc
  src/cmn/CmnBlifWriter.cc
  src/cmn/CmnDffCell.cc
  src/cmn/CmnDumper.cc
  src/cmn/CmnLatchCell.cc
  src/cmn/CmnMgr.cc
  src/cmn/CmnMgrImpl.cc
  src/cmn/CmnNode.cc
  src/cmn/CmnPort.cc
  src/cmn/CmnVerilogWriter.cc
  src/cmn/VerilogWriterImpl.cc
  )

set (SOURCES
  ${SOURCES}
  src/conv_mvn_bdn/AddConv.cc
  src/conv_mvn_bdn/AndConv.cc
  src/conv_mvn_bdn/CaseEqConv.cc
  src/conv_mvn_bdn/CmplConv.cc
  src/conv_mvn_bdn/ConcatConv.cc
  src/conv_mvn_bdn/ConstBitSelectConv.cc
  src/conv_mvn_bdn/ConstPartSelectConv.cc
  src/conv_mvn_bdn/DivConv.cc
  src/conv_mvn_bdn/EqConv.cc
  src/conv_mvn_bdn/IteConv.cc
  src/conv_mvn_bdn/LtConv.cc
  src/conv_mvn_bdn/ModConv.cc
  src/conv_mvn_bdn/MultConv.cc
  src/conv_mvn_bdn/MvnBdnConv.cc
  src/conv_mvn_bdn/MvnBdnMap.cc
  src/conv_mvn_bdn/NotConv.cc
  src/conv_mvn_bdn/OrConv.cc
  src/conv_mvn_bdn/PowConv.cc
  src/conv_mvn_bdn/RandConv.cc
  src/conv_mvn_bdn/RorConv.cc
  src/conv_mvn_bdn/RxorConv.cc
  src/conv_mvn_bdn/SlaConv.cc
  src/conv_mvn_bdn/SllConv.cc
  src/conv_mvn_bdn/SraConv.cc
  src/conv_mvn_bdn/SrlConv.cc
  src/conv_mvn_bdn/SubConv.cc
  src/conv_mvn_bdn/ThroughConv.cc
  src/conv_mvn_bdn/XorConv.cc
  )

set (SOURCES
  ${SOURCES}
  src/iscas89/IdHash.cc
  src/iscas89/Iscas89Handler.cc
  src/iscas89/Iscas89Network.cc
  src/iscas89/Iscas89NetworkHandler.cc
  src/iscas89/Iscas89NetworkReader.cc
  src/iscas89/Iscas89Parser.cc
  src/iscas89/Iscas89ParserImpl.cc
  src/iscas89/Iscas89Scanner.cc
  )

set (SOURCES
  ${SOURCES}
  src/mvn/MvnCaseEq.cc
  src/mvn/MvnCellNode.cc
  src/mvn/MvnConst.cc
  src/mvn/MvnConstBitSelect.cc
  src/mvn/MvnConstPartSelect.cc
  src/mvn/MvnDff.cc
  src/mvn/MvnDumper.cc
  src/mvn/MvnMgr.cc
  src/mvn/MvnModule.cc
  src/mvn/MvnNode.cc
  src/mvn/MvnNodeBase.cc
  src/mvn/MvnPort.cc
  src/mvn/MvnVerilogWriter.cc
  src/mvn/VerilogWriterImpl.cc
  )

set (SOURCES
  ${SOURCES}
  src/tgnet/LogicMgr.cc
  src/tgnet/NameHash.cc
  src/tgnet/TgNetwork.cc
  )

set (SOURCES
  ${SOURCES}
  src/verilog/VlWriter.cc
  src/verilog/VlwAssign.cc
  src/verilog/VlwBase.cc
  src/verilog/VlwIO.cc
  src/verilog/VlwInst.cc
  src/verilog/VlwModule.cc
  src/verilog/VlwModuleHeader.cc
  )


# Create target for the parser
 ADD_CUSTOM_TARGET(iscas89_grammer echo "Creating iscas89_grammer.cc")

# Create custom command for bison/yacc (note the DEPENDS)
 ADD_CUSTOM_COMMAND(
   SOURCE src/iscas89/iscas89_grammer.yy
   COMMAND ${BISON_EXECUTABLE}
   ARGS -y ${CMAKE_CURRENT_SOURCE_DIR}/src/iscas89/iscas89_grammer.yy
        -o ${CMAKE_CURRENT_BINARY_DIR}/iscas89_grammer.cc
	-d -v
   TARGET iscas89_grammer
   OUTPUTS ${CMAKE_CURRENT_BINARY_DIR}/iscas89_grammer.cc)

# Add iscas89_grammer.cc to the list of sources
 SET(SOURCES ${SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/iscas89_grammer.cc)

# Since parser.c does not exists yet when cmake is run, mark
# it as generated
 SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_BINARY_DIR}/iscas89_grammer.cc GENERATED)

# Include binary directory to include lexer.c in parser.c
 INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# ===================================================================
#  ターゲットの設定
# ===================================================================

add_library(ym_networks SHARED
  ${SOURCES}
  )

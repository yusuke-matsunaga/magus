
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

set ( main_SOURCES
  src/BNetBdnConv.cc
  )

set ( bdn_SOURCES
  src/bdn/BdnBlifWriter.cc
  src/bdn/BdnDumper.cc
  src/bdn/BdnMgr.cc
  src/bdn/BdnMgrImpl.cc
  src/bdn/BdnNode.cc
  src/bdn/BdnVerilogWriter.cc

  src/bdn/blif/BdnBlifReader.cc
  src/bdn/blif/BlifBdnConv.cc

  src/bdn/iscas89/BdnIscas89Reader.cc
  src/bdn/iscas89/Iscas89BdnConv.cc
  )

set ( blif_SOURCES
  src/blif/BlifDic.cc
  src/blif/BlifHandler.cc
  src/blif/BlifIdHash.cc
  src/blif/BlifNetwork.cc
  src/blif/BlifNetworkHandler.cc
  src/blif/BlifNetworkImpl.cc
  src/blif/BlifNodeImpl.cc
  src/blif/BlifParser.cc
  src/blif/BlifScanner.cc
  )

set ( bnet_SOURCES
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

  src/bnet/blif/BNetBlifHandler.cc
  src/bnet/blif/BNetBlifReader.cc

  src/bnet/iscas89/BNetIscas89Handler.cc
  src/bnet/iscas89/BNetIscas89Reader.cc
  )

set ( cmn_SOURCES
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

set ( conv_mvn_bdn_SOURCES
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

set ( iscas89_SOURCES
  src/iscas89/Iscas89Handler.cc
  src/iscas89/Iscas89IdHash.cc
  src/iscas89/Iscas89Network.cc
  src/iscas89/Iscas89NetworkHandler.cc
  src/iscas89/Iscas89NetworkReader.cc
  src/iscas89/Iscas89Parser.cc
  src/iscas89/Iscas89ParserImpl.cc
  src/iscas89/Iscas89Scanner.cc
  )

set ( mvn_SOURCES
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
  src/mvn/verilog/DeclHash.cc
  src/mvn/verilog/DeclMap.cc
  src/mvn/verilog/Env.cc
  src/mvn/verilog/EnvMerger.cc
  src/mvn/verilog/MvnVerilogReader.cc
  src/mvn/verilog/MvnVlMap.cc
  src/mvn/verilog/ReaderImpl.cc
  src/mvn/verilog/ReaderImpl_expr.cc
  src/mvn/verilog/ReaderImpl_item.cc
  src/mvn/verilog/ReaderImpl_stmt.cc
  src/mvn/verilog/Xmask.cc
  )

set ( tgnet_SOURCES
  src/tgnet/LogicMgr.cc
  src/tgnet/NameHash.cc
  src/tgnet/TgNetwork.cc

  src/tgnet/blif/TgBlifHandler.cc
  src/tgnet/blif/TgBlifReader.cc

  src/tgnet/iscas89/TgIscas89Handler.cc
  src/tgnet/iscas89/TgIscas89Reader.cc
  )

set ( verilog_SOURCES
  src/verilog/VlWriter.cc
  src/verilog/VlwAssign.cc
  src/verilog/VlwBase.cc
  src/verilog/VlwIO.cc
  src/verilog/VlwInst.cc
  src/verilog/VlwModule.cc
  src/verilog/VlwModuleHeader.cc
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================

add_library( ym_networks
  SHARED
  ${main_SOURCES}
  ${bdn_SOURCES}
  ${blif_SOURCES}
  ${bnet_SOURCES}
  ${cmn_SOURCES}
  ${conv_mvn_bdn_SOURCES}
  ${iscas89_SOURCES}
  ${mvn_SOURCES}
  ${tgnet_SOURCES}
  ${verilog_SOURCES}
  )

target_link_libraries ( ym_networks
  ym_verilog
  ym_cell
  ym_logic
  ym_utils
  )

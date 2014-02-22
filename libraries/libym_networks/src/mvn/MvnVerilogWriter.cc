
/// @file MvnVerilogWriter.cc
/// @brief MvnVerilogWriter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "networks/MvnVerilogWriter.h"

#include "VerilogWriterImpl.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnVerilogWriter
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MvnVerilogWriter::MvnVerilogWriter() :
  mImpl(new VerilogWriterImpl())
{
}

// @brief デストラクタ
MvnVerilogWriter::~MvnVerilogWriter()
{
  delete mImpl;
}

// @brief 内容を Verilog-HDL 形式で出力する
// @param[in] s 出力先のストリーム
// @param[in] mgr MvnMgr
void
MvnVerilogWriter::operator()(ostream& s,
			     const MvnMgr& mgr)
{
  mImpl->dump(s, mgr);
}

// @brief 内容を Verilog-HDL 形式で出力する
// @param[in] s 出力先のストリーム
// @param[in] mgr MvnMgr
// @param[in] node_map ノードと Verilog 名の対応表
void
MvnVerilogWriter::operator()(ostream& s,
			     const MvnMgr& mgr,
			     const MvnVlMap& node_map)
{
  mImpl->dump(s, mgr, node_map);
}

END_NAMESPACE_YM_NETWORKS_MVN


/// @file CmnVerilogWriter.cc
/// @brief CmnVerilogWriter 実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/CmnVerilogWriter.h"

#include "VerilogWriterImpl.h"


BEGIN_NAMESPACE_YM_NETWORKS_CMN

// @brief コンストラクタ
CmnVerilogWriter::CmnVerilogWriter() :
  mImpl(new VerilogWriterImpl)
{
}

// @brief デストラクタ
CmnVerilogWriter::~CmnVerilogWriter()
{
  delete mImpl;
}


// 内容を Verilog-HDL 形式で s に出力する．
void
CmnVerilogWriter::operator()(ostream& s,
			     const CmnMgr& network)
{
  mImpl->dump(s, network);
}

END_NAMESPACE_YM_NETWORKS_CMN

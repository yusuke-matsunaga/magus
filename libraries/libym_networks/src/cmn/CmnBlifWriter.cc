
/// @file CmnBlifWriter.cc
/// @brief CmnBlifWriter 実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "networks/CmnBlifWriter.h"

#include "BlifWriterImpl.h"


BEGIN_NAMESPACE_YM_NETWORKS_CMN

// @brief コンストラクタ
CmnBlifWriter::CmnBlifWriter() :
  mImpl(new BlifWriterImpl)
{
}

// @brief デストラクタ
CmnBlifWriter::~CmnBlifWriter()
{
  delete mImpl;
}


// 内容を Blif-HDL 形式で s に出力する．
void
CmnBlifWriter::operator()(ostream& s,
			  const CmnMgr& network)
{
  mImpl->dump(s, network);
}

END_NAMESPACE_YM_NETWORKS_CMN

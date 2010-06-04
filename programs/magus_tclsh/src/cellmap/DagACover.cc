
/// @file magus/cellmap/DagCoverFactory.cc
/// @brief DagCoverFactory の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DagACover.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "DagACover.h"
#include "WeightCover.h"


BEGIN_NAMESPACE_MAGUS_CELLMAP

//////////////////////////////////////////////////////////////////////
// クラス DagACoverFactory
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DagACoverFactory::DagACoverFactory()
{
}

// @brief デストラクタ
DagACoverFactory::~DagACoverFactory()
{
}

// @brief DagACover (の派生クラス)を生成する．
DagACover*
DagACoverFactory::operator()(const string& args)
{
  if ( args == "dag" ) {
    return new WeightCover(0);
  }
  if ( args == "fo" ) {
    return new WeightCover(1);
  }
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

END_NAMESPACE_MAGUS_CELLMAP

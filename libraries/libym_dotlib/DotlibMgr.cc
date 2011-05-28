
/// @file libym_dotlib/DotlibMgr.cc
/// @brief DotlibMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/DotlibMgr.h"
#include "DotlibMgrImpl.h"
#include "ym_utils/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotlibMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DotlibMgr::DotlibMgr() :
  mImpl(new DotlibMgrImpl)
{
}

// @brief デストラクタ
DotlibMgr::~DotlibMgr()
{
  delete mImpl;
}

// @brief 初期化する．
// @note 以前，生成されたオブジェクトは破壊される．
void
DotlibMgr::clear()
{
  mImpl->clear();
}

// @brief 根のノードを返す．
const DotlibNode*
DotlibMgr::root_node() const
{
  return mImpl->root_node();
}

// @brief 使用メモリ量の一覧を出力する．
// @param[in] s 出力先のストリーム
void
DotlibMgr::show_stats(ostream& s) const
{
  mImpl->show_stats(s);
}

END_NAMESPACE_YM_DOTLIB


/// @file BtBase.cc
/// @brief BtBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "BtBase.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス BtBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BtBase::BtBase()
{
}

// @brief デストラクタ
BtBase::~BtBase()
{
}

// @brief ノードID番号の最大値を設定する．
/// @param[in] max_id ID番号の最大値
//
// このクラスの実装ではなにもしない．
void
BtBase::set_max_id(ymuint max_id)
{
}

END_NAMESPACE_YM_SATPG

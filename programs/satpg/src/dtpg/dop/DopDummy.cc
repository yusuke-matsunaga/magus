
/// @file DopDummy.cc
/// @brief DopDummy の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DopDummy.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'base' タイプを生成する．
DetectOp*
new_DopDummy()
{
  return new DopDummy();
}


//////////////////////////////////////////////////////////////////////
// クラス DopDummy
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DopDummy::DopDummy()
{
}

// @brief デストラクタ
DopDummy::~DopDummy()
{
}

// @brief テストパタンが見つかった時の処理
// @param[in] f 故障
// @param[in] assign_list 値の割当リスト
void
DopDummy::operator()(TpgFault* f,
		     const AssignList& assign_list)
{
}

END_NAMESPACE_YM_SATPG

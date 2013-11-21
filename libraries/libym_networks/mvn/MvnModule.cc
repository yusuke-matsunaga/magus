
/// @file MvnModule.cc
/// @brief MvnModule の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/MvnModule.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] np ポート数
// @param[in] ni 入力ノード数
// @param[in] no 出力ノード数
// @param[in] nio 入出力ノード数
MvnModule::MvnModule(const char* name,
		     ymuint np,
		     ymuint ni,
		     ymuint no,
		     ymuint nio) :
  mName(name),
  mParent(NULL),
  mPortArray(np),
  mInputArray(ni),
  mOutputArray(no),
  mInoutArray(nio)
{
}

// @brief デストラクタ
MvnModule::~MvnModule()
{
}

END_NAMESPACE_YM_NETWORKS_MVN

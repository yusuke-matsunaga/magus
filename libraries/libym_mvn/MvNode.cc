
/// @file libym_mvn/MvNode.cc
/// @brief MvNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvNode.h"
#include "ym_mvn/MvPin.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
MvNode::MvNode(MvModule* parent) :
  mParent(parent)
{
}

// @brief デストラクタ
MvNode::~MvNode()
{
  cout << "MvNode is deleted" << endl;
}

// @param[in] ピンを初期化する．
// @param[in] pin 対象のピン
// @param[in] pos 位置
// @param[in] bit_width ビット幅
void
MvNode::init_pin(MvPin* pin,
		 ymuint pos,
		 ymuint bit_width)
{
  pin->mNode = this;
  pin->mPos = pos;
  pin->mBitWidth = bit_width;
}

// @brief 入力ノード/出力ノードの場合に位置を返す．
// @note type() が kInput と kOutput の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
ymuint
MvNode::pos() const
{
  return 0;
}

// @brief 下位モジュールを得る．
// @note type() が kInst の時のみ意味を持つ．
// @note デフォルトの実装では NULL を返す．
const MvModule*
MvNode::module() const
{
  return NULL;
}

// @brief 定数値を得る．
// @param[out] val 値を格納するベクタ
// @note type() が kConst の時のみ意味を持つ．
// @note デフォルトの実装ではなにもしない．
void
MvNode::const_value(vector<ymuint32>& val) const
{
}

END_NAMESPACE_YM_MVN

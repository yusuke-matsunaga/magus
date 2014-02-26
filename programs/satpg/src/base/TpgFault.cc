
/// @file TpgFault.cc
/// @brief TpgFault の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2007, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "TpgFault.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include <sstream>


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgFault
//////////////////////////////////////////////////////////////////////

// @brief 内容を設定する．
// @param[in] id ID番号
// @param[in] node 対象のゲート
// @param[in] output 入力の故障のとき false, 出力の故障のとき true を与える．
// @param[in] pos 故障位置
// @param[in] val 故障値 0 か非0 で 0/1 を区別する．
// @param[in] rep_fault 代表故障
void
TpgFault::set(ymuint id,
	      TpgNode* node,
	      bool output,
	      ymuint pos,
	      int val,
	      TpgFault* rep_fault)
{
  mId = id;
  mNode = node;

  mPosVal = 0U;
  if ( val ) {
    mPosVal |= 1U;
  }
  if ( output ) {
    mPosVal |= 2U;
  }
  mPosVal |= (pos << 3);

  mStatus = kFsUndetected;

  if ( rep_fault != NULL ) {
    mRepFault = rep_fault;
  }
  else {
    mRepFault = this;
  }
}

// @brief 故障の入力側のゲートを返す．
TpgNode*
TpgFault::source_node() const
{
  if ( is_output_fault() ) {
    return node();
  }
  else {
    return node()->fanin(pos());
  }
}

// @brief 故障の内容を表す文字列を返す．
string
TpgFault::str() const
{
  ostringstream ans;
  ans << node()->name() << ":";
  if ( is_input_fault() ) {
    ans << "I" << pos();
  }
  else {
    ans << "O";
  }
  ans << ":";
  if ( val() ) {
    ans <<"SA1";
  }
  else {
    ans <<"SA0";
  }
  return ans.str();
}

// @brief ストリーム出力演算子
// @param[in] s 出力先のストリーム
// @param[in] f 故障
ostream&
operator<<(ostream& s,
	   const TpgFault* f)
{
  return s << f->str();
}

END_NAMESPACE_YM_SATPG

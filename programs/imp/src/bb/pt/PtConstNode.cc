
/// @file src/pt/PtConstNode.cc
/// @brief PtConstNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtConstNode.h"


BEGIN_NAMESPACE_YM_BB

// @brief コンストラクタ
// @param[in] file_region ファイル上の位置
// @param[in] value 値
PtConstNode::PtConstNode(const FileRegion& file_region,
			 ymint32 value) :
  PtNode(file_region),
  mValue(value)
{
}

// @brief デストラクタ
PtConstNode::~PtConstNode()
{
}

// @brief 型を返す．
PtNode::tType
PtConstNode::type() const
{
  return kConst;
}

// @brief 定数値を返す．
// @note 定数型のノードの時に意味を持つ．
int
PtConstNode::cvalue() const
{
  return mValue;
}

// @brief 内容を表す文字列を返す．
string
PtConstNode::decompile() const
{
  ostringstream buf;
  buf << mValue;
  return buf.str();
}

// @brief 対応した AIG を作る．
void
PtConstNode::gen_aig(AigMgr& aigmgr,
		     const vector<Aig>& bvar_array,
		     ymuint bw,
		     vector<Aig>& out_array)
{
  ymuint n = bw;
  if ( bw > 32 ) {
    n = 32;
  }
  for (ymuint i = 0; i < n; ++ i) {
    if ( (mValue >> i) & 1 ) {
      out_array[i] = aigmgr.make_one();
    }
    else {
      out_array[i] = aigmgr.make_zero();
    }
  }
  for (ymuint i = n; i < bw; ++ i) {
    out_array[i] = aigmgr.make_zero();
  }
}

END_NAMESPACE_YM_BB

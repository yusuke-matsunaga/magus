
/// @file src/pt/PtIdNode.cc
/// @brief PtIdNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtIdNode.h"


BEGIN_NAMESPACE_YM_BB

// @brief コンストラクタ
// @param[in] file_region ファイル上の位置
// @param[in] id ID番号
PtIdNode::PtIdNode(const FileRegion& file_region,
		   ymuint id) :
  PtNode(file_region),
  mId(id)
{
}

// @param[in]
// @brief デストラクタ
PtIdNode::~PtIdNode()
{
}

// @brief 型を返す．
PtNode::tType
PtIdNode::type() const
{
  return kId;
}

// @brief ID 番号を返す．
// @note ID型のノードの時に意味を持つ．
ymuint
PtIdNode::id() const
{
  return mId;
}

// @brief 内容を表す文字列を返す．
string
PtIdNode::decompile() const
{
  ostringstream buf;
  buf << "Var#" << id();
  return buf.str();
}

// @brief 対応した AIG を作る．
void
PtIdNode::gen_aig(AigMgr& aigmgr,
		  const vector<Aig>& bvar_array,
		  ymuint bw,
		  vector<Aig>& out_array)
{
  ymuint base = id() * bw;
  for (ymuint i = 0; i < bw; ++ i) {
    out_array[i] = bvar_array[base + i];
  }
}

END_NAMESPACE_YM_BB

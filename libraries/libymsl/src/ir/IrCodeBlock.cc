
/// @file IrCodeBlock.cc
/// @brief IrCodeBlock の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrCodeBlock.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrCodeBlock
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
IrCodeBlock::IrCodeBlock()
{
}

// @brief デストラクタ
IrCodeBlock::~IrCodeBlock()
{
}

// @brief 変数を追加する．
void
IrCodeBlock::add_var(Var* var)
{
  ymuint index = mVarList.size();
  mVarList.push_back(var);
}

// @brief ノードを追加する．
void
IrCodeBlock::add_node(IrNode* node)
{
  mNodeList.push_back(node);
}

// @brief 変数のリストを得る．
const vector<const Var*>&
IrCodeBlock::var_list() const
{
  return mVarList;
}

// @brief ノードのリストを得る．
const vector<IrNode*>&
IrCodeBlock::node_list() const
{
  return mNodeList;
}

END_NAMESPACE_YM_YMSL


/// @file libym_mvn/verilog/DfgBlock.cc
/// @brief DfgBlock の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "DfgBlock.h"
#include "SsaMgr.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ(最上位のブロック用)
// @param[in] ssa_mgr SSA用のマネージャ
// @param[in] stmt_list ステートメントのリスト
DfgBlock::DfgBlock(SsaMgr& ssa_mgr,
		   const vector<const VlStmt*>& stmt_list) :
  mSsaMgr(ssa_mgr),
  mParent(NULL),
  mStmtList(stmt_list)
{
}

// @brief コンストラクタ(通常のブロック用)
// @param[in] parent 親のブロック
// @param[in] stmt_list ステートメントのリスト
DfgBlock::DfgBlock(DfgBlock* parent,
		   const vector<const VlStmt*>& stmt_list) :
  mSsaMgr(parent->mSsaMgr),
  mParent(parent),
  mStmtList(stmt_list)
{
}

// @brief デストラクタ
DfgBlock::~DfgBlock()
{
}

// @brief 宣言要素に対するインスタンス番号を返す．
// @param[in] decl 宣言要素
// @return decl の最後の代入のインスタンス番号
// @note このブロックで代入されていないときには 0 を返す．
ymuint
DfgBlock::last_id(const VlDecl* decl) const
{
  return mSsaMgr.last_id(this, decl);
}

// @brief 宣言要素に新しいインスタンス番号を割り当てる．
// @param[in] decl 宣言要素
ymuint
DfgBlock::new_id(const VlDecl* decl)
{
  return mSsaMgr.new_id(this, decl);
}

END_NAMESPACE_YM_MVN


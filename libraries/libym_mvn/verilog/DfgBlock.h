#ifndef LIBYM_MVN_VERILOG_DFGBLOCK_H
#define LIBYM_MVN_VERILOG_DFGBLOCK_H

/// @file libym_mvn/verilog/DfgBlock.h
/// @brief DfgBlock のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"
#include "ym_verilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_MVN

class SsaMgr;

//////////////////////////////////////////////////////////////////////
/// @class DfgBlock DfgBlock.h "DfgBlock.h"
/// @brief DFG のブロックを表すクラス
//////////////////////////////////////////////////////////////////////
class DfgBlock
{
public:

  /// @brief コンストラクタ(最上位のブロック用)
  /// @param[in] ssa_mgr SSA用のマネージャ
  /// @param[in] stmt_list ステートメントのリスト
  DfgBlock(SsaMgr& ssa_mgr,
	   const vector<const VlStmt*>& stmt_list);

  /// @brief コンストラクタ(通常のブロック用)
  /// @param[in] parent 親のブロック
  /// @param[in] stmt_list ステートメントのリスト
  DfgBlock(DfgBlock* parent,
	   const vector<const VlStmt*>& stmt_list);

  /// @brief デストラクタ
  ~DfgBlock();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のブロックを返す．
  DfgBlock*
  parent_block() const;

  /// @brief ステートメント数を返す．
  ymuint
  stmt_num() const;

  /// @brief 対応するステートメントを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < stmt_num() )
  const VlStmt*
  stmt(ymuint pos) const;

  /// @brief 宣言要素に対するインスタンス番号を返す．
  /// @param[in] decl 宣言要素
  /// @return decl の最後の代入のインスタンス番号
  /// @note このブロックで代入されていないときには 0 を返す．
  ymuint
  last_id(const VlDecl* decl) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素に新しいインスタンス番号を割り当てる．
  /// @param[in] decl 宣言要素
  ymuint
  new_id(const VlDecl* decl);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SSA用のマネージャ
  SsaMgr& mSsaMgr;

  // 親のブロック
  DfgBlock* mParent;

  // ステートメントのリスト
  vector<const VlStmt*> mStmtList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 親のブロックを返す．
inline
DfgBlock*
DfgBlock::parent_block() const
{
  return mParent;
}

// @brief ステートメント数を返す．
inline
ymuint
DfgBlock::stmt_num() const
{
  return mStmtList.size();
}

// @brief 対応するステートメントを返す．
// @param[in] pos 位置番号 ( 0 <= pos < stmt_num() )
inline
const VlStmt*
DfgBlock::stmt(ymuint pos) const
{
  return mStmtList[pos];
}

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_VERILOG_DFGBLOCK_H

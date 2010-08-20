#ifndef VMNODE_H
#define VMNODE_H

/// @file libym_verilog/tests/vlview/VmNode.h
/// @brief VmNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VmNode.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include <QAbstractItemModel>
#include "ym_verilog/verilog.h"
#include "ym_utils/FileRegion.h"
#include "ym_verilog/vl/VlFwd.h"



BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VmNode VmNode.h
/// @brief verilog の構造を表示するためのモデル
//////////////////////////////////////////////////////////////////////
class VmNode
{
  friend class VerilogModel;
  
public:
  
  /// @brief コンストラクタ
  VmNode();

  /// @brief デストラクタ
  virtual
  ~VmNode();
  
  
public:

  /// @brief 親のインデックスを返す．
  QModelIndex
  parent_index() const;
  
  /// @brief 子供の数を返す．
  int
  child_num() const;

  /// @brief 子供を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < row_num() )
  VmNode*
  child(int pos) const;

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const = 0;
  
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが用いる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 子供を追加する．
  void
  add_child(VmNode* node) const;

  /// @brief StmtListNodeを追加する．
  /// @param[in] label ラベル
  /// @param[in] stmt_list ステートメントの配列
  void
  add_child(const QString& label,
	    const vector<const VlStmt*>& stmt_list) const;
  
  /// @brief StmtNodeを追加する．
  /// @param[in] label ラベル
  /// @param[in] stmt ステートメント
  void
  add_child(const QString& label,
	    const VlStmt* stmt) const;
  
  /// @brief ExprListNode を追加する．
  /// @param[in] label ラベル
  /// @param[in] expr_list 式の配列
  void
  add_child(const QString& label,
	    const vector<const VlExpr*>& expr_list) const;
  
  /// @brief ExprNode を追加する．
  /// @param[in] label ラベル
  /// @param[in] expr 式
  void
  add_child(const QString& label,
	    const VlExpr* expr) const;

  /// @brief 文字列型のノードを追加する．
  /// @param[in] label ラベル
  /// @param[in] value 値
  void
  add_child(const QString& label,
	    const QString& value) const;

  /// @brief 文字列型のノードを追加する．
  /// @param[in] label ラベル
  /// @param[in] value 値
  void
  add_child(const QString& label,
	    const string& value) const;

  /// @brief ブール型のノードを追加する．
  /// @param[in] label ラベル
  /// @param[in] value 値
  void
  add_child(const QString& label,
	    bool value) const;

  /// @brief 整数型のノードを追加する．
  /// @param[in] label ラベル
  /// @param[in] value 値
  void
  add_child(const QString& label,
	    int value) const;

  /// @brief 整数型のノードを追加する．
  /// @param[in] label ラベル
  /// @param[in] value 値
  void
  add_child(const QString& label,
	    ymuint value) const;
    

private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const = 0;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のノード
  QModelIndex mParentIndex;
  
  // 子供の配列を作っているかを示すフラグ
  mutable
  bool mExpanded;
  
  // 子供の配列
  mutable
  std::vector<VmNode*> mChildren;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 親のインデックスを返す．
inline
QModelIndex
VmNode::parent_index() const
{
  return mParentIndex;
}
  
// @brief 子供の数を返す．
inline
int
VmNode::child_num() const
{
  if ( !mExpanded ) {
    expand();
    mExpanded = true;
  }
  return mChildren.size();
}

// @brief 子供を返す．
// @param[in] pos 位置番号 ( 0 <= pos < row_num() )
inline
VmNode*
VmNode::child(int pos) const
{
  if ( !mExpanded ) {
    expand();
    mExpanded = true;
  }
  return mChildren[pos];
}

END_NAMESPACE_YM_VERILOG

#endif // VMNODE_H

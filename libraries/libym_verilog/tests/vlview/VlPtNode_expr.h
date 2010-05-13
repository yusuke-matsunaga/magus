#ifndef VLPTNODE_EXPR_H
#define VLPTNODE_EXPR_H

/// @file libym_verilog/tests/vlview/VlPtNode_expr.h
/// @brief VlPtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlPtNode_expr.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VlPtNode.h"
#include <ym_verilog/verilog.h>
#include <ym_verilog/pt/PtP.h>
#include <ym_verilog/pt/PtArray.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ExprListNode VlPtNode_expr.h
/// @brief 式のリストを表すノード
//////////////////////////////////////////////////////////////////////
class ExprListNode :
  public VlPtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] expr_array 式の配列
  ExprListNode(const QString& label,
	       const PtExprArray& expr_array);

  /// @brief デストラクタ
  virtual
  ~ExprListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ラベル
  QString mLabel;
  
  // 式の配列
  PtExprArray mExprArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class ExprNode VlPtNode_expr.h
/// @brief 式を表すノード
//////////////////////////////////////////////////////////////////////
class ExprNode :
  public VlPtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] expr 式
  ExprNode(const QString& label,
	   const PtExpr* expr);

  /// @brief デストラクタ
  virtual
  ~ExprNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ラベル
  QString mLabel;

  // 式
  const PtExpr* mExpr;
  
};

END_NAMESPACE_YM_VERILOG

#endif // VLPTNODE_EXPR_H

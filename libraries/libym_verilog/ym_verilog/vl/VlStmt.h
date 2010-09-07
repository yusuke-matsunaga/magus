#ifndef YM_VERILOG_VL_VLSTMT_H
#define YM_VERILOG_VL_VLSTMT_H

/// @file ym_verilog/vl/VlStmt.h
/// @brief VlStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlStmt.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlObj.h"
#include "ym_verilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlStmt VlStmt.h "VlStmt.h"
/// @brief エラボレーション中の statement を表す基底クラス
/// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
/// IEEE Std 1364-2001 26.6.28 Assignment
/// IEEE Std 1364-2001 26.6.29 Delay control
/// IEEE Std 1364-2001 26.6.30 Event control
/// IEEE Std 1364-2001 26.6.31 Repeat control
/// IEEE Std 1364-2001 26.6.32 While, repeat, wait
/// IEEE Std 1364-2001 26.6.33 For
/// IEEE Std 1364-2001 26.6.34 Forever
/// IEEE Std 1364-2001 26.6.35 If, if-else
/// IEEE Std 1364-2001 26.6.36 Case
/// IEEE Std 1364-2001 26.6.37 Assign statement, deassign, force, release
/// IEEE Std 1364-2001 26.6.38 Disable
//////////////////////////////////////////////////////////////////////
class VlStmt :
  public VlObj
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlStmt() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  virtual
  const VlNamedObj*
  parent() const = 0;
  
  /// @brief 対象のスコープの取得
  /// @note kDisable/kParBlock/kSeqBlock で意味のある関数
  virtual
  const VlNamedObj*
  scope() const = 0;

  /// @brief task の実体を返す．
  /// @note kEnable で意味のある関数
  virtual
  const VlTaskFunc*
  task() const = 0;

  /// @brief user systf クラスへのポインタを返す．
  /// @note kSysEnable で意味のある関数
  virtual
  const VlUserSystf*
  user_systf() const = 0;

  /// @brief 引数の数の取得
  /// @note kEnable/kSysEnable で意味のある関数
  virtual
  ymuint32
  arg_num() const = 0;
  
  /// @brief 引数の取得
  /// @param[in] pos 位置 (0 <= pos < arg_num())
  /// @note kEnable/kSysEnable で意味のある関数
  virtual
  const VlExpr*
  arg(ymuint32 pos) const = 0;

  /// @brief control の取得
  /// @note kDc/kEc/kAssignment で意味のある関数
  virtual
  const VlControl*
  control() const = 0;

  /// @brief 本体のステートメントの取得
  /// @note kDc/kEc/kWait/kForever/kRepeat/kWhile/kFor/kIf で意味のある関数
  virtual
  const VlStmt*
  body_stmt() const = 0;

  /// @brief 式の取得
  /// @note kWait/kRepeat/kWhile/kFor/kIf/kCase/kCaseX/kCaseZ で意味のある関数
  virtual
  const VlExpr*
  expr() const = 0;
  
  /// @brief 代入のブロッキング/ノンブロッキングの区別の取得
  /// @retval true ブロッキング代入文を表す．
  /// @retval false ノンブロッキング代入文もしくは他のステートメント
  /// @note 代入文のときに意味を持つ．
  virtual
  bool
  is_blocking() const = 0;
  
  /// @brief 左辺式の取得
  /// @note kAssign/kForce/kPcAssign/kRelease/kDeassign で意味のある関数
  virtual
  const VlExpr*
  lhs() const = 0;

  /// @brief 右辺式の取得
  /// @note kAssign/kForce/kPcAssign で意味のある関数
  virtual
  const VlExpr*
  rhs() const = 0;

  /// @brief イベントプライマリの取得
  /// @note kEvent で意味のある関数
  virtual
  const VlExpr*
  named_event() const = 0;

  /// @brief 条件が成り立たなかったとき実行されるステートメントの取得
  /// @note kIf で意味のある関数
  virtual
  const VlStmt*
  else_stmt() const = 0;
  
  /// @brief case type の取得
  /// @return case type
  /// @note kCase/kCaseX/kCaseZ で意味のある関数
  virtual
  tVpiCaseType
  case_type() const = 0;
  
  /// @brief case item の要素数の取得
  /// @return case item の要素数
  /// @note kCase/kCaseX/kCaseZ で意味のある関数
  virtual
  ymuint32
  caseitem_num() const = 0;

  /// @brief case item の取得
  /// @param[in] pos 位置番号 (0 <= pos < caseitem_num())
  /// @note kCase/kCaseX/kCaseZ で意味のある関数
  virtual
  const VlCaseItem*
  caseitem(ymuint32 pos) const = 0;

  /// @brief 初期化代入文の取得
  /// @note kFor で意味のある関数
  virtual
  const VlStmt*
  init_stmt() const = 0;
  
  /// @brief 繰り返し代入文の取得
  /// @note kFor で意味のある関数
  virtual
  const VlStmt*
  inc_stmt() const = 0;

  /// @brief 子供のステートメントの数の取得
  /// @note kParBlock/kSeqBlock で意味のある関数
  virtual
  ymuint32
  child_stmt_num() const = 0;
  
  /// @brief 子供のステートメントの取得
  /// @param[in] pos 位置番号 (0 <= pos < stmt_num())
  /// @note kParBlock/kSeqBlock で意味のある関数
  virtual
  const VlStmt*
  child_stmt(ymuint32 pos) const = 0;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VlCaseItem VlStmt.h "VlStmt.h"
/// @brief caes item を表すクラス
/// IEEE Std 1364-2001 26.6.36 Case
//////////////////////////////////////////////////////////////////////
class VlCaseItem :
  public VlObj
{
protected:

  /// @brief デストラクタ
  ~VlCaseItem() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlCaseItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件式のリストの要素数を返す．
  virtual
  ymuint32
  expr_num() const = 0;

  /// @brief 条件式を返す．
  /// @param[in] pos 位置番号
  virtual
  const VlExpr*
  expr(ymuint32 pos) const = 0;
  
  /// @brief 本体のステートメントを返す．
  virtual
  const VlStmt*
  body_stmt() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VL_VLSTMT_H

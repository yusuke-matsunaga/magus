#ifndef LIBYM_VERILOG_ELB_ELBSTMT_H
#define LIBYM_VERILOG_ELB_ELBSTMT_H

/// @file libym_verilog/elaborator/include/ElbStmt.h
/// @brief ElbStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbStmt.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/vl/VlStmt.h>
#include <ym_verilog/pt/PtP.h>
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbStmt ElbStmt.h "ElbStmt.h"
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
class ElbStmt :
  public VlStmt
{
protected:
  
  /// @brief コンストラクタ
  ElbStmt() : mNext(NULL) { }
  
  /// @brief デストラクタ
  virtual
  ~ElbStmt() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 対象のスコープの取得
  /// @note kDisable/kParBlock/kSeqBlock で意味のある関数
  /// @note このクラスでは NULL を返す．
  virtual
  const VlNamedObj*
  scope() const;

  /// @brief task の実体を返す．
  /// @note kEnable で意味のある関数
  /// @note このクラスでは NULL を返す．
  virtual
  const VlTask*
  task() const;

  /// @brief user systf クラスへのポインタを返す．
  /// @note kSysEnable で意味のある関数
  /// @note このクラスでは NULL を返す．
  virtual
  const VlUserSystf*
  user_systf() const;

  /// @brief 引数の数の取得
  /// @note kEnable/kSysEnable で意味のある関数
  /// @note このクラスでは 0 を返す．
  virtual
  ymuint32
  arg_num() const;
  
  /// @brief 引数の取得
  /// @param[in] pos 位置 (0 <= pos < arg_num())
  /// @note kEnable/kSysEnable で意味のある関数
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  arg(ymuint32 pos) const;

  /// @brief control の取得
  /// @note このクラスでは NULL を返す．
  virtual
  const VlControl*
  control() const;
  
  /// @brief 本体のステートメントの取得
  /// @note kDc/kEc/kWait/kForever/kRepeat/kWhile/kFor/kIf で意味のある関数
  /// @note このクラスでは NULL を返す．
  virtual
  const VlStmt*
  body_stmt() const;
  
  /// @brief 式の取得
  /// @note kWait/kRepeat/kWhile/kFor/kIf/kCase/kCaseX/kCaseZ で意味のある関数
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  expr() const;

  /// @brief 左辺式の取得
  /// @note kAssign/kForce/kPcAssign/kRelease/kDeassign で意味のある関数
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  lhs() const;

  /// @brief 右辺式の取得
  /// @note kAssign/kForce/kPcAssign で意味のある関数
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  rhs() const;
  
  /// @brief 代入のブロッキング/ノンブロッキングの区別の取得
  /// @note このクラスでは false を返す．
  virtual
  bool
  is_blocking() const;

  /// @brief イベントプライマリの取得
  /// @note kEvent で意味のある関数
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  named_event() const;

  /// @brief 条件が成り立たなかったとき実行されるステートメントの取得
  /// @note kIf で意味のある関数
  /// @note このクラスでは NULL を返す．
  virtual
  const VlStmt*
  else_stmt() const;
  
  /// @brief case type の取得
  /// @return case type
  /// @note kCase/kCaseX/kCaseZ で意味のある関数
  /// @note このクラスでは kVpiCaseExact を返す．
  virtual
  tVpiCaseType
  case_type() const;

  /// @brief case item の要素数の取得
  /// @return case item の要素数
  /// @note kCase/kCaseX/kCaseZ で意味のある関数
  /// @note このクラスでは 0 を返す．
  virtual
  ymuint32
  caseitem_num() const;

  /// @brief case item の取得
  /// @param[in] pos 位置番号 (0 <= pos < caseitem_num())
  /// @note kCase/kCaseX/kCaseZ で意味のある関数
  /// @note このクラスでは NULL を返す．
  virtual
  const VlCaseItem*
  caseitem(ymuint32 pos) const;

  /// @brief 初期化代入文の取得
  /// @note kFor で意味のある関数
  /// @note このクラスでは NULL を返す．
  virtual
  const VlStmt*
  init_stmt() const;
  
  /// @brief 繰り返し代入文の取得
  /// @note kFor で意味のある関数
  /// @note このクラスでは NULL を返す．
  virtual
  const VlStmt*
  inc_stmt() const;

  /// @brief 子供のステートメントの数の取得
  /// @note kParBlock/kSeqBlock で意味のある関数
  /// @note このクラスでは 0 を返す．
  virtual
  ymuint32
  child_stmt_num() const;
  
  /// @brief 子供のステートメントの取得
  /// @param[in] pos 位置番号 (0 <= pos < stmt_num())
  /// @note kParBlock/kSeqBlock で意味のある関数
  virtual
  const VlStmt*
  child_stmt(ymuint32 pos) const;
  

public:
  //////////////////////////////////////////////////////////////////////
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief caseitem を設定する．
  /// @param[in] pos 位置番号
  /// @param[in] pt_caseitem パース木の caseitem 定義
  /// @param[in] expr_array 条件式を納める配列
  /// @param[in] stmt 本体のステートメント
  /// @note このクラスでは何もしない．
  virtual
  void
  set_caseitem(ymuint32 pos,
	       const PtCaseItem* pt_caseitem,
	       ElbExpr** expr_array,
	       ElbStmt* stmt);

  /// @brief function 中の実行を行う．
  virtual
  const VlNamedObj*
  func_exec(bool constant_function) const = 0;
  
  /// @brief 子供ののステートメントの取得
  /// @note このクラスでは NULL を返す．
  virtual
  ElbStmt*
  _child_stmt(ymuint32 pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すリンク
  ElbStmt* mNext;
  
};


//////////////////////////////////////////////////////////////////////
/// @class ElbCaseItem ElbStmt.h "ElbStmt.h"
/// @brief caes item を表すクラス
/// IEEE Std 1364-2001 26.6.36 Case
//////////////////////////////////////////////////////////////////////
class ElbCaseItem :
  public VlCaseItem
{
protected:
  
  /// @brief コンストラクタ
  ElbCaseItem() { }
  
  /// @brief デストラクタ
  ~ElbCaseItem() { }

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_ELBSTMT_H

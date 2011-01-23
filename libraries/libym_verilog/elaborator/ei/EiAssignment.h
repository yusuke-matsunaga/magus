#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIASSIGNMENT_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIASSIGNMENT_H

/// @file libym_verilog/elaborator/ei/EiAssignment.h
/// @brief 代入文を表すクラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiAssignment.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// ステートメントを表すクラス
// IEEE Std 1364-2001 26.6.28 Assignment
// IEEE Std 1364-2001 26.6.29 Delay control
// IEEE Std 1364-2001 26.6.30 Event control
// IEEE Std 1364-2001 26.6.37 Assign statement, deassign, force, release


#include "EiStmt.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiAssignBase EiAssignment.h "EiAssignment.h"
/// @brief 代入文の基底クラス
//////////////////////////////////////////////////////////////////////
class EiAssignBase :
  public EiStmtBase
{
protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  /// @param[in] rhs 右辺の式
  EiAssignBase(const VlNamedObj* parent,
	       ElbProcess* process,
	       const PtStmt* pt_stmt,
	       ElbLhs* lhs,
	       ElbExpr* rhs);

  /// @brief デストラクタ
  virtual
  ~EiAssignBase();


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 左辺を返す．
  virtual
  const VlExpr*
  lhs() const;

  /// @brief 左辺式の要素数の取得
  /// @note 通常は1だが，連結演算子の場合はその子供の数となる．
  /// @note ただし，連結演算の入れ子はすべて平坦化して考える．
  virtual
  ymuint
  lhs_elem_num() const;

  /// @brief 左辺式の要素の取得
  /// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
  /// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
  virtual
  const VlExpr*
  lhs_elem(ymuint pos) const;

  /// @brief 右辺を返す．
  virtual
  const VlExpr*
  rhs() const;


protected:

  /// @brief 左辺を返す．
  ElbExpr*
  _lhs() const;

  /// @brief 右辺を返す．
  ElbExpr*
  _rhs() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 左辺
  ElbLhs* mLhs;

  // 右辺
  ElbExpr* mRhs;

};


//////////////////////////////////////////////////////////////////////
/// @class EiNbAssignment EiAssignment.h "EiAssignment.h"
/// @brief non-blocking assignment を表すクラス
/// IEEE Std 1364-2001 26.6.28 Assignment
//////////////////////////////////////////////////////////////////////
class EiNbAssignment :
  public EiAssignBase
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  /// @param[in] rhs 右辺の式
  /// @param[in] control コントロール
  EiNbAssignment(const VlNamedObj* parent,
		 ElbProcess* process,
		 const PtStmt* pt_stmt,
		 ElbLhs* lhs,
		 ElbExpr* rhs,
		 ElbControl* control);

  /// @brief デストラクタ
  virtual
  ~EiNbAssignment();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief control を返す．NULL の場合もありうる．
  virtual
  const VlControl*
  control() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // コントロール
  ElbControl* mControl;

};


//////////////////////////////////////////////////////////////////////
/// @class EiAssignment EiAssignment.h "EiAssignment.h"
/// @brief blocking assignment を表すクラス
/// IEEE Std 1364-2001 26.6.28 Assignment
//////////////////////////////////////////////////////////////////////
class EiAssignment :
  public EiNbAssignment
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  /// @param[in] rhs 右辺の式
  /// @param[in] control コントロール
  EiAssignment(const VlNamedObj* parent,
	       ElbProcess* process,
	       const PtStmt* pt_stmt,
	       ElbLhs* lhs,
	       ElbExpr* rhs,
	       ElbControl* control);

  /// @brief デストラクタ
  virtual
  ~EiAssignment();


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 代入のブロッキング/ノンブロッキングの区別の取得
  /// @note このクラスでは true を返す．
  virtual
  bool
  is_blocking() const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiAssignStmt EiAssignment.h "EiAssignment.h"
/// @brief assign statement を表すクラス
/// assignment とは別物
/// IEEE Std 1364-2001 26.6.37 Assign statement, deassign, force, release
//////////////////////////////////////////////////////////////////////
class EiAssignStmt :
  public EiAssignBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  /// @param[in] rhs 右辺の式
  EiAssignStmt(const VlNamedObj* parent,
	       ElbProcess* process,
	       const PtStmt* pt_stmt,
	       ElbLhs* lhs,
	       ElbExpr* rhs);

  /// @brief デストラクタ
  virtual
  ~EiAssignStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiForceStmt EiAssignment.h "EiAssignment.h"
/// @brief force statement を表すクラス
/// IEEE Std 1364-2001 26.6.37 Assign statement, deassign, force, release
//////////////////////////////////////////////////////////////////////
class EiForceStmt :
  public EiAssignBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  /// @param[in] rhs 右辺の式
  EiForceStmt(const VlNamedObj* parent,
	      ElbProcess* process,
	      const PtStmt* pt_stmt,
	      ElbLhs* lhs,
	      ElbExpr* rhs);

  /// @brief デストラクタ
  virtual
  ~EiForceStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbDeassignBase EiAssignment.h "EiAssignment.h"
/// @brief deassign/release の基底クラス
//////////////////////////////////////////////////////////////////////
class EiDeassignBase :
  public EiStmtBase
{
protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  EiDeassignBase(const VlNamedObj* parent,
		 ElbProcess* process,
		 const PtStmt* pt_stmt,
		 ElbLhs* lhs);

  /// @brief デストラクタ
  virtual
  ~EiDeassignBase();


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 左辺を返す．
  virtual
  const VlExpr*
  lhs() const;

  /// @brief 左辺式の要素数の取得
  /// @note 通常は1だが，連結演算子の場合はその子供の数となる．
  /// @note ただし，連結演算の入れ子はすべて平坦化して考える．
  virtual
  ymuint
  lhs_elem_num() const;

  /// @brief 左辺式の要素の取得
  /// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
  /// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
  virtual
  const VlExpr*
  lhs_elem(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 左辺
  ElbLhs* mLhs;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeassignStmt EiAssignment.h "EiAssignment.h"
/// @brief deassign statement を表すクラス
/// IEEE Std 1364-2001 26.6.37 Assign statement, deassign, force, release
//////////////////////////////////////////////////////////////////////
class EiDeassignStmt :
  public EiDeassignBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  EiDeassignStmt(const VlNamedObj* parent,
		 ElbProcess* process,
		 const PtStmt* pt_stmt,
		 ElbLhs* lhs);

  /// @brief デストラクタ
  virtual
  ~EiDeassignStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiReleaseStmt EiAssignment.h "EiAssignment.h"
/// @brief release statement を表すクラス
/// IEEE Std 1364-2001 26.6.37 Assign statement, deassign, force, release
//////////////////////////////////////////////////////////////////////
class EiReleaseStmt :
  public EiDeassignBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or NULL)
  /// @param[in] pt_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  EiReleaseStmt(const VlNamedObj* parent,
		ElbProcess* process,
		const PtStmt* pt_stmt,
		ElbLhs* lhs);

  /// @brief デストラクタ
  virtual
  ~EiReleaseStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIASSIGNMENT_H

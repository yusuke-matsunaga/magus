#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIPRIMARY_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIPRIMARY_H

/// @file libym_verilog/elaborator/ei/EiPrimary.h
/// @brief EiPrimary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiPrimary.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiPrimary EiPrimary.h "EiPrimary.h"
/// @brief プライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiPrimary :
  public EiExprBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  EiPrimary(const PtExpr* pt_expr,
	    ElbDecl* obj);

  /// @brief デストラクタ
  virtual
  ~EiPrimary();


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
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  virtual
  tVpiValueType
  value_type() const;

  /// @brief 定数の時 true を返す．
  /// @note 参照している要素の型によって決まる．
  virtual
  bool
  is_const() const;

  /// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
  virtual
  bool
  is_primary() const;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  /// @note 宣言要素に対するビット選択，部分選択の場合にも意味を持つ．
  virtual
  const VlDecl*
  decl_obj() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  virtual
  void
  set_reqsize(tVpiValueType type);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の宣言要素
  ElbDecl* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclPrimary EiPrimary.h "EiPrimary.h"
/// @brief プライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclPrimary :
  public EiExpr
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_item パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  EiDeclPrimary(const PtDeclItem* pt_item,
		ElbDecl* obj);

  /// @brief デストラクタ
  virtual
  ~EiDeclPrimary();


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
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  virtual
  tVpiValueType
  value_type() const;

  /// @brief 定数の時 true を返す．
  /// @note 参照している要素の型によって決まる．
  virtual
  bool
  is_const() const;

  /// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
  virtual
  bool
  is_primary() const;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  /// @note 宣言要素に対するビット選択，部分選択の場合にも意味を持つ．
  virtual
  const VlDecl*
  decl_obj() const;

  /// @brief Verilog-HDL の文字列を得る．
  virtual
  string
  decompile() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  virtual
  void
  set_reqsize(tVpiValueType type);


private:
  //////////////////////////////////////////////////////////////////////
  // EiExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パース木の定義要素を返す．
  virtual
  const PtBase*
  pt_obj() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の定義要素
  const PtDeclItem* mPtObj;

  // 対象の宣言要素
  ElbDecl* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class EiParamPrimary EiPrimary.h "EiPrimary.h"
/// @brief パラメータ用のプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiParamPrimary :
  public EiExprBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  EiParamPrimary(const PtExpr* pt_expr,
		 ElbParameter* obj);

  /// @brief デストラクタ
  virtual
  ~EiParamPrimary();


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
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  virtual
  tVpiValueType
  value_type() const;

  /// @brief 定数の時 true を返す．
  /// @note 参照している要素の型によって決まる．
  virtual
  bool
  is_const() const;

  /// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
  virtual
  bool
  is_primary() const;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  /// @note 宣言要素に対するビット選択，部分選択の場合にも意味を持つ．
  virtual
  const VlDecl*
  decl_obj() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  virtual
  void
  set_reqsize(tVpiValueType type);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の宣言要素
  ElbParameter* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class EiArrayElemPrimary EiPrimary.h "EiPrimary.h"
/// @brief 配列要素のプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiArrayElemPrimary :
  public EiExprBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  /// @param[in] dim 配列の次元
  /// @param[in] index_list インデックスのリスト
  EiArrayElemPrimary(const PtExpr* pt_expr,
		     ElbDeclArray* obj,
		     ymuint dim,
		     ElbExpr** index_list);

  /// @brief デストラクタ
  virtual
  ~EiArrayElemPrimary();


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
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  virtual
  tVpiValueType
  value_type() const;

  /// @brief 定数の時 true を返す．
  /// @note 参照している要素の型によって決まる．
  virtual
  bool
  is_const() const;

  /// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
  virtual
  bool
  is_primary() const;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  /// @note 宣言要素に対するビット選択，部分選択の場合にも意味を持つ．
  virtual
  const VlDeclArray*
  declarray_obj() const;

  /// @brief 配列型宣言要素への参照の場合，配列の次元を返す．
  /// @note それ以外では 0 を返す．
  virtual
  ymuint
  declarray_dimension() const;

  /// @brief 配列型宣言要素への参照の場合，配列のインデックスを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < declarray_dimension() )
  /// @note それ以外では NULL を返す．
  virtual
  const VlExpr*
  declarray_index(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  virtual
  void
  set_reqsize(tVpiValueType type);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の宣言要素
  ElbDeclArray* mObj;

  // 配列の次元
  ymuint32 mDim;

  // インデックスのリスト
  ElbExpr** mIndexList;

};


//////////////////////////////////////////////////////////////////////
/// @class EiScopePrimary EiPrimary.h "EiPrimary.h"
/// @brief VlNamedObj のプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiScopePrimary :
  public EiExprBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  EiScopePrimary(const PtExpr* pt_expr,
		 const VlNamedObj* obj);

  /// @brief デストラクタ
  virtual
  ~EiScopePrimary();


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
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  virtual
  tVpiValueType
  value_type() const;

  /// @brief 定数の時 true を返す．
  /// @note 参照している要素の型によって決まる．
  virtual
  bool
  is_const() const;

  /// @brief 対象のオブジェクトを返す．
  virtual
  const VlNamedObj*
  scope_obj() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  virtual
  void
  set_reqsize(tVpiValueType type);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の宣言要素
  const VlNamedObj* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPrimitivePrimary EiPrimary.h "EiPrimary.h"
/// @brief VlPrimitive のプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiPrimitivePrimary :
  public EiExprBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] obj 本体のオブジェクト
  EiPrimitivePrimary(const PtExpr* pt_expr,
		     ElbPrimitive* obj);

  /// @brief デストラクタ
  virtual
  ~EiPrimitivePrimary();


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
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  virtual
  tVpiValueType
  value_type() const;

  /// @brief 定数の時 true を返す．
  /// @note 参照している要素の型によって決まる．
  virtual
  bool
  is_const() const;

  /// @brief 対象のオブジェクトを返す．
  virtual
  const VlPrimitive*
  primitive_obj() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  virtual
  void
  set_reqsize(tVpiValueType type);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の宣言要素
  ElbPrimitive* mObj;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIPRIMARY_H

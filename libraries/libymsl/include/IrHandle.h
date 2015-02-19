#ifndef IRHANDLE_H
#define IRHANDLE_H

/// @file IrHandle.h
/// @brief IrHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrHandle IrHandle.h "IrHandle.h"
/// @brief Symbol Table のハンドル
//////////////////////////////////////////////////////////////////////
class IrHandle
{
  friend class Scope;

public:
  //////////////////////////////////////////////////////////////////////
  // 型定義
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を表す列挙型
  enum HandleType {
    kScope,
    kVar,
    kFunction,
    kBooleanConst,
    kIntConst,
    kFloatConst,
    kStringConst,
    kLabel,
    kNamedType,
    kArrayRef,
    kMemberRef,
    kMethodRef
  };


public:

  /// @brief コンストラクタ
  IrHandle();

  /// @brief デストラクタ
  virtual
  ~IrHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  ShString
  name() const = 0;

  /// @brief 種類を返す．
  virtual
  HandleType
  handle_type() const = 0;

  /// @brief 静的評価可能か調べる．
  ///
  /// 要するに定数式かどうかということ
  virtual
  bool
  is_static() const = 0;

  /// @brief スコープを返す．
  ///
  /// kScope のみ有効
  virtual
  Scope*
  scope() const;

  /// @brief 変数を返す．
  ///
  /// kVar, kMemberRef のみ有効
  virtual
  const IrVar*
  var() const;

  /// @brief 関数を返す．
  ///
  /// kFunction, kMethodRef のみ有効
  virtual
  IrFuncBlock*
  function() const;

  /// @brief ブール値を返す．
  ///
  /// kBooleanConst のみ有効
  virtual
  Ymsl_BOOLEAN
  boolean_val() const;

  /// @brief 整数値を返す．
  ///
  /// kIntConst のみ有効
  virtual
  Ymsl_INT
  int_val() const;

  /// @brief 実数値を返す．
  ///
  /// kFloatConst のみ有効
  virtual
  Ymsl_FLOAT
  float_val() const;

  /// @brief 文字列値を返す．
  ///
  /// kStringConst のみ有効
  virtual
  const char*
  string_val() const;

  /// @brief ラベルを返す．
  ///
  /// kLabel のみ有効
  virtual
  IrNode*
  label() const;

  /// @brief 名前付き型を返す．
  ///
  /// kNamedType のみ有効
  virtual
  const Type*
  named_type() const;

  /// @brief 配列本体の式を返す．
  ///
  /// kArrayRef のみ有効
  virtual
  IrNode*
  array_expr() const;

  /// @brief 配列のインデックスを返す．
  ///
  /// kArrayRef のみ有効
  virtual
  IrNode*
  array_index() const;

  /// @brief オブジェクトを指す式を返す．
  ///
  /// kMemberRef, kMethodRef のみ有効
  virtual
  IrNode*
  obj_expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すリンク
  IrHandle* mLink;

};

END_NAMESPACE_YM_YMSL

#endif // IRHANDLE_H

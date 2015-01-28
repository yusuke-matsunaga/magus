#ifndef SYMHANDLE_H
#define SYMHANDLE_H

/// @file SymHandle.h
/// @brief SymHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class SymHandle SymHandle.h "SymHandle.h"
/// @brief Symbol Table のハンドル
//////////////////////////////////////////////////////////////////////
class SymHandle
{
  friend class Scope;

public:

  /// @brief コンストラクタ
  SymHandle();

  /// @brief デストラクタ
  virtual
  ~SymHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  ShString
  name() const = 0;

  /// @brief スコープを返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  Scope*
  scope() const;

  /// @brief 変数宣言を返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  const Var*
  var() const;

  /// @brief 関数を返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  const Function*
  function() const;

  /// @brief ラベルを返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  const Label*
  label() const;

  /// @brief 名前付き型を返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  const Type*
  named_type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すリンク
  SymHandle* mLink;

};

END_NAMESPACE_YM_YMSL

#endif // SYMHANDLE_H

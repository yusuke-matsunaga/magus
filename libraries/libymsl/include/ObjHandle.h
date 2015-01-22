#ifndef OBJHANDLE_H
#define OBJHANDLE_H

/// @file ObjHandle.h
/// @brief ObjHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class ObjHandle ObjHandle.h "ObjHandle.h"
/// @brief Symbol Table のハンドル
//////////////////////////////////////////////////////////////////////
class ObjHandle
{
  friend class YmslScope;

public:

  /// @brief コンストラクタ
  ObjHandle();

  /// @brief デストラクタ
  virtual
  ~ObjHandle();


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
  YmslScope*
  scope() const;

  /// @brief 変数宣言を返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  YmslVar*
  var() const;

  /// @brief 関数を返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  YmslFunction*
  function() const;

  /// @brief ラベルを返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  YmslLabel*
  label() const;

  /// @brief 名前付き型を返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  YmslType*
  named_type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すリンク
  ObjHandle* mLink;

};

END_NAMESPACE_YM_YMSL

#endif // OBJHANDLE_H

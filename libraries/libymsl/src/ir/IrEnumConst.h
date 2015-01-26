#ifndef IRENUMCONST_H
#define IRENUMCONST_H

/// @file IrEnumConst.h
/// @brief IrEnumConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrEnumConst IrEnumConst.h "IrEnumConst.h"
/// @brief enum 定数を表すクラス
//////////////////////////////////////////////////////////////////////
class IrEnumConst
{
  friend class IrEnumType;

public:

  /// @brief コンストラクタ
  IrEnumConst();

  /// @brief デストラクタ
  ~IrEnumConst();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 親の enum 型を返す．
  IrType*
  parent() const;

  /// @brief 名前を返す．
  ShString
  name() const;

  /// @brief 値を返す．
  int
  val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の enum 型
  IrType* mParent;

  // 名前
  ShString mName;

  // 値
  int mVal;

};

END_NAMESPACE_YM_YMSL

#endif // IRENUMCONST_H

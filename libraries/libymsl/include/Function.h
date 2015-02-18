#ifndef FUNCTION_H
#define FUNCTION_H

/// @file Function.h
/// @brief Function のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class Function Function.h "Function.h"
/// @brief YMSL の関数を表すクラス
//////////////////////////////////////////////////////////////////////
class Function
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 関数名
  /// @param[in] type 関数の型
  Function(ShString name,
	   const Type* type);

  /// @brief デストラクタ
  virtual
  ~Function();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数名を返す．
  ShString
  name() const;

  /// @brief 関数の型を返す．
  const Type*
  type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数名
  ShString mName;

  // 関数の型
  const Type* mType;

};

END_NAMESPACE_YM_YMSL

#endif // FUNCTION_H

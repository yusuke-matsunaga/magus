#ifndef YMSLUSERTYPE_H
#define YMSLUSERTYPE_H

/// @file YmslUserType.h
/// @brief YmslUserType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslUserType YmslUserType.h "YmslUserType.h"
/// @brief ユーザー定義型を表す YmslType
//////////////////////////////////////////////////////////////////////
class YmslUserType :
  public YmslType
{
public:

  /// @brief コンストラクタ
  /// @param[in] type_name 型名
  YmslUserType(string type_name);

  /// @brief デストラクタ
  virtual
  ~YmslUserType();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  TypeId
  type() const;

  /// @brief 値を表す文字列を返す．
  virtual
  string
  str() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型名
  string mTypeName;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLUSERTYPE_H

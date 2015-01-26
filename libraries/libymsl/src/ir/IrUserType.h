#ifndef YMSLUSERTYPE_H
#define YMSLUSERTYPE_H

/// @file IrUserType.h
/// @brief IrUserType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrUserType IrUserType.h "IrUserType.h"
/// @brief ユーザー定義型を表す IrType
//////////////////////////////////////////////////////////////////////
class IrUserType :
  public IrType
{
public:

  /// @brief コンストラクタ
  /// @param[in] type_name 型名
  IrUserType(string type_name);

  /// @brief デストラクタ
  virtual
  ~IrUserType();


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

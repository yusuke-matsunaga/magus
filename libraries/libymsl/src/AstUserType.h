#ifndef ASTUSERTYPE_H
#define ASTUSERTYPE_H

/// @file AstUserType.h
/// @brief AstUserType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstValueType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstUserType AstUserType.h "AstUserType.h"
/// @brief ユーザー定義型を表す Ast
//////////////////////////////////////////////////////////////////////
class AstUserType :
  public AstValueType
{
public:

  /// @brief コンストラクタ
  /// @param[in] type_name 型名
  /// @param[in] loc ファイル位置
  AstUserType(AstSymbol* type_name);

  /// @brief デストラクタ
  virtual
  ~AstUserType();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  AstType
  type() const;

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  /// @param[in] indent インデントレベル
  virtual
  void
  print(ostream& s,
	ymuint indent = 0) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 形名
  AstSymbol* mTypeName;

};

END_NAMESPACE_YM_YMSL

#endif // ASTUSERTYPE_H

#ifndef YMSLSIMPLETYPE_H
#define YMSLSIMPLETYPE_H

/// @file YmslSimpleType.h
/// @brief YmslSimpleType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslSimpleType YmslType.h "YmslType.h"
/// @brief 単純な型を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslSimpleType :
  public YmslType
{
public:

  /// @brief コンストラクタ
  /// @param[in] type 型
  YmslSimpleType(TypeId type);


  /// @brief デストラクタ
  virtual
  ~YmslSimpleType();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  TypeId
  type_id() const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  virtual
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型
  TypeId mType;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLSIMPLETYPE_H

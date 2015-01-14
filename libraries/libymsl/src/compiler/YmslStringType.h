#ifndef YMSLSTRINGTYPE_H
#define YMSLSTRINGTYPE_H

/// @file YmslStringType.h
/// @brief YmslStringType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslStringType YmslStringType.h "YmslStringType.h"
/// @brief string 型を表す YmslType
//////////////////////////////////////////////////////////////////////
class YmslStringType :
  public YmslType
{
public:

  /// @brief コンストラクタ
  YmslStringType();

  /// @brief デストラクタ
  virtual
  ~YmslStringType();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  TypeId
  type() const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  virtual
  void
  print(ostream& s) const;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLASTSTRINGTYPE_H

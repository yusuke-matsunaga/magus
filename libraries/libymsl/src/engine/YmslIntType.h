#ifndef YMSLINTTYPE_H
#define YMSLINTTYPE_H

/// @file YmslIntType.h
/// @brief YmslIntType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslIntType YmslIntType.h "YmslIntType.h"
/// @brief int 型を表す YmslType
//////////////////////////////////////////////////////////////////////
class YmslIntType :
  public YmslType
{
public:

  /// @brief コンストラクタ
  YmslIntType();

  /// @brief デストラクタ
  virtual
  ~YmslIntType();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  TypeId
  type_id() const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  virtual
  void
  print(ostream& s) const;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLINTTYPE_H

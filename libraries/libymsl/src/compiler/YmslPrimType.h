#ifndef YMSLPRIMTYPE_H
#define YMSLPRIMTYPE_H

/// @file YmslPrimType.h
/// @brief YmslPrimType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslPrimType YmslType.h "YmslType.h"
/// @brief 単純な型を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslPrimType :
  public YmslType
{
public:

  /// @brief コンストラクタ
  /// @param[in] type 型
  YmslPrimType(TypeId type);


  /// @brief デストラクタ
  virtual
  ~YmslPrimType();


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

#endif // YMSLPRIMTYPE_H

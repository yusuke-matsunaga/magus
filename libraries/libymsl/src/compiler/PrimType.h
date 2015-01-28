#ifndef PRIMTYPE_H
#define PRIMTYPE_H

/// @file PrimType.h
/// @brief PrimType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "Type.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class PrimType Type.h "Type.h"
/// @brief 単純な型を表すクラス
//////////////////////////////////////////////////////////////////////
class PrimType :
  public Type
{
public:

  /// @brief コンストラクタ
  /// @param[in] type 型
  PrimType(TypeId type);


  /// @brief デストラクタ
  virtual
  ~PrimType();


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

#endif // PRIMTYPE_H

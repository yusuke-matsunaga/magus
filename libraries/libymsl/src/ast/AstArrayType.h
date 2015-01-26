#ifndef ASTARRAYTYPE_H
#define ASTARRAYTYPE_H

/// @file AstArrayType.h
/// @brief AstArrayType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstArrayType AstArrayType.h "AstArrayType.h"
/// @brief array 型を表す AstType
//////////////////////////////////////////////////////////////////////
class AstArrayType :
  public AstType
{
public:

  /// @brief コンストラクタ
  /// @param[in] elem_type 要素の型
  /// @param[in] loc ファイル位置
  AstArrayType(AstType* elem_type,
	       const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstArrayType();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  TypeId
  type_id() const;

  /// @brief array/set/map 型の時に要素の型を返す．
  virtual
  const AstType*
  elem_type() const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力ストリーム
  virtual
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素の型
  AstType* mElemType;

};

END_NAMESPACE_YM_YMSL


#endif // ASTARRAYTYPE_H

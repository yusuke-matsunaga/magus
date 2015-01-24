#ifndef ASTMEMBERREF_H
#define ASTMEMBERREF_H

/// @file AstMemberRef.h
/// @brief AstMemberRef のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstLeaf.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstMemberRef AstMemberRef.h "AstMemberRef.h"
/// @brief メンバ参照を表すクラス
//////////////////////////////////////////////////////////////////////
class AstMemberRef :
  public AstLeaf
{
public:

  /// @brief コンストラクタ
  /// @param[in] body 本体の式
  /// @param[in] member メンバ名
  /// @param[in] loc ファイル位置
  AstMemberRef(AstLeaf* body,
	       AstSymbol* member,
	       const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstMemberRef();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  LeafType
  leaf_type() const;

  /// @brief 本体の式を返す．
  ///
  /// kMemberRef, kArrayRef, kFuncCall のみ有効
  virtual
  const AstLeaf*
  body() const;

  /// @brief メンバ名を返す．
  ///
  /// kMemberRef のみ有効
  virtual
  const AstSymbol*
  symbol() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オブジェクト名
  AstLeaf* mBody;

  // メンバ名
  AstSymbol* mMember;

};

END_NAMESPACE_YM_YMSL

#endif // ASTMEMBERREF_H

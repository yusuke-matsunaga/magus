#ifndef ASTBOOLCONST_H
#define ASTBOOLCONST_H

/// @file AstBoolConst.h
/// @brief AstBoolConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstLeaf.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstBoolConst AstBoolConst.h "AstBoolConst.h"
/// @brief true/false を表すクラス
//////////////////////////////////////////////////////////////////////
class AstBoolConst :
  public AstLeaf
{
public:

  /// @brief コンストラクタ
  /// @param[in] leaf_type 式の種類 (kTrue/kFalse)
  /// @param[in] loc ファイル位置
  AstBoolConst(LeafType leaf_type,
	       const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstBoolConst();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  LeafType
  leaf_type() const;

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 式の種類
  LeafType mType;

};

END_NAMESPACE_YM_YMSL

#endif // ASTBOOLCONST_H

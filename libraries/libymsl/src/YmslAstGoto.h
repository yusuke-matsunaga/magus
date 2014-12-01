#ifndef YMSLASTGOTO_H
#define YMSLASTGOTO_H

/// @file YmslAstGoto.h
/// @brief YmslAstGoto のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstGoto YmslAstGoto.h "YmslAstGoto.h"
/// @brief while 文を表す YmslAst
//////////////////////////////////////////////////////////////////////
class YmslAstGoto :
  public YmslAstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] loc ファイル位置
  YmslAstGoto(YmslAst* label,
	      const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstGoto();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  AstType
  type() const;

  /// @brief 子供の数を返す．
  virtual
  ymuint
  child_num() const;

  /// @brief 子供を返す．
  /// @param[in] pos 位置( 0 <= pos < child_num() )
  virtual
  YmslAst*
  child(ymuint pos) const;

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

  // 要素の配列
  YmslAst* mLabel;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLASTGOTO_H

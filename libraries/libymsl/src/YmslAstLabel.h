#ifndef YMSLASTLABEL_H
#define YMSLASTLABEL_H

/// @file YmslAstLabel.h
/// @brief YmslAstLabel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstLabel YmslAstLabel.h "YmslAstLabel.h"
/// @brief ラベルを表す YmslAst
//////////////////////////////////////////////////////////////////////
class YmslAstLabel :
  public YmslAstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] loc ファイル位置
  YmslAstLabel(YmslAst* label,
	       const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstLabel();


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

#endif // YMSLASTLABEL_H

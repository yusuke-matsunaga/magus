#ifndef ASTLIST_H
#define ASTLIST_H

/// @file AstList.h
/// @brief AstList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstList AstList.h "AstList.h"
/// @brief リストを表すクラス
//////////////////////////////////////////////////////////////////////
class AstList
{
public:

  /// @brief コンストラクタ
  AstList();

  /// @brief デストラクタ
  virtual
  ~AstList();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 子供の数を返す．
  ymuint
  child_num() const;

  /// @brief 子供を返す．
  /// @param[in] pos 位置( 0 <= pos < child_num() )
  Ast*
  child(ymuint pos) const;

  /// @brief 子供を追加する．
  void
  add_child(Ast* child);

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  /// @param[in] indent インデントレベル
  void
  print(ostream& s,
	ymuint indent = 0) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // リストの本体
  vector<Ast*> mList;

};

END_NAMESPACE_YM_YMSL

#endif // ASTLIST_H

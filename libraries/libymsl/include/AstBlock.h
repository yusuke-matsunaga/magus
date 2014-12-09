#ifndef ASTBLOCK_H
#define ASTBLOCK_H

/// @file AstBlock.h
/// @brief AstBlock のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmslDict.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstBlock AstBlock.h "AstBlock.h"
/// @brief ブロックを表すクラス
//////////////////////////////////////////////////////////////////////
class AstBlock
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent_dict 親の辞書
  AstBlock(YmslDict* parent_dict);

  /// @brief コンストラクタ
  /// @param[in] parent 親のブロック
  AstBlock(AstBlock* parent);

  /// @brief デストラクタ
  ~AstBlock();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ステートメントを追加する．
  void
  add_statement(AstStatement* statement);

  /// @brief 変数定義を追加する．
  /// @param[in] item 追加する要素
  void
  add_vardecl(AstVarDecl* item);

  /// @brief 名前からラベルステートメントを見つける．
  /// @param[in] name 名前
  ///
  /// ここになければ親のブロックを探す．
  /// それでもなければ NULL を返す．
  AstStatement*
  find_label(ShString name) const;

  /// @brief 名前から変数宣言を見つける．
  /// @param[in] name 名前
  ///
  /// ここになければ親のブロックを探す．
  /// それでもなければ NULL を返す．
  AstVarDecl*
  find_vardecl(ShString name) const;

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // statement リスト
  vector<AstStatement*> mStatementList;

  // 辞書
  YmslDict mDict;

};

END_NAMESPACE_YM_YMSL

#endif // ASTBLOCK_H

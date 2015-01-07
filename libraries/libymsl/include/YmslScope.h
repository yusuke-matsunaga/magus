#ifndef YMSLSCOPE_H
#define YMSLSCOPE_H

/// @file YmslScope.h
/// @brief YmslScope のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "SymDict.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslScope YmslScope.h "YmslScope.h"
/// @brief スコープを表すクラス
//////////////////////////////////////////////////////////////////////
class YmslScope
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent_dict 親の辞書
  YmslScope(SymDict* parent_dict);

  /// @brief コンストラクタ
  /// @param[in] parent 親のブロック
  YmslScope(YmslScope* parent);

  /// @brief デストラクタ
  ~YmslScope();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ステートメントを追加する．
  void
  add_statement(AstStatement* statement);

  /// @brief 関数を追加する．
  /// @param[in] item 追加する要素
  void
  add_function(AstFuncDecl* item);

  /// @brief ラベルを追加する．
  /// @param[in] item 追加する要素
  void
  add_label(YmslLabel* item);

  /// @brief 変数定義を追加する．
  /// @param[in] item 追加する要素
  void
  add_vardecl(AstVarDecl* item);

  /// @brief 名前からラベルを見つける．
  /// @param[in] name 名前
  ///
  /// ここになければ親のブロックを探す．
  /// それでもなければ NULL を返す．
  YmslLabel*
  find_label(ShString name) const;

  /// @brief 名前から変数宣言を見つける．
  /// @param[in] name 名前
  ///
  /// ここになければ親のブロックを探す．
  /// それでもなければ NULL を返す．
  AstVarDecl*
  find_vardecl(ShString name) const;

  /// @brief ステートメント数を返す．
  ymuint
  statement_num() const;

  /// @brief ステートメントを返す．
  /// @param[in] pos 位置 ( 0 <= pos < statement_num() )
  AstStatement*
  statement(ymuint pos) const;

  /// @brief 命令コードのサイズを計算する．
  ymuint
  calc_size() const;

  /// @brief 命令コードを生成する．
  /// @param[in] driver ドライバ
  /// @param[in] code_list 命令コードの格納先
  /// @param[inout] addr 命令コードの現在のアドレス
  ///
  /// addr の値は更新される．
  void
  compile(YmslDriver& driver,
	  YmslCodeList& code_list,
	  Ymsl_INT& addr);

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  /// @param[in] indent インデントレベル
  void
  print(ostream& s,
	ymuint indent) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // statement リスト
  vector<AstStatement*> mStatementList;

  // 辞書
  SymDict mDict;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLSCOPE_H

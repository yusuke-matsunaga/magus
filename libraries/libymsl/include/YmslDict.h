#ifndef YMSLDICT_H
#define YMSLDICT_H

/// @file YmslDict.h
/// @brief YmslDict のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

class SymHandle;

//////////////////////////////////////////////////////////////////////
/// @class YmslDict YmslDict.h "YmslDict.h"
/// @breif 辞書を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslDict
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親
  YmslDict(YmslDict* parent = NULL);

  /// @brief デストラクタ
  ~YmslDict();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ステートメントを追加する．
  void
  add_statement(AstStatement* statement);

  /// @brief 関数定義を追加する．
  /// @param[in] item 追加する要素
  void
  add_funcdecl(AstFuncDecl* item);

  /// @brief 変数定義を追加する．
  /// @param[in] item 追加する要素
  void
  add_vardecl(AstVarDecl* item);

  /// @brief 名前空間を追加する．
  /// @param[in] item 追加する要素
  void
  add_namespace(YmslSubspace* subspace);

  /// @brief 名前からラベルステートメントを見つける．
  /// @param[in] name 名前
  ///
  /// ここになければ親のブロックを探す．
  /// それでもなければ NULL を返す．
  AstStatement*
  find_label(ShString name) const;

  /// @brief 名前から関数宣言を見つける．
  /// @param[in] name 名前
  ///
  /// ここになければ親のブロックを探す．
  /// それでもなければ NULL を返す．
  AstFuncDecl*
  find_funcdecl(ShString name) const;

  /// @brief 名前から変数宣言を見つける．
  /// @param[in] name 名前
  ///
  /// ここになければ親のブロックを探す．
  /// それでもなければ NULL を返す．
  AstVarDecl*
  find_vardecl(ShString name) const;

  /// @brief 名前から名前空間を探す．
  /// @param[in] name 名前
  ///
  /// ここになければ親のブロックを探す．
  /// それでもなければ NULL を返す．
  YmslSubspace*
  find_subspace(ShString name) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ表を確保する．
  /// @param[in] req_size 要求サイズ
  void
  alloc_table(ymuint req_size);

  /// @brief 名前からハンドルを探す．
  /// @param[in] name 名前
  SymHandle*
  find(ShString name) const;

  /// @brief ハンドルを登録する．
  void
  put(SymHandle* handle);

  /// @brief ハンドルを登録する．
  ///
  /// こちらはサイズチェックなし
  void
  _put(SymHandle* handle);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の辞書
  YmslDict* mParent;

  // ハッシュサイズ
  ymuint mHashSize;

  // ハッシュ表を拡大するしきい値
  ymuint mNextLimit;

  // ハッシュ表
  SymHandle** mHashTable;

  // ハッシュの要素数
  ymuint mHashNum;

};

END_NAMESPACE_YM_YMSL


#endif // YMSLDICT_H

#ifndef SYMDICT_H
#define SYMDICT_H

/// @file SymDict.h
/// @brief SymDict のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

class SymHandle;

//////////////////////////////////////////////////////////////////////
/// @class SymDict SymDict.h "SymDict.h"
/// @breif 辞書を表すクラス
//////////////////////////////////////////////////////////////////////
class SymDict
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親
  SymDict(SymDict* parent = NULL);

  /// @brief デストラクタ
  ~SymDict();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前空間を追加する．
  /// @param[in] item 追加する要素
  void
  add_namespace(YmslSubspace* subspace);

  /// @brief 関数を追加する．
  /// @param[in] item 追加する要素
  void
  add_function(AstFuncDecl* item);

  /// @brief 変数定義を追加する．
  /// @param[in] item 追加する要素
  void
  add_vardecl(AstVarDecl* item);

  /// @brief ラベルを追加する．
  void
  add_label(YmslLabel* label);

  /// @brief 名前から名前空間を探す．
  /// @param[in] name 名前
  ///
  /// ここになければ親のブロックを探す．
  /// それでもなければ NULL を返す．
  YmslSubspace*
  find_subspace(ShString name) const;

  /// @brief 名前から関数を見つける．
  /// @param[in] name 名前
  ///
  /// ここになければ親のブロックを探す．
  /// それでもなければ NULL を返す．
  AstFuncDecl*
  find_function(ShString name) const;

  /// @brief 名前から変数宣言を見つける．
  /// @param[in] name 名前
  ///
  /// ここになければ親のブロックを探す．
  /// それでもなければ NULL を返す．
  AstVarDecl*
  find_vardecl(ShString name) const;

  /// @brief 名前からラベルを見つける．
  /// @param[in] name 名前
  ///
  /// ここになければ親のブロックを探す．
  /// それでもなければ NULL を返す．
  YmslLabel*
  find_label(ShString name) const;


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
  SymDict* mParent;

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


#endif // SYMDICT_H

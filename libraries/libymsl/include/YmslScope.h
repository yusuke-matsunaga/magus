#ifndef YMSLSCOPE_H
#define YMSLSCOPE_H

/// @file YmslScope.h
/// @brief YmslScope のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
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
  /// @param[in] parent 親のスコープ
  /// @param[in] name 名前
  YmslScope(YmslScope* parent,
	    ShString name = ShString());

  /// @brief デストラクタ
  ~YmslScope();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 自身の名前を返す．
  ///
  /// ShString() (空文字列)の場合もある．
  ShString
  name() const;

  /// @brief 子のスコープを追加する．
  /// @param[in] item 追加する要素
  void
  add_scope(YmslScope* item);

  /// @brief 関数を追加する．
  /// @param[in] item 追加する要素
  void
  add_function(YmslFunction* item);

  /// @brief 変数を追加する．
  /// @param[in] item 追加する要素
  void
  add_var(YmslVar* item);

  /// @brief 列挙型を追加する．
  void
  add_enum(YmslEnum* item);

  /// @brief ラベルを追加する．
  /// @param[in] item 追加する要素
  void
  add_label(YmslLabel* item);

  /// @brief 名前からハンドルを探す．
  /// @param[in] name 名前
  ObjHandle*
  find(ShString name) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ表を確保する．
  /// @param[in] req_size 要求サイズ
  void
  alloc_table(ymuint req_size);

  /// @brief ハンドルを登録する．
  void
  put(ObjHandle* handle);

  /// @brief ハンドルを登録する．
  ///
  /// こちらはサイズチェックなし
  void
  _put(ObjHandle* handle);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  YmslScope* mParent;

  // 自身の名前
  ShString mName;

  // ハッシュサイズ
  ymuint mHashSize;

  // ハッシュ表を拡大するしきい値
  ymuint mNextLimit;

  // ハッシュ表
  ObjHandle** mHashTable;

  // ハッシュの要素数
  ymuint mHashNum;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLSCOPE_H

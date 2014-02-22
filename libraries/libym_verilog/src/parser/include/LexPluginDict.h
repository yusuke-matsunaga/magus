#ifndef LEXPLUGINDICT_H
#define LEXPLUGINDICT_H

/// @file LexPluginDict.h
/// @brief LexPlugin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "verilog/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

class LexPlugin;

//////////////////////////////////////////////////////////////////////
// LexPlugin の辞書
//////////////////////////////////////////////////////////////////////
class LexPluginDict
{
public:

  /// @brief コンストラクタ
  LexPluginDict();

  /// @brief デストラクタ
  /// @note 登録されているプラグインはすべて削除される．
  ~LexPluginDict();


public:

  /// @brief プラグインを登録できるか調べる．
  /// @param[in] name プラグイン名
  /// @return true 登録可能
  /// @return false 同名の定義済みプラグインがあり，登録不可能
  bool
  check_name(const char* name);

  /// @brief プラグインを登録する．
  /// @param[in] plugin プラグイン
  void
  reg_plugin(LexPlugin* plugin);

  /// @brief プラグインの登録を解除する．
  /// @param[in] name 名前
  /// @retval true 指定された名前のプラグインの登録解除が成功した．
  /// @retval false 指定された名前のプラグインは登録されていなかった．
  bool
  unreg_plugin(const char* name);

  /// @brief プラグインを検索する．
  /// @param[in] name 名前
  /// @return name という名前で登録されているプラグインを返す．
  LexPlugin*
  find_plugin(const char* name) const;


private:

  /// @brief ハッシュ表を確保する．
  void
  new_table(ymuint size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表のサイズ
  ymuint32 mTableSize;

  // ハッシュのためのマスク
  ymuint32 mHashMask;

  // ハッシュ表
  LexPlugin** mHashTable;

  // ハッシュ表に登録されている要素数
  ymuint32 mNum;

  // 拡大する参考値
  ymuint32 mNextLimit;

};


END_NAMESPACE_YM_VERILOG

#endif // LEXPLUGINDICT_H

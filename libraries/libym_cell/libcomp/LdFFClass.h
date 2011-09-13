#ifndef LDFFCLASS_H
#define LDFFCLASS_H

/// @file LdFFClass.h
/// @brief LdFFClass のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libdump_nsdef.h"


BEGIN_NAMESPACE_YM_CELL_LIBDUMP

//////////////////////////////////////////////////////////////////////
/// @class LdFFClass LdFFClass.h "LdFFClass.h"
/// @brief FFのクラスを表すクラス
/// ここでは，ピン位置の違いを無視した同機能のFFの集合をクラスと呼ぶ．
//////////////////////////////////////////////////////////////////////
class LdFFClass
{
  friend class LdFFMgr;

private:

  /// @brief コンストラクタ
  LdFFClass();

  /// @brief デストラクタ
  ~LdFFClass();


public:

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 入力ピン数の取得
  virtual
  ymuint
  input_num() const;

  /// @brief 出力ピン数の取得
  virtual
  ymuint
  output_num() const;

  /// @brief 入出力ピン数の取得
  virtual
  ymuint
  inout_num() const;

  /// @brief 出力の論理関数を返す．
  /// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
  /// @note FF/ラッチの場合は状態変数の変数(Q, XQ)が2つ入力に加わる．
  virtual
  TvFunc
  logic_function(ymuint pos) const;

  /// @brief 出力のトライステート条件関数を返す．
  /// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
  /// @note トライステートでない場合には定数0関数を返す．
  virtual
  TvFunc
  tristate_function(ymuint pos) const;

  /// @brief FFセルの場合に次状態関数を返す．
  /// @note それ以外の型の場合の返り値は不定
  virtual
  TvFunc
  next_state_function() const;

  /// @brief FFセルの場合にクロックのアクティブエッジを表す関数を返す．
  /// @note それ以外の型の場合の返り値は不定
  virtual
  TvFunc
  clock_function() const;

  /// @brief ラッチセルの場合にデータ入力関数を返す．
  /// @note それ以外の型の場合の返り値は不定
  virtual
  TvFunc
  data_in_function() const;

  /// @brief ラッチセルの場合にイネーブル条件を表す関数を返す．
  /// @note それ以外の型の場合の返り値は不定
  virtual
  TvFunc
  enable_function() const;

  /// @brief FFセル/ラッチセルの場合にクリア端子を持っていたら true を返す．
  virtual
  bool
  has_clear() const;

  /// @brief FFセル/ラッチセルの場合にクリア条件を表す関数を返す．
  /// @note クリア端子がない場合の返り値は不定
  virtual
  TvFunc
  clear_function() const;

  /// @brief FFセル/ラッチセルの場合にプリセット端子を持っていたら true を返す．
  virtual
  bool
  has_preset() const;

  /// @brief FFセル/ラッチセルの場合にプリセット条件を表す関数を返す．
  /// @note プリセット端子がない場合の返り値は不定
  virtual
  TvFunc
  preset_function() const;

  /// @brief このクラスに属しているFFグループのリストを返す．
  const vector<LdFFGroup*>&
  group_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // FFグループのリスト
  vector<LdFFGroup*> mGroupList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
LdFFClass::LdFFClass() :
  mId(0)
{
}

// @brief デストラクタ
inline
LdFFClass::~LdFFClass()
{
}

// @brief ID番号を返す．
inline
ymuint
LdFFClass::id() const
{
  return mId;
}

// @brief このクラスに属しているFFグループのリストを返す．
inline
const vector<LdFFGroup*>&
LdFFClass::group_list() const
{
  return mGroupList;
}

END_NAMESPACE_YM_CELL_LIBDUMP

#endif // LDFFCLASS_H

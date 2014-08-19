#ifndef EXPRLUGIN_H
#define EXPRLUGIN_H

/// @file LexPlugin.h
/// @brief LexPlugin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/verilog.h"
#include "YmUtils/File.h"
#include "YmUtils/FileRegion.h"
#include "YmUtils/StrBuff.h"
#include "RawLex.h"


BEGIN_NAMESPACE_YM_VERILOG

class LexPluginDict;
class MacroData;
class TokenInfo;

//////////////////////////////////////////////////////////////////////
/// @class LexPlugin Lex.h "Lex.h"
/// @ingroup VlParser
/// @brief Lex に付随するプラグインの基底クラス
//////////////////////////////////////////////////////////////////////
class LexPlugin
{
  friend class LexPluginDict;

public:

  /// @brief コンストラクタ
  /// @param[in] lex 親の Lex オブジェクト
  /// @param[in] name compiler directive 名
  LexPlugin(RawLex& lex,
	    const char* name);

  /// @brief デストラクタ
  virtual
  ~LexPlugin();


public:

  /// @brief 名前を返す．
  const char*
  name();


public:
  //////////////////////////////////////////////////////////////////////
  // LexPlugin の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  parse() = 0;

  /// @brief 条件コンパイル用のプラグインの時 true を返す仮想関数
  /// @note デフォルトの実装では false を返す．
  virtual
  bool
  is_cond_plugin();

  /// @brief マクロの時 true を返す仮想関数
  /// @note デフォルトの実装では false を返す．
  virtual
  bool
  is_macro();


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いる便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 状態を初期化する
  /// @param[in] file_region 初期化をしているソースファイル上のファイル位置
  void
  resetall(const FileRegion& file_region);

  /// @brief トークンの位置の取得
  /// @return 最後に読んだトークンのファイル上の位置
  FileRegion
  cur_token_loc() const;

  /// @brief トークン文字列の取得
  /// @return 最後に読んだトークン文字列
  const char*
  cur_string() const;

  /// @brief 指定されたトークンを期待して待つ．
  /// @param[in] tok トークン
  /// @retval true tok というトークンを読み出した．
  /// @retval false tok 以外の非空白トークンを読み出した．
  /// @note tok というトークンが読み出されるまで空白文字を読み飛ばす
  bool
  expect(int tok);

  /// @brief 改行を期待して待つ．
  /// @retval true 改行もしくは EOF を読み出した．
  /// @retval false 改行もしくは EOF 以外の非空白トークンを読み出した．
  /// @note 空白トークンを読み飛ばす．
  bool
  expect_nl();

  /// @brief 空白以外の次のトークンの取得
  /// @return 空白以外の次のトークンの id
  int
  get_nospace_token();

  /// @brief トークンを取出す本当の関数
  /// @return 本当に次のトークンの id
  int
  get_raw_token();

  /// @brief モジュール定義の中にいるとき true を返す
  /// @return モジュール定義の中にいるとき true を返す．
  bool
  is_in_module();

  /// @brief デバッグフラグのチェック
  /// @return デバッグフラグが立っていたら true を返す．
  bool
  debug() const;

  /// @brief 親の Lex の取得
  /// @return 親の Lex オブジェクト
  RawLex&
  lex();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の Lex
  RawLex& mLex;

  // 名前
  StrBuff mName;

  // ハッシュ用のリンク
  LexPlugin* mLink;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////


// @brief 親の Lex の取得
// @return 親の Lex オブジェクト
inline
RawLex&
LexPlugin::lex()
{
  return mLex;
}

// @brief 名前を返す．
inline
const char*
LexPlugin::name()
{
  return mName.c_str();
}

// @brief 状態を初期化する
inline
void
LexPlugin::resetall(const FileRegion& file_region)
{
  mLex.resetall(file_region);
}

// @brief トークンの位置の取得
// @return 最後に読んだトークンのファイル上の位置
inline
FileRegion
LexPlugin::cur_token_loc() const
{
  return mLex.cur_token_loc();
}

// @brief トークン文字列の取得
// @return 最後に読んだトークン文字列
inline
const char*
LexPlugin::cur_string() const
{
  return mLex.cur_string();
}

// @brief トークンを取出す本当の関数
// @return 本当に次のトークンの id
inline
int
LexPlugin::get_raw_token()
{
  return mLex.get_raw_token();
}

// @brief モジュール定義の中にいるとき true を返す
inline
bool
LexPlugin::is_in_module()
{
  return mLex.mModuleMode;
}

// @brief デバッグフラグのチェック
// @return デバッグフラグが立っていたら true を返す．
inline
bool
LexPlugin::debug() const
{
  return mLex.debug();
}

END_NAMESPACE_YM_VERILOG

#endif // EXPRLUGIN_H

#ifndef LIBYM_CELL_MISLIB_MISLIBLEX_H
#define LIBYM_CELL_MISLIB_MISLIBLEX_H

/// @file libym_cell/mislib/MislibLex.h
/// @brief MislibLex のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibLex.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "mislib_nsdef.h"

#include "ym_utils/StrBuff.h"
#include "ym_utils/FileScanner.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/MsgHandler.h"


BEGIN_NAMESPACE_YM_CELL_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibLex MislibLex.h "MislibLex.h"
/// @brief Mislib 用の LEX クラス
//////////////////////////////////////////////////////////////////////
class MislibLex
{
public:

  /// @brief コンストラクタ
  /// @param[in] msg_mgr メッセージマネージャ
  MislibLex(MsgMgr& msg_mgr);

  /// @brief デストラクタ
  ~MislibLex();


public:

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  /// @return 失敗したら false を返す．
  bool
  open_file(const string& filename);

  /// @brief トークンを一つとってくる．
  int
  read_token();

  /// @brief 直前の read_token() に対応する文字列を返す．
  const char*
  cur_string() const;

  /// @brief cur_string() を double に変換したものを返す．
  double
  cur_num() const;

  /// @brief 現在のトークンの位置情報を返す．
  FileRegion
  cur_loc() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 一文字読み出す．
  /// @note 実際には peek(); acept() と等価
  int
  get();

  /// @brief 次の文字を読み出す．
  /// @note ファイル位置の情報等は変わらない
  int
  peek();

  /// @brief 直前の peek() を確定させる．
  void
  accept();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メッセージマネージャ
  MsgMgr& mMsgMgr;

  // ファイル入力用のオブジェクト
  FileScanner mFileScanner;

  // 現在読込中の文字列を貯めておくバッファ
  StrBuff mCurString;

  // 現在のトークンの開始位置の行番号
  ymuint32 mFirstLine;

  // 現在のトークンの開始位置のコラム位置
  ymuint32 mFirstColumn;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 直前の read_token() に対応する文字列を返す．
inline
const char*
MislibLex::cur_string() const
{
  return mCurString.c_str();
}

// @brief cur_string() を double に変換したものを返す．
inline
double
MislibLex::cur_num() const
{
  return strtod(cur_string(), static_cast<char**>(NULL));
}

// @brief 一文字読み出す．
// @note 実際には peek(); acept() と等価
inline
int
MislibLex::get()
{
  return mFileScanner.get();
}

// @brief 次の文字を読み出す．
// @note ファイル位置の情報等は変わらない
inline
int
MislibLex::peek()
{
  return mFileScanner.peek();
}

// @brief 直前の peek() を確定させる．
inline
void
MislibLex::accept()
{
  return mFileScanner.accept();
}

END_NAMESPACE_YM_CELL_MISLIB

#endif // LIBYM_CELL_MISLIB_MISLIBLEX_H

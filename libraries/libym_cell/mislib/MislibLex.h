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

  // 直前の読み込みを確定させる．
  void
  accept(int c);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メッセージマネージャ
  MsgMgr& mMsgMgr;

  // 現在のファイル情報
  FileInfo mFileInfo;

  // ファイル入力用のオブジェクト
  FileScanner mFileScanner;

  // 現在読込中の文字列を貯めておくバッファ
  StrBuff mCurString;

  // 現在のトークンの開始位置
  ymuint32 mFirstColumn;

  // 現在のトークンの終了位置
  ymuint32 mLastColumn;

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

END_NAMESPACE_YM_CELL_MISLIB

#endif // LIBYM_CELL_MISLIB_MISLIBLEX_H

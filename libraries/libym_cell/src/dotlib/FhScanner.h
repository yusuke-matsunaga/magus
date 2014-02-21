#ifndef FHSCANNER_H
#define FHSCANNER_H

/// @file FhScanner.h
/// @brief FhScanner のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib_int.h"
#include "utils/StrBuff.h"
#include "utils/FileRegion.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class FhScanner FhScanner.h "FhScanner.h"
/// @brief FuncHandler 用の字句解析器
//////////////////////////////////////////////////////////////////////
class FhScanner
{
public:

  /// @brief コンストラクタ
  FhScanner();

  /// @brief デストラクタ
  ~FhScanner();


public:

  /// @brief 文字列をセットする．
  /// @param[in] str 文字列
  /// @param[in] str のファイル上の位置
  void
  init(const char* str,
       const FileRegion& loc);

  /// @brief トークンを読み込む．
  /// @param[out] loc 対応するファイル上の位置情報を格納する変数
  tTokenType
  read_token(FileRegion& loc);

  /// @brief 直前の read_token() で読んだトークンの文字列を返す．
  const char*
  cur_string();

  /// @brief 直前の read_token() で読んだトークンの整数値を返す．
  int
  cur_int();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 一文字読み出す．
  int
  get();

  /// @brief 一文字先読みする．
  int
  peek();

  /// @brief 先読みを確定する．
  void
  accept();

  /// @brief トークンの先頭のファイル位置を設定する．
  void
  set_first_loc();

  /// @brief 現在のファイル位置を返す．
  FileRegion
  cur_loc();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パーズ中の文字列
  const char* mSrcString;

  // mString のファイル位置
  FileRegion mSrcLoc;

  // 現在の読み出し位置
  ymuint mCurPos;

  // 現在のトークン文字列
  StrBuff mCurString;

  // 最初のコラム位置
  ymuint32 mFirstColumn;

};

END_NAMESPACE_YM_DOTLIB

#endif // FHSCANNER_H

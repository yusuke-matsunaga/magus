#ifndef YMUTILS_STRINGIDO_H
#define YMUTILS_STRINGIDO_H

/// @file YmUtils/StringIDO.h
/// @brief StringIDO のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "YmUtils/IDO.h"
#include "YmUtils/FileInfo.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class StringIDO StringIDO.h "YmUtils/StringIDO.h"
/// @ingroup YmUtils
/// @brief 文字列を用いた IDO の継承クラス
//////////////////////////////////////////////////////////////////////
class StringIDO :
  public IDO
{
public:

  /// @brief コンストラクタ
  /// @param[in] str 文字列
  StringIDO(const char* str);

  /// @brief デストラクタ
  virtual
  ~StringIDO();


public:
  //////////////////////////////////////////////////////////////////////
  // IDO の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 読み出し可能なら true を返す．
  virtual
  bool
  is_ready() const;

  /// @brief オープン中のファイル情報を得る．
  virtual
  const FileInfo&
  file_info() const;

  /// @brief 現在のファイル情報を書き換える．
  /// @param[in] file_info 新しいファイル情報
  /// @note プリプロセッサのプラグマなどで用いることを想定している．
  /// @note 通常は使わないこと．
  virtual
  void
  set_file_info(const FileInfo& file_info);

  /// @brief データを読み込む．
  /// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
  /// @param[in] n 読み込むデータサイズ
  /// @return 実際に読み込んだ量を返す．
  virtual
  ymint64
  read(ymuint8* buff,
       ymuint64 n);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル情報
  // このクラスでは意味を持たない．
  FileInfo mFileInfo;

  // 文字列
  string mString;

  // 現在処理中の行番号
  ymuint32 mLineNum;

  // 次に読み出す位置
  ymuint32 mColumnNum;

};

END_NAMESPACE_YM

#endif // YMUTILS_STRINGIDO_H

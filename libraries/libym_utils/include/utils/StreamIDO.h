#ifndef UTILS_STREAMIDO_H
#define UTILS_STREAMIDO_H

/// @file utils/StreamIDO.h
/// @brief StreamIDO のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "utils/IDO.h"
#include "utils/FileInfo.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class StreamIDO StreamIDO.h "utils/StreamIDO.h"
/// @ingroup YmUtils
/// @brief istream を用いた IDO の継承クラス
//////////////////////////////////////////////////////////////////////
class StreamIDO :
  public IDO
{
public:

  /// @brief コンストラクタ
  /// @param[in] s 入力ストリーム
  StreamIDO(istream& s);

  /// @brief デストラクタ
  virtual
  ~StreamIDO();


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
  ssize_t
  read(ymuint8* buff,
       ymuint64 n);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル情報
  // このクラスでは意味を持たない．
  FileInfo mFileInfo;

  // 入力ストリーム
  istream& mS;

};

END_NAMESPACE_YM

#endif // UTILS_STREAMIDO_H

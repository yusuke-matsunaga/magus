#ifndef YM_UTILS_FILEIDO_H
#define YM_UTILS_FILEIDO_H

/// @file ym_utils/FileIDO.h
/// @brief FileIDO のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/IDO.h"


BEGIN_NAMESPACE_YM

class FileBuff;

//////////////////////////////////////////////////////////////////////
/// @class FileIDO FileIDO.h "ym_utils/FileIDO.h"
/// @ingroup YmUtils
/// @brief 通常のファイルを用いた IDO の継承クラス
//////////////////////////////////////////////////////////////////////
class FileIDO :
  public IDO
{
private:

  static
  const ymuint kDefaultBuffSize = 4096;


public:

  /// @brief 空のコンストラクタ
  /// @param[in] buff_size バッファサイズ
  explicit
  FileIDO(ymuint buff_size = kDefaultBuffSize);

  /// @brief コンストラクタ
  /// @param[in] filename ファイル名
  /// @param[in] buff_size バッファサイズ
  /// @note 意味的にはコンストラクタ + open()
  explicit
  FileIDO(const char* filename,
	  ymuint buff_size = kDefaultBuffSize);

  /// @brief コンストラクタ
  /// @param[in] filename ファイル名
  /// @param[in] buff_size バッファサイズ
  /// @note 意味的にはコンストラクタ + open()
  explicit
  FileIDO(const string& filename,
	  ymuint buff_size = kDefaultBuffSize);

  /// @brief デストラクタ
  virtual
  ~FileIDO();


public:
  //////////////////////////////////////////////////////////////////////
  // FileIDO の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 読み出し可能なら true を返す．
  operator bool() const;

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  /// @note 他のファイルを開いていたら強制的に close する．
  bool
  open(const char* filename);

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  /// @note 他のファイルを開いていたら強制的に close する．
  bool
  open(const string& filename);

  /// @brief ファイルを閉じる．
  /// @note 以降の読み出しは行われない．
  void
  close();


public:
  //////////////////////////////////////////////////////////////////////
  // IDO の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データを読み込む．
  /// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
  /// @param[in] n 読み込むデータサイズ
  /// @return 実際に読み込んだ量を返す．
  /// @note エラーが起こったら負の数を返す．
  virtual
  ssize_t
  read(ymuint8* buff,
       ymuint64 n);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイルバッファ
  FileBuff* mFileBuff;

};

END_NAMESPACE_YM

#endif // YM_UTILS_FILEIDO_H

#ifndef YM_UTILS_FILEODO_H
#define YM_UTILS_FILEODO_H

/// @file ym_utils/FileODO.h
/// @brief FileODO のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/ODO.h"


BEGIN_NAMESPACE_YM

class FileBuff;

//////////////////////////////////////////////////////////////////////
/// @class FileODO FileODO.h "ym_utils/FileODO.h"
/// @ingroup YmUtils
/// @brief 通常のファイルを用いた ODO の継承クラス
//////////////////////////////////////////////////////////////////////
class FileODO :
  public ODO
{
private:

  static
  const ymuint kDefaultBuffSize = 4096;

public:

  /// @brief 空のコンストラクタ
  /// @param[in] buff_size バッファサイズ
  explicit
  FileODO(ymuint buff_size = kDefaultBuffSize);

  /// @brief コンストラクタ
  /// @param[in] filename ファイル名
  /// @param[in] buff_size バッファサイズ
  explicit
  FileODO(const char* filename,
	  ymuint buff_size = kDefaultBuffSize);

  /// @brief コンストラクタ
  /// @param[in] filename ファイル名
  /// @param[in] buff_size バッファサイズ
  explicit
  FileODO(const string& filename,
	  ymuint buff_size = kDefaultBuffSize);

  /// @brief デストラクタ
  virtual
  ~FileODO();


public:
  //////////////////////////////////////////////////////////////////////
  // FileODO の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 書き込み可能なら true を返す．
  operator bool() const;

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  void
  open(const char* filename);

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  void
  open(const string& filename);

  /// @brief ファイルを閉じる．
  /// @note 以降の書き込みは行われない．
  void
  close();


public:
  //////////////////////////////////////////////////////////////////////
  // ODO の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データを書き出す．
  /// @param[in] buff データを収めた領域のアドレス
  /// @param[in] n データサイズ
  /// @return 実際に書き出した量を返す．
  virtual
  ssize_t
  write(const ymuint8* buff,
	ymuint64 n);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイルバッファ
  FileBuff* mFileBuff;

};

END_NAMESPACE_YM

#endif // YM_UTILS_FILEODO_H

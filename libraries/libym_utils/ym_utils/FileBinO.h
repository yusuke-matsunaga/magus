#ifndef YM_UTILS_FILEBINO_H
#define YM_UTILS_FILEBINO_H

/// @file ym_utils/FileBinO.h
/// @brief FileBinO のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/BinO.h"


BEGIN_NAMESPACE_YM

class FileBuff;

//////////////////////////////////////////////////////////////////////
/// @class FileBinO FileBinO.h "ym_utils/FileBinO.h"
/// @ingroup YmUtils
/// @brief 通常のファイルを用いた BinO の継承クラス
//////////////////////////////////////////////////////////////////////
class FileBinO :
  public BinO
{
private:

  static
  const ymuint kDefaultBuffSize = 4096;

public:

  /// @brief 空のコンストラクタ
  /// @param[in] buff_size バッファサイズ
  explicit
  FileBinO(ymuint buff_size = kDefaultBuffSize);

  /// @brief コンストラクタ
  /// @param[in] filename ファイル名
  /// @param[in] buff_size バッファサイズ
  explicit
  FileBinO(const char* filename,
	   ymuint buff_size = kDefaultBuffSize);

  /// @brief コンストラクタ
  /// @param[in] filename ファイル名
  /// @param[in] buff_size バッファサイズ
  explicit
  FileBinO(const string& filename,
	   ymuint buff_size = kDefaultBuffSize);

  /// @brief デストラクタ
  virtual
  ~FileBinO();


public:
  //////////////////////////////////////////////////////////////////////
  // FileBinO の関数
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
  // BinO の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データを書き出す．
  /// @param[in] n データサイズ
  /// @param[in] buff データを収めた領域のアドレス
  /// @return 実際に書き出した量を返す．
  virtual
  ssize_t
  write(ymuint64 n,
	const ymuint8* buff);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイルバッファ
  FileBuff* mFileBuff;

};

END_NAMESPACE_YM

#endif // YM_UTILS_FILEBINO_H

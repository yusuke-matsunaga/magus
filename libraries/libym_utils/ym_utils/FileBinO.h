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

//////////////////////////////////////////////////////////////////////
/// @class FileBinO FileBinO.h "ym_utils/FileBinO.h"
/// @ingroup YmUtils
/// @brief 通常のファイルを用いた BinO の継承クラス
//////////////////////////////////////////////////////////////////////
class FileBinO :
  public BinO
{
public:

  /// @brief 空のコンストラクタ
  FileBinO();

  /// @brief コンストラクタ
  /// @param[in] filename ファイル名
  FileBinO(const char* filename);

  /// @brief コンストラクタ
  /// @param[in] filename ファイル名
  FileBinO(const string& filename);

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
  ymuint
  write(ymuint64 n,
	const ymuint8* buff);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる定数
  //////////////////////////////////////////////////////////////////////

  static
  const ymuint BUFF_SIZE = 4096;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイルディスクリプタ
  int mFd;

  // バッファ
  ymuint8 mBuff[BUFF_SIZE];

  // バッファ上の書き込み位置
  ymuint16 mPos;

};

END_NAMESPACE_YM

#endif // YM_UTILS_FILEBINO_H

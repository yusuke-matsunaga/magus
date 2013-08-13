#ifndef YM_UTILS_FILEBINI_H
#define YM_UTILS_FILEBINI_H

/// @file ym_utils/FileBinI.h
/// @brief FileBinI のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/BinI.h"


BEGIN_NAMESPACE_YM

class FileBuff;

//////////////////////////////////////////////////////////////////////
/// @class FileBinI FIleBinI.h "ym_utils/FileBinI.h"
/// @ingroup YmUtils
/// @brief 通常のファイルを用いた BinI の継承クラス
//////////////////////////////////////////////////////////////////////
class FileBinI :
  public BinI
{
private:

  static
  const ymuint kDefaultBuffSize = 4096;

public:

  /// @brief 空のコンストラクタ
  /// @param[in] buff_size バッファサイズ
  explicit
  FileBinI(ymuint buff_size = kDefaultBuffSize);

  /// @brief コンストラクタ
  /// @param[in] filename ファイル名
  /// @param[in] buff_size バッファサイズ
  explicit
  FileBinI(const char* filename,
	   ymuint buff_size = kDefaultBuffSize);

  /// @brief コンストラクタ
  /// @param[in] filename ファイル名
  /// @param[in] buff_size バッファサイズ
  explicit
  FileBinI(const string& filename,
	   ymuint buff_size = kDefaultBuffSize);

  /// @brief デストラクタ
  virtual
  ~FileBinI();


public:
  //////////////////////////////////////////////////////////////////////
  // FileBinI の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 読み出し可能なら true を返す．
  operator bool() const;

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  bool
  open(const char* filename);

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  bool
  open(const string& filename);

  /// @brief ファイルを閉じる．
  /// @note 以降の読み出しは行われない．
  void
  close();


public:
  //////////////////////////////////////////////////////////////////////
  // BinI の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データを読み込む．
  /// @param[in] n 読み込むデータサイズ
  /// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
  /// @return 実際に読み込んだ量を返す．
  virtual
  ssize_t
  read(ymuint64 n,
       ymuint8* buff);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイルバッファ
  FileBuff* mFileBuff;

};

END_NAMESPACE_YM

#endif // YM_UTILS_FILEBINI_H

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

//////////////////////////////////////////////////////////////////////
/// @class FileBinI FIleBinI.h "ym_utils/FileBinI.h"
/// @ingroup YmUtils
/// @brief 通常のファイルを用いた BinI の継承クラス
//////////////////////////////////////////////////////////////////////
class FileBinI :
  public BinI
{
public:

  /// @brief 空のコンストラクタ
  FileBinI();

  /// @brief コンストラクタ
  /// @param[in] filename ファイル名
  FileBinI(const char* filename);

  /// @brief コンストラクタ
  /// @param[in] filename ファイル名
  FileBinI(const string& filename);

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
  void
  open(const char* filename);

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  void
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
  ymuint
  read(ymuint64 n,
       ymuint8* buff);


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

  // バッファ上の読み出し位置
  ymuint16 mPos;

};

END_NAMESPACE_YM

#endif // YM_UTILS_FILEBINI_H

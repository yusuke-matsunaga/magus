#ifndef YMUTILS_FILEODO_H
#define YMUTILS_FILEODO_H

/// @file YmUtils/FileODO.h
/// @brief FileODO のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmUtils/ODO.h"
#include "YmUtils/CodecType.h"


#if defined(YM_WIN32)
typedef int mode_t;
#endif

BEGIN_NAMESPACE_YM

class FileCoder;

//////////////////////////////////////////////////////////////////////
/// @class FileODO FileODO.h "YmUtils/FileODO.h"
/// @ingroup YmUtils
/// @brief 通常のファイルを用いた ODO の継承クラス
//////////////////////////////////////////////////////////////////////
class FileODO :
  public ODO
{
public:

  /// @brief コンストラクタ
  /// @param[in] codec_type Coder の種類
  explicit
  FileODO(CodecType codec_type = kCodecThrough);

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
  /// @param[in] mode ファイル作成用のモード
  /// @param[in] opt 圧縮用のオプション
  bool
  open(const char* filename,
       mode_t mode = 0666,
       ymuint opt = 0);

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  /// @param[in] mode ファイル作成用のモード
  /// @param[in] opt 圧縮用のオプション
  bool
  open(const string& filename,
       mode_t mode = 0666,
       ymuint opt = 0);

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
  ymint64
  write(const ymuint8* buff,
	ymuint64 n);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル符号器
  FileCoder* mCoder;

};

END_NAMESPACE_YM

#endif // YMUTILS_FILEODO_H

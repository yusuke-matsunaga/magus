
/// @file FileODO.cc
/// @brief FileODO の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/FileODO.h"
#include "ym_utils/FileCoder.h"



BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス FileODO
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] codec_type Coder の種類
FileODO::FileODO(tCodecType codec_type)
{
  mCoder = FileCoder::new_coder(codec_type);
}

// @brief デストラクタ
FileODO::~FileODO()
{
  close();
  delete mCoder;
}

// @brief 書き込み可能なら true を返す．
FileODO::operator bool() const
{
  return mCoder->is_ready();
}

// @brief ファイルを開く
// @param[in] filename ファイル名
// @param[in] mode ファイル作成用のモード
// @param[in] opt 圧縮用のオプション
bool
FileODO::open(const char* filename,
	      mode_t mode,
	      ymuint opt)
{
  close();
  return mCoder->open(filename, mode, opt);
}

// @brief ファイルを開く
// @param[in] filename ファイル名
// @param[in] mode ファイル作成用のモード
// @param[in] opt 圧縮用のオプション
bool
FileODO::open(const string& filename,
	      mode_t mode,
	      ymuint opt)
{
  return open(filename.c_str(), mode, opt);
}

// @brief ファイルを閉じる．
// @note 以降の書き込みは行われない．
void
FileODO::close()
{
  mCoder->close();
}

// @brief データを書き出す．
// @param[in] buff データを収めた領域のアドレス
// @param[in] n データサイズ
// @return 実際に書き出した量を返す．
ssize_t
FileODO::write(const ymuint8* buff,
	       ymuint64 n)
{
  return mCoder->write(buff, n);
}

END_NAMESPACE_YM

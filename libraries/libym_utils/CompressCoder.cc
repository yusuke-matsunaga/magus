
/// @file CompressCoder.cc
/// @brief CompressCoder の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "CompressCoder.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス CompressCoder
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CompressCoder::CompressCoder()
{
}

// @brief デストラクタ
CompressCoder::~CompressCoder()
{
  close();
}

// @brief ファイルを開く
// @param[in] filename ファイル名
// @param[in] level 圧縮レベル
// @retval true 成功した
// @retval false 失敗した．
//
// 失敗する理由は以下の通り
//  - ファイルに対する書き込み許可がない．
bool
CompressCoder::open(const char* filename,
		    ymuint level)
{
  return mOut.open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
}

// @brief ファイルを閉じる．
void
CompressCoder::close()
{
  mOut.close();
}

// @brief 書き込み可能の時に true を返す．
bool
CompressCoder::is_ready() const
{
  return mOut.is_ready();
}

// @brief 最大 num バイトのデータを圧縮してファイルに書き込む．
// @param[in] wbuff 圧縮するデータを格納するバッファ
// @param[in] num 書き込むデータ数(バイト)
// @return 実際に書き込んだバイト数を返す．
// @note エラーが起こったら -1 を返す．
ssize_t
CompressCoder::write(const ymuint8* wbuff,
		     ymuint64 num)
{
  return mOut.write(wbuff, num);
}

END_NAMESPACE_YM

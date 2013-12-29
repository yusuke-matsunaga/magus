
/// @file ThruDecoder.cc
/// @brief CpressDecoder の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ThruDecoder.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス ThruDecoder
//////////////////////////////////////////////////////////////////////

/// @brief コンストラクタ
ThruDecoder::ThruDecoder()
{
}

// @brief デストラクタ
ThruDecoder::~ThruDecoder()
{
  close();
}

// @brief ファイルを開く
// @param[in] filename ファイル名
// @retval true 成功した
// @retval false 失敗した．
//
// 失敗する理由は以下の通り
//  - ファイルが存在しない．
//  - ファイルに対する読み出し許可がない．
//  - ファイルの形式が異なる．
bool
ThruDecoder::open(const char* filename)
{
  return mBuff.open(filename, O_RDONLY, 0);
}

// @brief ファイルを閉じる．
void
ThruDecoder::close()
{
  mBuff.close();
}

// @brief 読み出せるデータがある時に true を返す．
bool
ThruDecoder::is_ready() const
{
  return mBuff.is_ready();
}

// @brief 圧縮されたファイルを読んで最大 num バイトをバッファに格納する．
// @param[in] rbuff 展開したデータを格納するバッファ
// @param[in] num 読み出すデータ数(バイト)
// @return 実際に読み出したバイト数を返す．
// @note エラーが起こったら -1 を返す．
ssize_t
ThruDecoder::read(ymuint8* rbuff,
		  ymuint64 num)
{
  return mBuff.read(rbuff, num);
}

END_NAMESPACE_YM


/// @file CompressDecoder.cc
/// @brief CpressDecoder の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "CompressDecoder.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス CompressDecoder
//////////////////////////////////////////////////////////////////////

/// @brief コンストラクタ
CompressDecoder::CompressDecoder()
{
}

// @brief デストラクタ
CompressDecoder::~CompressDecoder()
{
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
CompressDecoder::open(const char* filename)
{
  return mIn.open(filename, O_RDONLY, 0);
}

// @brief ファイルを閉じる．
void
CompressDecoder::close()
{
  mIn.close();
}

// @brief 読み出せるデータがある時に true を返す．
bool
CompressDecoder::is_ready() const
{
  return mIn.is_ready();
}

// @brief 圧縮されたファイルを読んで最大 num バイトをバッファに格納する．
// @param[in] rbuff 展開したデータを格納するバッファ
// @param[in] num 読み出すデータ数(バイト)
// @return 実際に読み出したバイト数を返す．
// @note エラーが起こったら -1 を返す．
ssize_t
CompressDecoder::read(ymuint8* rbuff,
		      ymuint64 num)
{
  return mIn.read(rbuff, num);
}

END_NAMESPACE_YM

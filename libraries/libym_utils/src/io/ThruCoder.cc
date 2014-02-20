
/// @file ThruCoder.cc
/// @brief ThruCoder の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "ThruCoder.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス ThruCoder
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ThruCoder::ThruCoder()
{
}

// @brief デストラクタ
ThruCoder::~ThruCoder()
{
  close();
}

// @brief ファイルを開く
// @param[in] filename ファイル名
// @param[in] mode ファイル作成用のモード
// @param[in] level 圧縮レベル
// @retval true 成功した
// @retval false 失敗した．
//
// 失敗する理由は以下の通り
//  - ファイルに対する書き込み許可がない．
bool
ThruCoder::open(const char* filename,
		mode_t mode,
		ymuint level)
{
  return mBuff.open(filename, O_WRONLY | O_CREAT | O_TRUNC, mode);
}

// @brief ファイルを閉じる．
void
ThruCoder::close()
{
  mBuff.close();
}

// @brief 書き込み可能の時に true を返す．
bool
ThruCoder::is_ready() const
{
  return mBuff.is_ready();
}

// @brief 最大 num バイトのデータを圧縮してファイルに書き込む．
// @param[in] wbuff 圧縮するデータを格納するバッファ
// @param[in] num 書き込むデータ数(バイト)
// @return 実際に書き込んだバイト数を返す．
// @note エラーが起こったら -1 を返す．
ssize_t
ThruCoder::write(const ymuint8* wbuff,
		 ymuint64 num)
{
  return mBuff.write(wbuff, num);
}

END_NAMESPACE_YM

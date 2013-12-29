#ifndef ZCODER_H
#define ZCODER_H

/// @file ZCoder.h
/// @brief ZCoder のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/FileCoder.h"
#include "FileBuff.h"
#include "compress_common.h"


BEGIN_NAMESPACE_YM_COMPCOMMON

//////////////////////////////////////////////////////////////////////
/// @class ZCoder ZCoder.h "ZCoder.h"
/// @brief compress(Z) 形式の圧縮器
//////////////////////////////////////////////////////////////////////
class ZCoder :
  public FileCoder
{
public:

  /// @brief コンストラクタ
  ZCoder();

  /// @brief デストラクタ
  virtual
  ~ZCoder();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  /// @param[in] mode ファイル作成用のモード
  /// @param[in] level 圧縮レベル
  /// @retval true 成功した
  /// @retval false 失敗した．
  ///
  /// 失敗する理由は以下の通り
  ///  - ファイルに対する書き込み許可がない．
  virtual
  bool
  open(const char* filename,
       mode_t mode = 0666,
       ymuint level = 0);

  /// @brief ファイルを閉じる．
  virtual
  void
  close();

  /// @brief 書き込み可能の時に true を返す．
  virtual
  bool
  is_ready() const;

  /// @brief 最大 num バイトのデータを圧縮してファイルに書き込む．
  /// @param[in] wbuff 圧縮するデータを格納するバッファ
  /// @param[in] num 書き込むデータ数(バイト)
  /// @return 実際に書き込んだバイト数を返す．
  /// @note エラーが起こったら -1 を返す．
  virtual
  ssize_t
  write(const ymuint8* wbuff,
	ymuint64 num);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  int
  cl_block();

  void
  cl_hash(count_int hsize);

  int
  output(code_int code);

  count_int&
  htabof(ymuint i);

  u_short&
  codetabof(ymuint i);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // コードあたりのビット数
  ymuint32 m_n_bits;

  // n_bits の最大値
  ymuint32 m_maxbits;

  // Maximum code
  code_int m_maxcode;

  // Should NEVER generate this code
  code_int m_maxmaxcode;

  tState m_state;

  code_int m_free_ent;

  ymint32 m_block_compress;

  ymint32 m_clear_flg;

  count_int m_htab[k_HSIZE];

  u_short m_codetab[k_HSIZE];

  ymlong m_fcode;

  code_int m_ent;

  code_int m_hsize_reg;

  ymint32 m_hshift;

  ymuint32 m_offset;

  code_int m_hsize;

  ymlong m_ratio;

  count_int m_checkpoint;

  count_int m_in_count;

  count_int m_out_count;

  ymlong m_bytes_out;

  char_type m_buf[k_BITS];

  // ファイルバッファ
  FileBuff mBuff;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

inline
count_int&
ZCoder::htabof(ymuint i)
{
  return m_htab[i];
}

inline
u_short&
ZCoder::codetabof(ymuint i)
{
  return m_codetab[i];
}


END_NAMESPACE_YM_COMPCOMMON

BEGIN_NAMESPACE_YM

using nsCompCommon::ZCoder;

END_NAMESPACE_YM

#endif // ZCODER_H

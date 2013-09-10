#ifndef COMPO_H
#define COMPO_H

/// @file CompO.h
/// @brief ZState のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "CompBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class CompO CompO.h "CompO.h"
/// @brief compress 用の ZState
//////////////////////////////////////////////////////////////////////
class CompO :
  public CompBase
{
public:

  /// @brief コンストラクタ
  explicit
  CompO(ymuint bits = 0);

  /// @brief デストラクタ
  ~CompO();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルをクローズする．
  void
  close();

  /// @brief データを圧縮して書き込む．
  /// @param[in] wbuff 書き込むデータを格納したバッファ
  /// @param[in] num データ(バイト)
  /// @return 実際に処理したバイト数を返す．
  /// @note エラーが起こったら -1 を返す．
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

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

inline
CompBase::count_int&
CompO::htabof(ymuint i)
{
  return m_htab[i];
}

inline
u_short&
CompO::codetabof(ymuint i)
{
  return m_codetab[i];
}

END_NAMESPACE_YM

#endif // COMPO_H

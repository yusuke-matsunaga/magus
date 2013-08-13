#ifndef ZSTATE_H
#define ZSTATE_H

/// @file ZState.h
/// @brief ZState のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

class FileBuff;

//////////////////////////////////////////////////////////////////////
/// @class ZStateBase ZState.h "ZState.h"
/// @brief compress/uncompress 用の s_zstate をクラス化したもの
//////////////////////////////////////////////////////////////////////
class ZStateBase
{
public:


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief buff[0] - buff[num - 1] の内容を書き込む．
  /// @param[in] buff データを格納したバッファ
  /// @param[in] num 書き込むバイト数
  /// @return 実際に書き込んだバイト数を返す．
  ssize_t
  _write(ymuint8* buff,
	 ymuint num);

  /// @brief num バイトを読み込み buff[] に格納する．
  /// @param[in] buff データを格納するバッファ
  /// @param[in] num 読み込むバイト数．
  /// @return 実際に読み込んだバイト数を返す．
  ssize_t
  _read(ymuint8* buff,
	ymuint num);


protected:
  //////////////////////////////////////////////////////////////////////
  // 定数
  //////////////////////////////////////////////////////////////////////

  // Default bits
  static
  const ymuint8 k_BITS = 16;

  // 95 % occupancy
  static
  const ymuint32 k_HSIZE = 69001;

  // magic number
  static
  const ymuint8 k_MAGICHEADER[2] = { '\037', '\235' };

  static
  const ymuint8 k_BIT_MASK = 0x1f;

  static
  const ymuint8 k_BLOCK_MASK = 0x80;

  // Initial number of bits/code
  static
  const ymuint32 k_INIT_BITS = 9;

  // First free entry
  static
  code_int k_FIRST 257

  // Table clear output code
  static
  code_int k_CLEAR 256

  // バッファサイズ
  static
  const ymuint32 kBuffSize;


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイルバッファ
  FileBuff* mFileBuff;

  // 状態
  enum tState {
    kStart,  // 開始
    kMiddle, // 処理中
    kEof     // 末尾
  } m_state;

  // コードあたりのビット数
  ymuint32 m_n_bits;

  // n_bits の最大値
  ymuint32 m_maxbits;

  // Maximum code
  code_int m_maxcode;

  // Should NEVER generate this code
  code_int m_maxmaxcode;

  count_int m_htab[k_HSIZE];

  ymuint8 m_codetab[k_HSIZE];

  code_int m_hsize;

  code_int m_free_ent;

  ymint32 m_block_compress;

  ymint32 m_clear_flg;

  ymlong m_ratio;

  count_int m_checkpoint;

  ymuint32 m_offset;

  ymlong m_in_count;

  ymlong m_bytes_out;

  ymlong m_out_count;

  ymuint8 m_buf[k_BITS];

};


//////////////////////////////////////////////////////////////////////
/// @class ZStateW ZState.h "ZState.h"
/// @brief compress 用の ZState
//////////////////////////////////////////////////////////////////////
class ZStateW :
  public ZStateBase
{
public:

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  ymlong m_fcode;

  code_int m_ent;

  code_int m_hsize_reg;

  ymint32 m_hshift;

};


//////////////////////////////////////////////////////////////////////
/// @class ZStateR ZState.h "ZState.h"
/// @brief uncompress 用の ZState
//////////////////////////////////////////////////////////////////////
class ZStateR :
  public ZStateBase
{
public:

  /// @brief コンストラクタ
  ZStateR();

  /// @brief デストラクタ
  ~ZStateR();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 圧縮されたファイルを読んで最大 num バイトをバッファに格納する．
  /// @param[in] rbuff 展開したデータを格納するバッファ
  /// @param[in] num 読み出すデータ数(バイト)
  /// @return 実際に読み出したバイト数を返す．
  /// @note エラーが起こったら -1 を返す．
  int
  read(ymuint8* rbuff,
       int num);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  ymuint8&
  tab_prefixof(ymuint i);

  ymuint8&
  tab_suffixof(ymuinti);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  ymuint8* m_stackp

  ymuint32 m_finchar;

  code_int m_oldcode;

  code_int m_incode;

  ymint32 m_roffset;

  ymint32 m_size;

  ymuint8 m_gbuf[k_BITS];

};

inline
ymuint32
MAXCODE(ymuint32 n_bits)
{
  return (1UL << n_bits) - 1;
}

inline
ymuint8&
ZStateR::tab_prefixof(ymuint i)
{
  return m_codetab[i];
}

inline
ymuint8&
ZStateR::tab_suffixof(ymuint i)
{
  return static_cast<ymuint8*>(&m_htab)[i];
}

END_NAMESPACE_YM

#endif // ZSTATE_H

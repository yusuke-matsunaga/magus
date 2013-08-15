#ifndef ZSTATE_H
#define ZSTATE_H

/// @file ZState.h
/// @brief ZState のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "FileBuff.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class ZStateBase ZState.h "ZState.h"
/// @brief compress/uncompress 用の s_zstate をクラス化したもの
//////////////////////////////////////////////////////////////////////
class ZStateBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // 型の定義
  //////////////////////////////////////////////////////////////////////

  typedef long code_int;
  typedef long count_int;
  typedef ymuint8 char_type;

public:

  /// @brief コンストラクタ
  explicit
  ZStateBase(int bits = 0);

  /// @brief デストラクタ
  ~ZStateBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルをオープンする．
  /// @param[in] filename ファイル名
  /// @param[in] flags フラグ
  /// @param[in] mode モード
  /// @retval true オープンが成功した．
  /// @retval false オープンが失敗した．
  bool
  open(const char* filename,
       int flags,
       mode_t mode = 0);

  /// @brief ファイルをクローズする．
  void
  close();


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief buff[0] - buff[num - 1] の内容を書き込む．
  /// @param[in] buff データを格納したバッファ
  /// @param[in] num 書き込むバイト数
  /// @return 実際に書き込んだバイト数を返す．
  ssize_t
  _write(const ymuint8* buff,
	 ymuint num);

  /// @brief num バイトを読み込み buff[] に格納する．
  /// @param[in] buff データを格納するバッファ
  /// @param[in] num 読み込むバイト数．
  /// @return 実際に読み込んだバイト数を返す．
  ssize_t
  _read(ymuint8* buff,
	ymuint num);

  count_int&
  htabof(ymuint i);

  ymuint8&
  codetabof(ymuint i);

  char_type&
  tab_prefixof(ymuint i);

  char_type&
  tab_suffixof(ymuint i);

  char_type*
  de_stack();

  void
  cl_hash(count_int hsize);


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

  static
  const char_type k_BIT_MASK = 0x1f;

  static
  const char_type k_BLOCK_MASK = 0x80;

  // Initial number of bits/code
  static
  const ymuint32 k_INIT_BITS = 9;

  // Ratio check interval
  static
  const count_int k_CHECK_GAP = 10000;

  // First free entry
  static
  const code_int k_FIRST = 257;

  // Table clear output code
  static
  const code_int k_CLEAR = 256;


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

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

  code_int m_free_ent;

  ymint32 m_block_compress;

  ymint32 m_clear_flg;


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートデータメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイルバッファ
  FileBuff mFileBuff;

  count_int m_htab[k_HSIZE];

  ymuint8 m_codetab[k_HSIZE];

};


//////////////////////////////////////////////////////////////////////
/// @class ZStateW ZState.h "ZState.h"
/// @brief compress 用の ZState
//////////////////////////////////////////////////////////////////////
class ZStateW :
  public ZStateBase
{
public:

  /// @brief コンストラクタ
  ZStateW(int bits = 0);

  /// @brief デストラクタ
  ~ZStateW();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief データを圧縮して書き込む．
  /// @param[in] wbuff 書き込むデータを格納したバッファ
  /// @param[in] num データ(バイト)
  /// @return 実際に処理したバイト数を返す．
  /// @note エラーが起こったら -1 を返す．
  int
  write(ymuint8* wbuff,
	int num);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  int
  cl_block();

  int
  output(code_int code);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

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

  code_int
  getcode();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  char_type* m_stackp;

  ymint32 m_finchar;

  code_int m_oldcode;

  code_int m_incode;

  ymint32 m_roffset;

  ymint32 m_size;

  char_type m_gbuf[k_BITS];

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

inline
ymuint32
MAXCODE(ymuint32 n_bits)
{
  return (1UL << n_bits) - 1;
}

// @brief ファイルをオープンする．
// @param[in] filename ファイル名
// @param[in] flags フラグ
// @param[in] mode モード
// @retval true オープンが成功した．
// @retval false オープンが失敗した．
inline
bool
ZStateBase::open(const char* filename,
		 int flags,
		 mode_t mode)
{
  return mFileBuff.open(filename, flags, mode);
}

// @brief ファイルをクローズする．
inline
void
ZStateBase::close()
{
  mFileBuff.close();
}

// @brief buff[0] - buff[num - 1] の内容を書き込む．
// @param[in] buff データを格納したバッファ
// @param[in] num 書き込むバイト数
// @return 実際に書き込んだバイト数を返す．
inline
ssize_t
ZStateBase::_write(const ymuint8* buff,
		   ymuint num)
{
  return mFileBuff.write(buff, num);
}

// @brief num バイトを読み込み buff[] に格納する．
// @param[in] buff データを格納するバッファ
// @param[in] num 読み込むバイト数．
// @return 実際に読み込んだバイト数を返す．
inline
ssize_t
ZStateBase::_read(ymuint8* buff,
		  ymuint num)
{
  return mFileBuff.read(buff, num);
}

inline
ZStateBase::count_int&
ZStateBase::htabof(ymuint i)
{
  return m_htab[i];
}

inline
ZStateBase::char_type&
ZStateBase::codetabof(ymuint i)
{
  return m_codetab[i];
}

inline
ZStateBase::char_type&
ZStateBase::tab_prefixof(ymuint i)
{
  return m_codetab[i];
}

inline
ZStateBase::char_type&
ZStateBase::tab_suffixof(ymuint i)
{
  return (reinterpret_cast<char_type*>(&m_htab))[i];
}

inline
ZStateBase::char_type*
ZStateBase::de_stack()
{
  return &tab_suffixof(1 << k_BITS);
}

END_NAMESPACE_YM

#endif // ZSTATE_H

#ifndef COMPBASE_H
#define COMPBASE_H

/// @file CompBase.h
/// @brief CompBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "FileBuff.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class CompBase CompBase.h "CompBase.h"
/// @brief compress/uncompress 用の s_zstate をクラス化したもの
//////////////////////////////////////////////////////////////////////
class CompBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // 型の定義
  //////////////////////////////////////////////////////////////////////

  typedef long code_int;
  typedef long count_int;
  typedef u_char char_type;

public:

  /// @brief コンストラクタ
  explicit
  CompBase(ymuint bits = 0);

  /// @brief デストラクタ
  ~CompBase();


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

  /// @brief 適正な状態の時 true を返す．
  bool
  is_ready() const;


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
	 size_t num);

  /// @brief num バイトを読み込み buff[] に格納する．
  /// @param[in] buff データを格納するバッファ
  /// @param[in] num 読み込むバイト数．
  /// @return 実際に読み込んだバイト数を返す．
  ssize_t
  _read(ymuint8* buff,
	size_t num);


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
CompBase::open(const char* filename,
	       int flags,
	       mode_t mode)
{
  return mFileBuff.open(filename, flags, mode);
}

// @brief ファイルをクローズする．
inline
void
CompBase::close()
{
  mFileBuff.close();
}

// @brief 適正な状態の時 true を返す．
inline
bool
CompBase::is_ready() const
{
  return mFileBuff.is_ready();
}

// @brief buff[0] - buff[num - 1] の内容を書き込む．
// @param[in] buff データを格納したバッファ
// @param[in] num 書き込むバイト数
// @return 実際に書き込んだバイト数を返す．
inline
ssize_t
CompBase::_write(const ymuint8* buff,
		 size_t num)
{
  return mFileBuff.write(buff, num);
}

// @brief num バイトを読み込み buff[] に格納する．
// @param[in] buff データを格納するバッファ
// @param[in] num 読み込むバイト数．
// @return 実際に読み込んだバイト数を返す．
inline
ssize_t
CompBase::_read(ymuint8* buff,
		size_t num)
{
  return mFileBuff.read(buff, num);
}

END_NAMESPACE_YM

#endif // COMPBASE_H

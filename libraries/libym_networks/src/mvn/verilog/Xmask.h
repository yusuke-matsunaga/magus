#ifndef XMASK_H
#define XMASK_H

/// @file Xmask.h
/// @brief Xmask のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/mvn.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class Xmask Xmask.h "Xmask.h"
/// @brief ドントケアマスクを表すクラス
//////////////////////////////////////////////////////////////////////
class Xmask
{
public:

  /// @brief 空のコンストラクタ
  /// @note ビット幅は0で初期化される．
  Xmask();

  /// @brief ビット幅を指定したコンストラクタ
  /// @note この時点では mBits はアロケートされない．
  Xmask(ymuint bit_width);

  /// @brief コピーコンストラクタ
  Xmask(const Xmask& src);

  /// @brief 代入演算子
  const Xmask&
  operator=(const Xmask& src);

  /// @brief デストラクタ
  ~Xmask();


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ビット幅を再設定する．
  /// @param[in] bit_width 設定するビット幅
  /// @note 以前の内容はクリアされる．
  void
  set_bit_width(ymuint bit_width);

  /// @brief 全ビットをセットする．
  void
  set_all();

  /// @brief 全ビットをクリアする．
  void
  clear_all();

  /// @brief 全ビットを同時に設定する．
  /// @param[in] val 設定する値
  void
  change_all(bool val);

  /// @brief 指定された位置にビットをセットする．
  /// @param[in] pos 位置番号 ( 0 <= pos < bit_width() )
  void
  set_bit(ymuint pos);

  /// @brief 指定された位置にビットをクリアする．
  /// @param[in] pos 位置番号 ( 0 <= pos < bit_width() )
  void
  clear_bit(ymuint pos);

  /// @brief 指定された位置のビットを設定する．
  /// @param[in] pos 位置番号 ( 0 <= pos < bit_width() )
  /// @param[in] val 設定する値
  void
  change_bit(ymuint pos,
	     bool val);


public:
  //////////////////////////////////////////////////////////////////////
  // 演算子の定義
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理和を取る．
  /// @param[in] right オペランド
  /// @return 自分自身の参照を返す．
  const Xmask&
  operator|=(const Xmask& right);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ビット幅を得る．
  ymuint
  bit_width() const;

  /// @brief 指定された位置の値を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < bit_width() )
  /// @retval true マスクあり(ドントケア)
  /// @retval false マスクなし
  bool
  operator[](const ymuint pos) const;

  /// @brief ドントケアビットを含むときに true を返す．
  bool
  has_x() const;

  /// @brief ymuint32 のベクタに変換する．
  void
  to_vector(vector<ymuint32>& vect) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief mBits のサイズを得る．
  ymuint
  blksize() const;

  /// @brief 指定された位置のブロック番号を得る．
  ymuint
  blkpos(ymuint pos) const;

  /// @brief 指定された位置のシフト量を得る．
  ymuint
  shift(ymuint pos) const;

  /// @brief 指定された位置のマスクを得る．
  ymuint
  bitmask(ymuint pos) const;

  /// @brief メモリ領域を確保する．
  void
  alloc();

  /// @brief メモリ領域を解放する．
  void
  free();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ビット幅
  ymuint32 mBitWidth;

  // マスクを表す本体
  ymuint32* mBits;

};

/// @brief Xmask の論理和を計算する．
/// @relates Xmask
/// @param[in] left, right オペランド
Xmask
operator|(const Xmask& left,
	  const Xmask& right);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ビット幅を得る．
inline
ymuint
Xmask::bit_width() const
{
  return mBitWidth;
}

// @brief 全ビットを同時に設定する．
// @param[in] val 設定する値
inline
void
Xmask::change_all(bool val)
{
  if ( val ) {
    set_all();
  }
  else {
    clear_all();
  }
}

// @brief 指定された位置にビットをセットする．
// @param[in] pos 位置番号 ( 0 <= pos < bit_width() )
inline
void
Xmask::set_bit(ymuint pos)
{
  alloc();
  ymuint blk = blkpos(pos);
  ymuint32 mask = bitmask(pos);
  mBits[blk] |= mask;
}

// @brief 指定された位置にビットをクリアする．
// @param[in] pos 位置番号 ( 0 <= pos < bit_width() )
inline
void
Xmask::clear_bit(ymuint pos)
{
  if ( mBits ) {
    ymuint blk = blkpos(pos);
    ymuint32 mask = bitmask(pos);
    mBits[blk] &= ~mask;
  }
}

// @brief 指定された位置のビットを設定する．
// @param[in] pos 位置番号 ( 0 <= pos < bit_width() )
// @param[in] val 設定する値
inline
void
Xmask::change_bit(ymuint pos,
		  bool val)
{
  if ( val ) {
    set_bit(pos);
  }
  else {
    clear_bit(pos);
  }
}

// @brief mBits のサイズを得る．
inline
ymuint
Xmask::blksize() const
{
  return (bit_width() + 31) / 32;
}

// @brief 指定された位置のブロック番号を得る．
inline
ymuint
Xmask::blkpos(ymuint pos) const
{
  return pos / 32;
}

// @brief 指定された位置のシフト量を得る．
inline
ymuint
Xmask::shift(ymuint pos) const
{
  return pos % 32;
}

// @brief 指定された位置のマスクを得る．
inline
ymuint
Xmask::bitmask(ymuint pos) const
{
  return (1U << shift(pos));
}

// @brief 指定された位置の値を返す．
// @param[in] pos 位置番号 ( 0 <= pos < bit_width() )
// @retval true マスクあり(ドントケア)
// @retval false マスクなし
inline
bool
Xmask::operator[](const ymuint pos) const
{
  if ( mBits ) {
    ymuint blk = blkpos(pos);
    ymuint sft = shift(pos);
    return static_cast<bool>((mBits[blk] >> sft) & 1U);
  }
  return false;
}

// @brief Xmask の論理和を計算する．
// @relates Xmask
// @param[in] left, right オペランド
inline
Xmask
operator|(const Xmask& left,
	  const Xmask& right)
{
  return Xmask(left).operator|=(right);
}

END_NAMESPACE_YM_NETWORKS_MVN_VERILOG

#endif // XMASK_H

#ifndef NPNXFORM_H
#define NPNXFORM_H

/// @file NpnXform.h
/// @brief NpnXform のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class NpnXform NpnXform.h "NpnXform.h"
/// @brief NPN 変換を表すクラス(4入力限定)
//////////////////////////////////////////////////////////////////////
class NpnXform
{
public:

  /// @brief 空のコンストラクタ
  /// @note 恒等変換になる．
  NpnXform();

  /// @brief 順列番号と極性ビットベクタを指定したコンストラクタ
  /// @param[in] pid 順列番号 ( 0 <= pid < 24 )
  /// @param[in] pols 極性ビットベクタ ( 0 <= pols < 32 )
  NpnXform(ymuint pid,
	   ymuint pols);

  /// @brief 生のデータを引数にしたコンストラクタ
  /// @note 危険
  explicit
  NpnXform(ymuint data);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力の置換を得る．
  /// @param[in] pos 元の入力位置 ( 0 <= pos < 4 )
  /// @return 置換先の位置
  ymuint
  input_perm(ymuint pos) const;

  /// @brief 入力の極性を得る．
  /// @param[in] pos 元の入力位置 ( 0 <= pos < 4 )
  /// @retval true 反転あり
  /// @retval false 反転なし
  bool
  input_inv(ymuint pos) const;

  /// @brief 出力の極性を得る．
  bool
  output_inv() const;

  /// @brief 生のデータを取り出す．
  /// @note 値域は 0 - 1023
  ymuint
  data() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力の反転属性を反転させる．
  void
  flip_oinv();


public:
  //////////////////////////////////////////////////////////////////////
  // クラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力の順列から順列番号を得る．
  static
  ymuint
  perm_id(ymuint perm[]);


public:
  //////////////////////////////////////////////////////////////////////
  // 演算
  //////////////////////////////////////////////////////////////////////

  /// @brief 合成する．
  friend
  NpnXform
  operator*(const NpnXform& left,
	    const NpnXform& right);

  /// @brief 逆変換を求める．
  friend
  NpnXform
  inverse(const NpnXform& left);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実体
  ymuint16 mData;

};


/// @brief 関数ベクタを変換する．
ymuint16
xform(ymuint16 func,
      NpnXform xf);

/// @brief 内容を表示する．
ostream&
operator<<(ostream& s,
	   NpnXform xf);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
// @note 恒等変換になる．
inline
NpnXform::NpnXform() :
  mData(0)
{
}

// @brief 順列番号と極性ビットベクタを指定したコンストラクタ
// @param[in] pid 順列番号 ( 0 <= pid < 24 )
// @param[in] pols 極性ビットベクタ ( 0 <= pols < 32 )
inline
NpnXform::NpnXform(ymuint pid,
		   ymuint pols) :
  mData((pid << 5) | pols)
{
}

// @brief 生のデータを引数にしたコンストラクタ
// @note 危険
inline
NpnXform::NpnXform(ymuint data) :
  mData(data)
{
}

// @brief 入力の極性を得る．
// @param[in] pos 元の入力位置 ( 0 <= pos < 4 )
// @retval true 反転あり
// @retval false 反転なし
inline
bool
NpnXform::input_inv(ymuint pos) const
{
  return static_cast<bool>((mData >> pos) & 1U);
}

// @brief 出力の極性を得る．
inline
bool
NpnXform::output_inv() const
{
  return static_cast<bool>((mData >> 4) & 1U);
}

// @brief 生のデータを取り出す．
inline
ymuint
NpnXform::data() const
{
  return mData;
}

// @brief 出力の反転属性を反転させる．
inline
void
NpnXform::flip_oinv()
{
  mData ^= 16U;
}

END_NAMESPACE_YM

#endif // NPNXFORM_H

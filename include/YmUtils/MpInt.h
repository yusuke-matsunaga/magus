#ifndef YMUTILS_MPINT_H
#define YMUTILS_MPINT_H

/// @file YmUtils/MpInt.h
/// @brief MpInt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class MpInt MpInt.h "YmUtils/MpInt.h"
/// @brief 多倍長精度整数のクラス
//////////////////////////////////////////////////////////////////////
class MpInt
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 値は不定
  MpInt();

  /// @brief 整数からの変換コンストラクタ
  /// @param[in] val 値
  explicit
  MpInt(int val);

  /// @brief コピーコンストラクタ
  /// @param[in] val 値
  MpInt(const MpInt& val);

  /// @brief 符号反転フラグ付きのコピーコンストラクタ
  /// @param[in] val 値
  /// @param[in] inv 符号反転フラグ(true で反転)
  MpInt(const MpInt& val,
	bool inv);

  /// @brief 代入演算子
  /// @param[in] val 値
  const MpInt&
  operator=(const MpInt& val);

  /// @brief デストラクタ
  ~MpInt();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ブロック数を返す．
  ///
  /// 1つのブロックは64ビット
  ymuint
  block_num() const;

  /// @brief ブロックの内容を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < block_num() )
  ///
  /// 内部では絶対値表現を用いている．
  ymuint64
  block(ymuint pos) const;

  /// @brief 正の時 true を返す．
  ///
  /// 0 も正とみなす．
  bool
  is_positive() const;

  /// @brief 負の時 true を返す．
  bool
  is_negative() const;

  /// @brief 絶対値を返す．
  MpInt
  abs() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 計算を行うメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号反転を行い自身に代入する．
  /// @return 結果を返す．
  const MpInt&
  complement();

  /// @brief 加算付き代入
  /// @param[in] right 右辺
  /// @return 結果を返す．
  const MpInt&
  operator+=(const MpInt& right);

  /// @brief 減算付き代入
  /// @param[in] right 右辺
  /// @return 結果を返す．
  const MpInt&
  operator-=(const MpInt& right);

  /// @brief 乗算付き代入
  /// @param[in] right 右辺
  /// @return 結果を返す．
  const MpInt&
  operator*=(const MpInt& right);

  /// @brief 左シフト付き代入
  /// @param[in] shamt シフト量
  /// @return 結果を返す．
  const MpInt&
  operator<<=(ymuint shamt);

  /// @brief 右シフト付き代入
  /// @param[in] shamt シフト量
  /// @return 結果を返す．
  const MpInt&
  operator>>=(ymuint shamt);

  /// @brief 大小比較を行う関数
  /// @param[in] left, right オペランド
  /// @retval -1 left < right
  /// @retval  0 left = right
  /// @retval  1 left > right
  static
  int
  compare(const MpInt& left,
	  const MpInt& right);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 絶対値の大小比較を行う関数
  /// @param[in] left, right オペランド
  /// @retval -1 left < right
  /// @retval  0 left = right
  /// @retval  1 left > right
  static
  int
  abs_compare(const MpInt& left,
	      const MpInt& right);

  /// @brief 絶対値の加算を行う関数
  /// @param[in] left, right オペランド
  /// @param[out] block_list 結果を格納するリスト
  static
  void
  abs_add(const MpInt& left,
	  const MpInt& right,
	  vector<ymuint64>& block_list);

  /// @brief 絶対値の減算を行うサブルーティン
  /// @param[in] left, right オペランド
  /// @param[out] block_list 結果を格納するリスト
  static
  void
  abs_sub(const MpInt& left,
	  const MpInt& right,
	  vector<ymuint64>& block_list);

  /// @brief 演算結果を作るときに用いられるコンストラクタ
  /// @param[in] block_list ブロックリスト
  /// @param[in] inv 結果に対する符号反転フラグ
  MpInt(const vector<ymuint64>& block_list,
	bool inv);

  /// @brief 内容を設定する関数
  /// @param[in] block_list ブロックリスト
  /// @param[in] inv 結果に対する符号反転フラグ
  void
  set(const vector<ymuint64>& block_list,
      bool inv);

  /// @brief ブロックを確保する．
  /// @param[in] nb ブロック数
  ///
  /// 現在のブロック数が nb と等しい時はなにもしない．
  /// いかなる時も内容は初期化されない．
  void
  alloc_block(ymuint nb);

  /// @brief 符号ビットをセットする．
  void
  set_sign(bool sign);


  //////////////////////////////////////////////////////////////////////
  // フレンド関数の宣言
  //////////////////////////////////////////////////////////////////////

  friend
  MpInt
  operator-(const MpInt& opr);

  friend
  MpInt
  operator+(const MpInt& left,
	    const MpInt& right);

  friend
  MpInt
  operator-(const MpInt& left,
	    const MpInt& right);

  friend
  MpInt
  operator*(const MpInt& left,
	    const MpInt& right);

  friend
  MpInt
  operator<<(const MpInt& left,
	     ymuint shamt);

  friend
  MpInt
  operator>>(const MpInt& left,
	     ymuint shamt);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ブロック数+符号ビット
  // 符号ビットは最下位
  ymuint32 mBlockNum;

  // ブロックの配列
  ymuint64* mBlockArray;

};


//////////////////////////////////////////////////////////////////////
// MpInt の演算子
//////////////////////////////////////////////////////////////////////

/// @relates MpInt
/// @param[in] opr オペランド
/// @brief 符号反転を行う．
MpInt
operator-(const MpInt& opr);

/// @brief 加算を行う．
/// @param[in] left, right オペランド
MpInt
operator+(const MpInt& left,
	  const MpInt& right);

/// @brief 減算を行う．
/// @param[in] left, right オペランド
MpInt
operator-(const MpInt& left,
	  const MpInt& right);

/// @brief 乗算を行う．
/// @param[in] left, right オペランド
MpInt
operator*(const MpInt& left,
	  const MpInt& right);

/// @brief 左シフト演算を行う．
/// @param[in] left オペランド
/// @param[in] shamt シフト量
MpInt
operator<<(const MpInt& left,
	   ymuint shamt);

/// @brief 右シフト演算を行う．
/// @param[in] left オペランド
/// @param[in] shamt シフト量
MpInt
operator>>(const MpInt& left,
	   ymuint shamt);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ブロック数を返す．
//
// 1つのブロックは64ビット
inline
ymuint
MpInt::block_num() const
{
  return mBlockNum >> 1;
}

// @brief ブロックの内容を返す．
// @param[in] pos 位置番号 ( 0 <= pos < block_num() )
inline
ymuint64
MpInt::block(ymuint pos) const
{
  ASSERT_COND( pos < block_num() );
  return mBlockArray[pos];
}

// @brief 正の時 true を返す．
//
// 0 も正とみなす．
inline
bool
MpInt::is_positive() const
{
  return !is_negative();
}

// @brief 負の時 true を返す．
inline
bool
MpInt::is_negative() const
{
  return static_cast<bool>(mBlockNum & 1U);
}

// @brief 絶対値を返す．
inline
MpInt
MpInt::abs() const
{
  return MpInt(*this, is_negative());
}

// @relates MpInt
// @param[in] opr オペランド
// @brief 符号反転を行う．
inline
MpInt
operator-(const MpInt& opr)
{
  return MpInt(opr).complement();
}

// @brief 大小比較を行う関数
// @param[in] left, right オペランド
// @retval -1 left < right
// @retval  0 left = right
// @retval  1 left > right
inline
int
MpInt::compare(const MpInt& left,
	       const MpInt& right)
{
  if ( left.is_positive() ) {
    if ( right.is_negative() ) {
      return 1;
    }
    else {
      return abs_compare(left, right);
    }
  }
  else {
    if ( right.is_negative() ) {
      return abs_compare(right, left);
    }
    else {
      return -1;
    }
  }
}

// @brief 演算結果を作るときに用いられるコンストラクタ
// @param[in] block_list ブロックリスト
// @param[in] inv 結果に対する符号反転フラグ
inline
MpInt::MpInt(const vector<ymuint64>& block_list,
	     bool inv)
{
  mBlockNum = 0;
  mBlockArray = NULL;
  set(block_list, inv);
}

END_NAMESPACE_YM

#endif // YMUTILS_MPINT_H

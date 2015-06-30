#ifndef SEAL_TESTVECTOR_H
#define SEAL_TESTVECTOR_H

/// @file include/TestVectort.h
/// @brief TestVector のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TestVector.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

#include "seal_nsdef.h"
#include "seal_utils.h"
#include <YmUtils/RandGen.h>


BEGIN_NAMESPACE_YM_SEAL

class SaFault;

//////////////////////////////////////////////////////////////////////
/// @class TestVector TestVector.h "TestVector.h"
/// @brief テストベクタを表すクラス
//////////////////////////////////////////////////////////////////////
class TestVector
{
public:

  /// @brief テストベクタを生成する．
  static
  TestVector*
  new_vector(ymuint ni);

  /// @brief テストベクタを削除する．
  static
  void
  delete_vector(TestVector* tv);


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を得る．
  ymuint
  input_num() const;

  /// @brief pos 番めの値を得る．
  /// @note 返り値は 0/1 のいずれか
  int
  val(ymuint pos) const;

  /// @brief 内容を BIN 形式で出力する．
  void
  dump_bin(ostream& s) const;

  /// @brief 内容を HEX 形式で出力する．
  void
  dump_hex(ostream& s) const;


public:

  /// @breif pos 番めの値を設定する．
  /// @note val は 0 か非0 かで区別される．
  void
  set_val(ymuint pos,
	  int val);

  /// @brief HEX文字列から内容を設定する．
  /// @param[in] hex_string HEX 文字列
  /// @return hex_string に不適切な文字が含まれていたら false を返す．
  /// @note hex_string が短い時には残りは0で初期化される．
  /// @note hex_string が長い時には余りは捨てられる．
  bool
  set_from_hex(const string& hex_string);

  /// @brief 乱数パタンを設定する．
  /// @param[in] randgen 乱数生成器
  void
  set_from_random(RandGen& randgen);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ブロック数を返す．
  static
  ymuint
  block_num(ymuint ni);

  /// @brief HEX文字列の長さを返す．
  static
  ymuint
  hex_length(ymuint ni);

  // 入力位置からブロック番号を得る．
  static
  ymuint
  block_idx(ymuint ipos);

  // 入力位置からシフト量を得る．
  static
  ymuint
  shift_num(ymuint ipos);



private:
  //////////////////////////////////////////////////////////////////////
  // 特殊なアロケーションをしているのでコンストラクタ関係は
  // プライベートにしている．
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] 入力数を指定する．
  explicit
  TestVector(ymuint ni);

  /// @brief デストラクタ
  ~TestVector();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のソース
  TestVector(const TestVector& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のソース
  const TestVector&
  operator=(const TestVector& src);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  ymuint32 mInputNum;

  // ベクタ本体(ただしサイズは可変)
  tPackedVal mPat[1];


private:

  // 1ワードあたりのHEX文字数
  static
  const ymuint32 HPW = kPvBitLen / 4;

};


/// @brief 内容を出力する．
ostream&
operator<<(ostream& s,
	   const TestVector& tv);

/// @brief 内容を出力する．
ostream&
operator<<(ostream& s,
	   const TestVector* tvp);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 入力数を得る．
inline
ymuint
TestVector::input_num() const
{
  return mInputNum;
}

// @brief pos 番めの値を得る．
// @note 返り値は 0/1 のいずれか
inline
int
TestVector::val(ymuint pos) const
{
  ymuint shift = shift_num(pos);
  ymuint block = block_idx(pos);
  return (mPat[block] >> shift) & 1UL;
}

// @breif pos 番めの値を設定する．
// @note val は 0 か非0 かで区別される．
inline
void
TestVector::set_val(ymuint pos,
		    int val)
{
  ymuint shift = shift_num(pos);
  ymuint block = block_idx(pos);
  tPackedVal mask = 1UL << shift;
  if ( val ) {
    mPat[block] |= mask;
  }
  else {
    mPat[block] &= ~mask;
  }
}

// @brief ブロック数を返す．
inline
ymuint
TestVector::block_num(ymuint ni)
{
  return (ni + kPvBitLen - 1) / kPvBitLen;
}

// @brief HEX文字列の長さを返す．
inline
ymuint
TestVector::hex_length(ymuint ni)
{
  return (ni + 3) / 4;
}

// 入力位置からブロック番号を得る．
inline
ymuint
TestVector::block_idx(ymuint ipos)
{
  return ipos / kPvBitLen;
}

// 入力位置からシフト量を得る．
inline
ymuint
TestVector::shift_num(ymuint ipos)
{
  return (kPvBitLen - 1 - ipos) % kPvBitLen;
}

// @brief 内容を出力する．
inline
ostream&
operator<<(ostream& s,
	   const TestVector& tv)
{
  tv.dump_hex(s);
  return s;
}

// @brief 内容を出力する．
inline
ostream&
operator<<(ostream& s,
	   const TestVector* tvp)
{
  tvp->dump_hex(s);
  return s;
}


END_NAMESPACE_YM_SEAL

#endif // SEAL_TESTVECTOR_H

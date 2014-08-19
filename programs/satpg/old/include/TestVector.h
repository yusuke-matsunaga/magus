#ifndef TESTVECTOR_H
#define TESTVECTOR_H

/// @file TestVectort.h
/// @brief TestVector のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.

#include "satpg_nsdef.h"
#include "Val3.h"
#include "PackedVal.h"
#include "ym_YmUtils/RandGen.h"


BEGIN_NAMESPACE_YM_SATPG

class TvMgr;
class SaFault;

//////////////////////////////////////////////////////////////////////
/// @class TestVector TestVector.h "TestVector.h"
/// @brief テストベクタを表すクラス
//////////////////////////////////////////////////////////////////////
class TestVector
{
  friend class TvMgr;

public:
  //////////////////////////////////////////////////////////////////////
  // 値を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を得る．
  ymuint
  input_num() const;

  /// @brief pos 番めの値を得る．
  /// @param[in] pos 入力の位置番号
  Val3
  val3(ymuint pos) const;

  /// @brief X の個数を得る．
  ymuint
  x_num() const;

  /// @brief 内容を BIN 形式で出力する．
  /// @param[in] s 出力先のストリーム
  void
  dump_bin(ostream& s) const;

  /// @brief 内容を HEX 形式で出力する．
  /// @note X を含む場合の出力は不定
  /// @param[in] s 出力先のストリーム
  void
  dump_hex(ostream& s) const;

  /// @brief 暫定的に用意する関数
  /// @param[in] fp ファイルポインタ
  void
  dump(FILE* fp) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 値を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief すべて未定(X) で初期化する．
  void
  init();

  /// @breif pos 番めの値を設定する．
  /// @param[in] pos 入力の位置番号
  /// @param[in] val 値
  void
  set_val(ymuint pos,
	  Val3 val);

  /// @brief HEX文字列から内容を設定する．
  /// @param[in] hex_string HEX 文字列
  /// @return hex_string に不適切な文字が含まれていたら false を返す．
  /// @note hex_string が短い時には残りは0で初期化される．
  /// @note hex_string が長い時には余りは捨てられる．
  bool
  set_from_hex(const string& hex_string);

  /// @brief 乱数パタンを設定する．
  /// @param[in] randgen 乱数生成器
  /// @note 結果はかならず 0 か 1 になる．(Xは含まれない)
  void
  set_from_random(RandGen& randgen);

  /// @brief テストベクタをコピーする．
  /// @param[in] src コピー元のテストベクタ
  /// @note X の部分はコピーしない．
  void
  copy(const TestVector& src);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ブロック数を返す．
  /// @param[in] ni 入力数
  static
  ymuint
  block_num(ymuint ni);

  /// @brief HEX文字列の長さを返す．
  /// @param[in] ni 入力数
  static
  ymuint
  hex_length(ymuint ni);

  // 入力位置からブロック番号を得る．
  /// @param[in] ipos 入力の位置番号
  static
  ymuint
  block_idx(ymuint ipos);

  // 入力位置からシフト量を得る．
  /// @param[in] ipos 入力の位置番号
  static
  ymuint
  shift_num(ymuint ipos);


private:
  //////////////////////////////////////////////////////////////////////
  // 特殊なアロケーションをしているのでコンストラクタ関係は
  // プライベートにしている．
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] ni 入力数
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


public:

  // 暫定的に用意するメンバ
  int det_count;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  ymuint32 mInputNum;

  // ベクタ本体(ただしサイズは可変)
  PackedVal mPat[1];


private:
  //////////////////////////////////////////////////////////////////////
  // このクラスに固有の定数
  //////////////////////////////////////////////////////////////////////

  // 1ワードあたりのHEX文字数
  static
  const ymuint32 HPW = kPvBitLen / 4;

};


/// @brief 内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] tv テストベクタ
ostream&
operator<<(ostream& s,
	   const TestVector& tv);

/// @brief 内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] tvp テストベクタへのポインタ
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
inline
Val3
TestVector::val3(ymuint pos) const
{
  ymuint shift = shift_num(pos);
  ymuint block0 = block_idx(pos);
  ymuint block1 = block0 + 1;
  int v0 = (mPat[block0] >> shift) & 1UL;
  int v1 = (mPat[block1] >> shift) & 1UL;
  return static_cast<Val3>(v1 + v1 + v0);
}

// @breif pos 番めの値を設定する．
inline
void
TestVector::set_val(ymuint pos,
		    Val3 val)
{
  ymuint shift = shift_num(pos);
  ymuint block0 = block_idx(pos);
  ymuint block1 = block0 + 1;
  PackedVal mask = 1UL << shift;
  if ( val == kVal0 ) {
    mPat[block0] |= mask;
    mPat[block1] &= ~mask;
  }
  else if ( val == kVal1 ) {
    mPat[block0] &= ~mask;
    mPat[block1] |= mask;
  }
  else { // val == kValX
    mPat[block0] &= ~mask;
    mPat[block1] &= ~mask;
  }
}

// @brief ブロック数を返す．
inline
ymuint
TestVector::block_num(ymuint ni)
{
  return ((ni + kPvBitLen - 1) / kPvBitLen) * 2;
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
  return (ipos / kPvBitLen) * 2;
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
  tv.dump_bin(s);
  return s;
}

// @brief 内容を出力する．
inline
ostream&
operator<<(ostream& s,
	   const TestVector* tvp)
{
  tvp->dump_bin(s);
  return s;
}

END_NAMESPACE_YM_SATPG

#endif // TESTVECTOR_H

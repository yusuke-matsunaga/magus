#ifndef LOGIC_AIG_H
#define LOGIC_AIG_H

/// @file logic/Aig.h
/// @brief Aig のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "logic/aig_nsdef.h"
#include "logic/VarId.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class Aig Aig.h "logic/Aig.h"
/// @brief AIG を表すクラス (実際には根の枝を表すクラス)
//////////////////////////////////////////////////////////////////////
class Aig
{
  friend class AigMgrImpl;
  friend class AigNode;

public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタと生成/内容の設定
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  Aig();

  /// @brief デストラクタ
  ~Aig();


public:
  //////////////////////////////////////////////////////////////////////
  // 内部の情報を取得するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 否定したハンドルを返す．
  Aig
  operator~() const;

  /// @brief 反転属性を落としたハンドルを返す．
  Aig
  normalize() const;

  /// @brief ノードの通し番号を得る．
  ymuint
  node_id() const;

  /// @brief 根の極性を得る．
  /// @return 反転しているとき true を返す．
  bool
  inv() const;

  /// @brief 定数0を指しているとき true を返す．
  bool
  is_zero() const;

  /// @brief 定数1を指しているとき true を返す．
  bool
  is_one() const;

  /// @brief 定数を指しているとき true を返す．
  bool
  is_const() const;

  /// @brief 外部入力ノードへのハンドルのとき true を返す．
  bool
  is_input() const;

  /// @brief 外部入力ノードへのハンドルのとき，入力番号を返す．
  /// @note is_input() の時のみ意味を持つ．
  VarId
  input_id() const;

  /// @brief ANDノードへのハンドルのとき true を返す．
  bool
  is_and() const;

  /// @brief pos で指示されたファンインのハンドルを得る．
  /// @note pos は 0 か 1 でなければならない．
  /// @note is_and() の時のみ意味を持つ．
  Aig
  fanin(ymuint pos) const;

  /// @brief fanin0 のハンドルを得る．
  /// @note is_and() の時のみ意味を持つ．
  Aig
  fanin0() const;

  /// @brief fanin1 のハンドルを得る．
  /// @note is_and() の時のみ意味を持つ．
  Aig
  fanin1() const;

  /// @brief ハッシュ値を返す．
  ymuint32
  hash_func() const;


public:
  //////////////////////////////////////////////////////////////////////
  // friend 関数の宣言
  //////////////////////////////////////////////////////////////////////

  friend
  bool
  operator==(Aig src1,
	     Aig src2);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を直接指定したコンストラクタ
  explicit
  Aig(ympuint data);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // すべてのデータをまとめたもの
  ympuint mPackedData;

};


/// @relates Aig
/// @brief 等価比較演算
bool
operator==(Aig src1,
	   Aig src2);

/// @relates Aig
/// @brief 非等価比較演算
bool
operator!=(Aig src1,
	   Aig src2);

/// @relates Aig
/// @brief 内容を出力する関数
ostream&
operator<<(ostream& s,
	   Aig src);


//////////////////////////////////////////////////////////////////////
// Aig のインライン関数
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
Aig::Aig() :
  mPackedData(0UL)
{
}

// @brief 内容を直接指定したコンストラクタ
inline
Aig::Aig(ympuint data) :
  mPackedData(data)
{
}

// @brief デストラクタ
inline
Aig::~Aig()
{
}

// @brief 否定の枝を返す．
inline
Aig
Aig::operator~() const
{
  return Aig(mPackedData ^ 1UL);
}

// @brief 反転属性を落としたハンドルを返す．
inline
Aig
Aig::normalize() const
{
  return Aig(mPackedData & ~1UL);
}

// @brief 極性を得る．
// @return 反転しているとき true を返す．
inline
bool
Aig::inv() const
{
  return static_cast<bool>(mPackedData & 1UL);
}

// @brief 定数0を指しているとき true を返す．
inline
bool
Aig::is_zero() const
{
  return mPackedData == 0UL;
}

// @brief 定数1を指しているとき true を返す．
inline
bool
Aig::is_one() const
{
  return mPackedData == 1UL;
}

// @brief 定数を指しているとき true を返す．
inline
bool
Aig::is_const() const
{
  return (mPackedData & ~1UL) == 0UL;
}

// @brief ハッシュ値を返す．
inline
ymuint32
Aig::hash_func() const
{
  return static_cast<ymuint32>(mPackedData);
}

// @relates Aig
// @brief 等価比較演算
inline
bool
operator==(Aig src1,
	   Aig src2)
{
  return src1.mPackedData == src2.mPackedData;
}

// @relates Aig
// @brief 非等価比較演算
inline
bool
operator!=(Aig src1,
	   Aig src2)
{
  return !operator==(src1, src2);
}

END_NAMESPACE_YM_AIG

BEGIN_NAMESPACE_HASH
// Aigをキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::nsAig::Aig>
{
  ymuint
  operator()(nsYm::nsAig::Aig aig) const
  {
    return aig.hash_func();
  }
};
END_NAMESPACE_HASH

#endif // LOGIC_AIG_H

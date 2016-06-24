#ifndef FRAIGHANDLE_H
#define FRAIGHANDLE_H

/// @file FraigHandle.h
/// @brief FraigHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "equiv.h"
#include "ym/SatVarId.h"
#include "ym/HashFunc.h"
#include "ym/HashSet.h"


BEGIN_NAMESPACE_EQUIV

class FraigNode;

//////////////////////////////////////////////////////////////////////
/// @class FraigHandle FraigHandle.h "ym_cec/FraigHandle.h"
/// @brief Fraig の枝を表すクラス
//////////////////////////////////////////////////////////////////////
class FraigHandle
{
  //friend class FraigMgr;
  friend class FraigNode;
  friend class StructHash;

public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタと生成/内容の設定
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  FraigHandle();

  /// @brief 内容を設定したコンストラクタ
  /// @param[in] node ノード
  /// @param[in] inv 反転している時に true とするフラグ
  FraigHandle(FraigNode* node,
	      bool inv);

  /// @brief デストラクタ
  ~FraigHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．
  /// @param[in] node ノード
  /// @param[in] inv 反転している時に true とするフラグ
  void
  set(FraigNode* node,
      bool inv);


public:
  //////////////////////////////////////////////////////////////////////
  // 内部の情報を取得するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 否定の枝を返す．
  FraigHandle
  operator~() const;

  /// @brief ノードを得る．
  FraigNode*
  node() const;

  /// @brief ノードの変数番号を得る．
  SatVarId
  varid() const;

  /// @brief 対応するリテラルを得る．
  SatLiteral
  literal() const;

  /// @brief 極性を得る．
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
  ///
  /// = is_zero() || is_one()
  bool
  is_const() const;

  /// @brief 外部入力ノードへのハンドルのとき true を返す．
  bool
  is_input() const;

  /// @brief 外部入力ノードへのハンドルのとき，入力番号を返す．
  ///
  /// is_input() == true の時のみ意味を持つ．
  int
  input_id() const;

  /// @brief ANDノードへのハンドルのとき true を返す．
  bool
  is_and() const;

  /// @brief pos で指示されたファンインのハンドルを得る．
  /// @param[in] pos 位置 ( 0 or 1 )
  ///
  /// is_and() == true の時のみ意味を持つ．
  FraigHandle
  fanin_handle(int pos) const;

  /// @brief fanin0 のハンドルを得る．
  ///
  /// is_and() == true の時のみ意味を持つ．
  FraigHandle
  fanin0_handle() const;

  /// @brief fanin1 のハンドルを得る．
  ///
  /// is_and() == true の時のみ意味を持つ．
  FraigHandle
  fanin1_handle() const;

  /// @brief 代表ハンドルを得る．
  FraigHandle
  rep_handle() const;

  /// @brief ハッシュ値を返す．
  SizeType
  hash_func() const;


public:
  //////////////////////////////////////////////////////////////////////
  // friend 関数の宣言
  //////////////////////////////////////////////////////////////////////

  friend
  bool
  operator==(FraigHandle src1,
	     FraigHandle src2);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を直接指定したコンストラクタ
  explicit
  FraigHandle(ympuint data);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // すべてのデータをまとめたもの
  ympuint mPackedData;

};

/// @relates FraigHandle
/// @brief 等価比較演算
/// @param[in] src1, src2 オペランド
bool
operator==(FraigHandle src1,
	   FraigHandle src2);

/// @relates FraigHandle
/// @brief 非等価比較演算
/// @param[in] src1, src2 オペランド
bool
operator!=(FraigHandle src1,
	   FraigHandle src2);

/// @relates FraigHandle
/// @brief 内容をダンプする関数
/// @param[in] s 出力先のストリーム
/// @param[in] src 根の枝
/// @param[in] mark すでに処理したノードの番号を保持するマーク
void
dump_handle(ostream& s,
	    FraigHandle src,
	    HashSet<int>& mark);

/// @relates FraigHandle
/// @brief 内容を出力する関数
/// @param[in] s 出力先のストリーム
/// @param[in] src ハンドル
ostream&
operator<<(ostream& s,
	   FraigHandle src);


//////////////////////////////////////////////////////////////////////
// FraigHandle のインライン関数
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
FraigHandle::FraigHandle() :
  mPackedData(0UL)
{
}

// @brief 内容を直接指定したコンストラクタ
inline
FraigHandle::FraigHandle(ympuint data) :
  mPackedData(data)
{
}

// @brief 内容を設定したコンストラクタ
inline
FraigHandle::FraigHandle(FraigNode* node,
			 bool inv)
{
  mPackedData = reinterpret_cast<ympuint>(node) | inv;
}

// @brief デストラクタ
inline
FraigHandle::~FraigHandle()
{
}

// @brief 内容を設定する．
inline
void
FraigHandle::set(FraigNode* node,
		 bool inv)
{
  mPackedData = reinterpret_cast<ympuint>(node) | inv;
}

// @brief 否定の枝を返す．
inline
FraigHandle
FraigHandle::operator~() const
{
  return FraigHandle(mPackedData ^ 1UL);
}

// @brief ノードを得る．
inline
FraigNode*
FraigHandle::node() const
{
  return reinterpret_cast<FraigNode*>(mPackedData & ~3UL);
}

// @brief 極性を得る．
// @return 反転しているとき true を返す．
inline
bool
FraigHandle::inv() const
{
  return static_cast<bool>(mPackedData & 1UL);
}

// @brief 定数0を指しているとき true を返す．
inline
bool
FraigHandle::is_zero() const
{
  return mPackedData == 0UL;
}

// @brief 定数1を指しているとき true を返す．
inline
bool
FraigHandle::is_one() const
{
  return mPackedData == 1UL;
}

// @brief 定数を指しているとき true を返す．
inline
bool
FraigHandle::is_const() const
{
  return (mPackedData & ~1UL) == 0UL;
}

// @brief ハッシュ値を返す．
inline
SizeType
FraigHandle::hash_func() const
{
  return static_cast<SizeType>(mPackedData);
}

// @relates FraigHandle
// @brief 等価比較演算
inline
bool
operator==(FraigHandle src1,
	   FraigHandle src2)
{
  return src1.mPackedData == src2.mPackedData;
}

// @relates FraigHandle
// @brief 非等価比較演算
inline
bool
operator!=(FraigHandle src1,
	   FraigHandle src2)
{
  return !operator==(src1, src2);
}

END_NAMESPACE_EQUIV

BEGIN_NAMESPACE_YM

// FraigHandleをキーにしたハッシュ関数クラスの定義
template <>
struct HashFunc<MAGUS_NSNAME::nsEquiv::FraigHandle>
{
  SizeType
  operator()(MAGUS_NSNAME::nsEquiv::FraigHandle handle) const
  {
    return handle.hash_func();
  }
};

END_NAMESPACE_YM

#endif // FRAIGHANDLE_H

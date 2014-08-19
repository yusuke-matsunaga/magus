#ifndef LIBYM_CEC_CECHANDLE_H
#define LIBYM_CEC_CECHANDLE_H

/// @file libym_cec/CecHandle.h
/// @brief CecHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "cec_nsdef.h"
#include "YmLogic/VarId.h"


BEGIN_NAMESPACE_YM_CEC

class CecNode;

//////////////////////////////////////////////////////////////////////
/// @class CecHandle CecHandle.h "CecHandle.h"
/// @brief 枝を表すクラス
//////////////////////////////////////////////////////////////////////
class CecHandle
{
  friend class CecMgr;
  friend class CecNode;

public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタと生成/内容の設定
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  CecHandle();

  /// @brief 内容を設定したコンストラクタ
  /// @param[in] node ノード
  /// @param[in] inv 反転している時に true とするフラグ
  CecHandle(CecNode* node,
	    bool inv);

  /// @brief デストラクタ
  ~CecHandle();

  /// @brief 内容を設定する．
  /// @param[in] node ノード
  /// @param[in] inv 反転している時に true とするフラグ
  void
  set(CecNode* node,
      bool inv);


public:
  //////////////////////////////////////////////////////////////////////
  // 内部の情報を取得するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 否定の枝を返す．
  CecHandle
  operator~() const;

  /// @brief ノードを得る．
  CecNode*
  node() const;

  /// @brief ノードの通し番号を得る．
  VarId
  varid() const;

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
  bool
  is_const() const;

  /// @brief 外部入力ノードへのハンドルのとき true を返す．
  bool
  is_input() const;

  /// @brief 外部入力ノードへのハンドルのとき，入力番号を返す．
  /// @note is_input() の時のみ意味を持つ．
  ymuint
  input_id() const;

  /// @brief ANDノードへのハンドルのとき true を返す．
  bool
  is_and() const;

  /// @brief pos で指示されたファンインのハンドルを得る．
  /// @note pos は 0 か 1 でなければならない．
  /// @note is_and() の時のみ意味を持つ．
  CecHandle
  fanin_handle(ymuint pos) const;

  /// @brief fanin0 のハンドルを得る．
  /// @note is_and() の時のみ意味を持つ．
  CecHandle
  fanin0_handle() const;

  /// @brief fanin1 のハンドルを得る．
  /// @note is_and() の時のみ意味を持つ．
  CecHandle
  fanin1_handle() const;

  /// @brief 代表ハンドルを得る．
  CecHandle
  rep_handle() const;

  /// @brief ハッシュ値を返す．
  ymuint32
  hash_func() const;


public:
  //////////////////////////////////////////////////////////////////////
  // friend 関数の宣言
  //////////////////////////////////////////////////////////////////////

  friend
  bool
  operator==(CecHandle src1,
	     CecHandle src2);


private:

  /// @brief 内容を直接指定したコンストラクタ
  explicit
  CecHandle(ympuint data);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // すべてのデータをまとめたもの
  ympuint mPackedData;

};

/// @relates CecHandle
/// @brief 等価比較演算
bool
operator==(CecHandle src1,
	   CecHandle src2);

/// @relates CecHandle
/// @brief 非等価比較演算
bool
operator!=(CecHandle src1,
	   CecHandle src2);

/// @relates CecHandle
/// @brief 内容をダンプする関数
/// @param[in] src 根の枝
/// @param[in] mark すでに処理したノードの番号を保持するマーク
/// @param[in] s 出力先のストリーム
void
dump_handle(CecHandle src,
	    unordered_set<ymuint>& mark,
	    ostream& s);

/// @relates CecHandle
/// @brief 内容を出力する関数
ostream&
operator<<(ostream& s,
	   CecHandle src);


//////////////////////////////////////////////////////////////////////
// CecHandle のインライン関数
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
CecHandle::CecHandle() :
  mPackedData(0UL)
{
}

// @brief 内容を直接指定したコンストラクタ
inline
CecHandle::CecHandle(ympuint data) :
  mPackedData(data)
{
}

// @brief 内容を設定したコンストラクタ
inline
CecHandle::CecHandle(CecNode* node,
		     bool inv)
{
  mPackedData = reinterpret_cast<ympuint>(node) | inv;
}

// @brief デストラクタ
inline
CecHandle::~CecHandle()
{
}

// @brief 内容を設定する．
inline
void
CecHandle::set(CecNode* node,
	       bool inv)
{
  mPackedData = reinterpret_cast<ympuint>(node) | inv;
}

// @brief 否定の枝を返す．
inline
CecHandle
CecHandle::operator~() const
{
  return CecHandle(mPackedData ^ 1UL);
}

// @brief ノードを得る．
inline
CecNode*
CecHandle::node() const
{
  return reinterpret_cast<CecNode*>(mPackedData & ~3UL);
}

// @brief 極性を得る．
// @return 反転しているとき true を返す．
inline
bool
CecHandle::inv() const
{
  return static_cast<bool>(mPackedData & 1UL);
}

// @brief 定数0を指しているとき true を返す．
inline
bool
CecHandle::is_zero() const
{
  return mPackedData == 0UL;
}

// @brief 定数1を指しているとき true を返す．
inline
bool
CecHandle::is_one() const
{
  return mPackedData == 1UL;
}

// @brief 定数を指しているとき true を返す．
inline
bool
CecHandle::is_const() const
{
  return (mPackedData & ~1UL) == 0UL;
}

// @brief ハッシュ値を返す．
inline
ymuint32
CecHandle::hash_func() const
{
  return static_cast<ymuint32>(mPackedData);
}

// @relates CecHandle
// @brief 等価比較演算
inline
bool
operator==(CecHandle src1,
	   CecHandle src2)
{
  return src1.mPackedData == src2.mPackedData;
}

// @relates CecHandle
// @brief 非等価比較演算
inline
bool
operator!=(CecHandle src1,
	   CecHandle src2)
{
  return !operator==(src1, src2);
}

END_NAMESPACE_YM_CEC

BEGIN_NAMESPACE_HASH
// CecHandleをキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::nsCec::CecHandle>
{
  ymuint
  operator()(nsYm::nsCec::CecHandle aig) const
  {
    return aig.hash_func();
  }
};
END_NAMESPACE_HASH

#endif // LIBYM_CEC_CECHANDLE_H

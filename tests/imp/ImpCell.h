#ifndef IMPCELL_H
#define IMPCELL_H

/// @file ImpCell.h
/// @brief ImpCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"
#include "ImpVal.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class ImpCell ImpCell.h "ImpCell.h"
/// @brief 一つの含意情報を表すクラス
//////////////////////////////////////////////////////////////////////
class ImpCell
{
  friend class ImpInfo;
  friend class ImpList;
  friend class ImpListIterator;
public:

  /// @brief 空のコンストラクタ
  ImpCell();

  /// @brief コンストラクタ
  /// @param[in] src_id 含意元のノード番号
  /// @param[in] src_val 含意元の値
  /// @param[in] dst_id 含意先のノード番号
  /// @param[in] dst_val 含意先の値
  ImpCell(ymuint src_id,
	  ymuint src_val,
	  ymuint dst_id,
	  ymuint dst_val);

  /// @brief デストラクタ
  ~ImpCell();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 含意元のノード番号を取り出す．
  ymuint
  src_id() const;

  /// @brief 含意元の値 ( 0 or 1) を返す．
  ymuint
  src_val() const;

  /// @brief 含意先のノード番号を取り出す．
  ymuint
  dst_id() const;

  /// @brief 含意先の値 ( 0 or 1 ) を返す．
  ymuint
  dst_val() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 値を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を設定する．
  /// @param[in] src_id 含意元のノード番号
  /// @param[in] src_val 含意元の値
  /// @param[in] dst_id 含意先のノード番号
  /// @param[in] dst_val 含意先の値
  void
  set(ymuint src_id,
      ymuint src_val,
      ymuint dst_id,
      ymuint dst_val);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 含意元の情報
  ImpVal mSrcVal;

  // 含意先の情報
  ImpVal mDstVal;

  // リスト中の前の要素
  ImpCell* mPrev;

  // リスト中の後の要素
  ImpCell* mNext;

  // ハッシュ用のリンク
  ImpCell* mLink;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
ImpCell::ImpCell()
{
  mPrev = this;
  mNext = this;
  mLink = NULL;
}

// @brief コンストラクタ
// @param[in] src_id 含意元のノード番号
// @param[in] src_val 含意元の値
// @param[in] dst_id 含意先のノード番号
// @param[in] dst_val 含意先の値
inline
ImpCell::ImpCell(ymuint src_id,
		 ymuint src_val,
		 ymuint dst_id,
		 ymuint dst_val) :
  mSrcVal(src_id, src_val),
  mDstVal(dst_id, dst_val)
{
  mPrev = this;
  mNext = this;
  mLink = NULL;
}

// @brief 値を設定する．
// @param[in] src_id 含意元のノード番号
// @param[in] src_val 含意元の値
// @param[in] dst_id 含意先のノード番号
// @param[in] dst_val 含意先の値
inline
void
ImpCell::set(ymuint src_id,
	     ymuint src_val,
	     ymuint dst_id,
	     ymuint dst_val)
{
  mSrcVal.set(src_id, src_val);
  mDstVal.set(dst_id, dst_val);
}

// @brief デストラクタ
inline
ImpCell::~ImpCell()
{
}

// @brief 含意元のノード番号を取り出す．
inline
ymuint
ImpCell::src_id() const
{
  return mSrcVal.id();
}

// @brief 含意元の値 ( 0 or 1) を返す．
inline
ymuint
ImpCell::src_val() const
{
  return mSrcVal.val();
}

// @brief 含意先のノード番号を取り出す．
inline
ymuint
ImpCell::dst_id() const
{
  return mDstVal.id();
}

// @brief 含意先の値 ( 0 or 1 )
inline
ymuint
ImpCell::dst_val() const
{
  return mDstVal.val();
}

END_NAMESPACE_YM_NETWORKS

#endif // IMPCELL_H

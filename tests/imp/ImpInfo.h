#ifndef IMPINFO_H
#define IMPINFO_H

/// @file ImpInfo.h
/// @brief ImpInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class ImpCell ImpInfo.h "ImpInfo.h"
/// @brief 一つの含意情報を表すクラス
//////////////////////////////////////////////////////////////////////
class ImpCell
{
public:

  /// @brief コンストラクタ
  /// @param[in] dst_id 含意先のノード番号
  /// @param[in] dst_val 含意先の値
  ImpCell(ymuint dst_id,
	  ymuint dst_val);

  /// @brief デストラクタ
  ~ImpCell();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 含意先のノード番号を取り出す．
  ymuint
  dst_id() const;

  /// @brief 含意先の値 ( 0 or 1 )
  ymuint
  dst_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 含意先のノード番号 + 値
  ymuint32 mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class ImpInfo ImpInfo.h "ImpInfo.h"
/// @brief 含意情報を表すクラス
//////////////////////////////////////////////////////////////////////
class ImpInfo
{
public:

  /// @brief 空のコンストラクタ
  ImpInfo();

  /// @brief デストラクタ
  ~ImpInfo();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 含意情報のリストを取り出す．
  /// @param[in] src_id 含意元のノード番号
  /// @param[in] src_val 含意元の値 ( 0 or 1 )
  const list<ImpCell>&
  get(ymuint src_id,
      ymuint src_val) const;

  /// @brief 含意情報のリストを取り出す．
  /// @param[in] src_id 含意元のノード番号
  /// @param[in] src_val 含意元の値 ( 0 or 1 )
  list<ImpCell>&
  get(ymuint src_id,
      ymuint src_val);

  /// @brief 該当する含意情報が含まれているか調べる．
  bool
  check(ymuint src_id,
	ymuint src_val,
	ymuint dst_id,
	ymuint dst_val) const;

  /// @brief 含意の総数を得る．
  ymuint
  size() const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  void
  print(ostream& s) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 値を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief サイズを設定する．
  /// @param[in] max_id ID番号の最大値
  void
  set_size(ymuint max_id);

  /// @brief 含意情報を追加する．
  /// @param[in] src_id 含意元のノード番号
  /// @param[in] src_val 含意元の値 ( 0 or 1 )
  /// @param[in] dst_id 含意先のノード番号
  /// @param[in] dst_val 含意先の値 ( 0 or 1 )
  void
  put(ymuint src_id,
      ymuint src_val,
      ymuint dst_id,
      ymuint dst_val);

  /// @brief 含意情報を追加する．
  /// @param[in] src_id 含意元のノード番号
  /// @param[in] src_val 含意元の値 ( 0 or 1 )
  /// @param[in] imp_list 含意リスト
  void
  put(ymuint src_id,
      ymuint src_val,
      const vector<ImpCell>& imp_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 含意の総数
  ymuint32 mSize;

  // 値を格納する配列
  vector<list<ImpCell> > mArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] dst_id 含意先のノード番号
// @param[in] dst_val 含意先の値
inline
ImpCell::ImpCell(ymuint dst_id,
		 ymuint dst_val)
{
  mVal = (dst_id * 2) + dst_val;
}

// @brief デストラクタ
inline
ImpCell::~ImpCell()
{
}

// @brief 含意先のノード番号を取り出す．
inline
ymuint
ImpCell::dst_id() const
{
  return mVal >> 1;
}

// @brief 含意先の値 ( 0 or 1 )
inline
ymuint
ImpCell::dst_val() const
{
  return mVal & 1U;
}

END_NAMESPACE_YM_NETWORKS

#endif // IMPINFO_H

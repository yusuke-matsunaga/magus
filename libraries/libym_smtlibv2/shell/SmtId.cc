
/// @file SmtId.cc
/// @brief SmtId の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtId.h"
#include "SmtIndexId.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtId
//////////////////////////////////////////////////////////////////////

// @brief 単純な形のコンストラクタ
// @param[in] name 名前
SmtId::SmtId(const ShString& name) :
  mName(name)
{
  mLink = NULL;
}

// @brief デストラクタ
SmtId::~SmtId()
{
}

// @brief インデックスリストの要素数を返す．
// @note インデックスリストを持たない場合は 0 を返す．
ymuint
SmtId::index_size() const
{
  return 0;
}

// @brief インデックスを返す．
// @param[in] pos 位置番号 ( 0 <= pos < index_size() )
// @note インデックスを持たない場合や pos が index_size() より大きい場合はエラー(アボート)となる．
ymint32
SmtId::index(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtIndexId
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
SmtIndexId::SmtIndexId(const ShString& name) :
  SmtId(name)
{
  // mIndexList の確保は SmtIdMgr が行う．
}

// @brief デストラクタ
SmtIndexId::~SmtIndexId()
{
  // mIndexList の開放は SmtIdMgr が行う．
}

// @brief インデックスリストの要素数を返す．
// @note インデックスリストを持たない場合は 0 を返す．
ymuint
SmtIndexId::index_size() const
{
  return mIndexNum;
}

// @brief インデックスを返す．
// @param[in] pos 位置番号 ( 0 <= pos < index_size() )
// @note インデックスを持たない場合や pos が index_size() より大きい場合はエラー(アボート)となる．
ymint32
SmtIndexId::index(ymuint pos) const
{
  assert_cond( pos < index_size(), __FILE__, __LINE__);
  return mIndexList[pos];
}

END_NAMESPACE_YM_SMTLIBV2

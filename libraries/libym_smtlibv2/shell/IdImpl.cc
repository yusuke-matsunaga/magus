
/// @file SmtId.cc
/// @brief SmtId の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "IdImpl.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス IdImpl
//////////////////////////////////////////////////////////////////////

// @brief 単純な形のコンストラクタ
// @param[in] name 名前
IdImpl::IdImpl(const ShString& name) :
  mName(name)
{
  mLink = NULL;
}

// @brief デストラクタ
IdImpl::~IdImpl()
{
}

// @brief ID番号を返す．
ymuint
IdImpl::id() const
{
  return mId;
}

// @brief 名前を返す．
ShString
IdImpl::name() const
{
  return mName;
}

// @brief インデックスリストの要素数を返す．
// @note インデックスリストを持たない場合は 0 を返す．
ymuint
IdImpl::index_size() const
{
  return 0;
}

// @brief インデックスを返す．
// @param[in] pos 位置番号 ( 0 <= pos < index_size() )
// @note インデックスを持たない場合や pos が index_size() より大きい場合はエラー(アボート)となる．
ymuint32
IdImpl::index(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtIndexId
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] index_num インデックスの数
SmtIndexId::SmtIndexId(const ShString& name,
		       ymuint index_num) :
  IdImpl(name),
  mIndexNum(index_num)
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
ymuint32
SmtIndexId::index(ymuint pos) const
{
  assert_cond( pos < index_size(), __FILE__, __LINE__);
  return mIndexList[pos];
}

END_NAMESPACE_YM_SMTLIBV2

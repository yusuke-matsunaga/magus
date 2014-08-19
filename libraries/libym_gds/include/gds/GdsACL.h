#ifndef GDS_GDSACL_H
#define GDS_GDSACL_H

/// @file gds/GdsACL.h
/// @brief GdsACL のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "gds/gds_nsdef.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsACL GdsACL.h "gds/GdsACL.h"
/// @brief access control list を表すクラス
//////////////////////////////////////////////////////////////////////
class GdsACL
{
  friend class GdsParser;

private:

  /// @brief コンストラクタ
  /// @param[in] group グループ番号
  /// @param[in] user ユーザー番号
  /// @param[in] access アクセスコード
  GdsACL(ymuint group,
	 ymuint user,
	 ymuint access);

  /// @brief デストラクタ
  ~GdsACL();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数
  /// @brief グループIDを得る．
  ymuint
  group() const;

  /// @brief ユーザーIDを得る．
  ymuint
  user() const;

  /// @brief アクセスコードを得る．
  ymuint
  access() const;

  /// @brief 次の要素を得る．
  const GdsACL*
  next() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // グループID
  ymuint16 mGroup;

  // ユーザーID
  ymuint16 mUser;

  // アクセスコード
  ymuint16 mAccess;

  // 次の要素
  GdsACL* mNext;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] group グループ番号
// @param[in] user ユーザー番号
// @param[in] access アクセスコード
inline
GdsACL::GdsACL(ymuint group,
	       ymuint user,
	       ymuint access) :
  mGroup(group),
  mUser(user),
  mAccess(access),
  mNext(NULL)
{
}

// @brief デストラクタ
inline
GdsACL::~GdsACL()
{
}

// @brief 要素数
// @brief グループIDを得る．
inline
ymuint
GdsACL::group() const
{
  return mGroup;
}

// @brief ユーザーIDを得る．
inline
ymuint
GdsACL::user() const
{
  return mUser;
}

// @brief アクセスコードを得る．
inline
ymuint
GdsACL::access() const
{
  return mAccess;
}

// @brief 次の要素を得る．
inline
const GdsACL*
GdsACL::next() const
{
  return mNext;
}

END_NAMESPACE_YM_GDS

#endif // GDS_GDSACL_H

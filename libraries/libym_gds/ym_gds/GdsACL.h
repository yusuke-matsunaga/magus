#ifndef YM_GDS_GDSACL_H
#define YM_GDS_GDSACL_H

/// @file ym_gds/GdsACL.h
/// @brief GdsACL のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/gds_nsdef.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsACL GdsACL.h "ym_gds/GdsACL.h"
/// @brief access control list を表すクラス
//////////////////////////////////////////////////////////////////////
class GdsACL
{
public:

  /// @brief


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief グループIDを得る．
  ymuint
  group() const;

  /// @brief ユーザーIDを得る．
  ymuint
  user() const;

  /// @brief アクセスコードを得る．
  ymuint
  access() const;


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

};

END_NAMESPACE_YM_GDS

#endif // YM_GDS_GDSACL_H

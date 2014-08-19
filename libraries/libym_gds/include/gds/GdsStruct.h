#ifndef GDS_GDSSTRUCT_H
#define GDS_GDSSTRUCT_H

/// @file gds/GdsStruct.h
/// @brief GdsStruct のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "gds/gds_nsdef.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsStruct GdsStruct.h "gds/GdsStruct.h"
/// @brief BGNSTR - ENDSTR の構造を表すクラス
//////////////////////////////////////////////////////////////////////
class GdsStruct
{
  friend class GdsParser;

private:

  /// @brief コンストラクタ
  /// @param[in] date 2つの日時の配列
  /// @param[in] name 名前
  GdsStruct(GdsDate* date,
	    GdsString* name);

  /// @brief デストラクタ
  ~GdsStruct();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief生成日時を返す．
  const GdsDate&
  creation_time() const;

  /// @brief 最終更新日時を返す．
  const GdsDate&
  last_modification_time() const;

  /// @brief名前を返す．
  const char*
  name() const;

  /// @brief 要素を返す．
  const GdsElement*
  element() const;

  /// @brief 次の要素を返す．
  const GdsStruct*
  next() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 生成日時
  GdsDate* mCreationTime;

  // 最終更新日時
  GdsDate* mLastModificationTime;

  // 名前
  GdsString* mName;

  // 要素
  GdsElement* mElement;

  // 次の要素
  GdsStruct* mLink;

};

END_NAMESPACE_YM_GDS

#endif // GDS_GDSSTRUCT_H

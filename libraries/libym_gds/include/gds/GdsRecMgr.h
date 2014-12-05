#ifndef GDS_GDSRECMGR_H
#define GDS_GDSRECMGR_H

/// @file gds/GdsRecMgr.h
/// @brief GdsRecMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "gds/gds_nsdef.h"
#include "YmUtils/FragAlloc.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsRecMgr GdsRecMgr.h "gds/GdsRecMgr.h"
/// @brief GdsRecord の生成/破壊を管理するクラス
//////////////////////////////////////////////////////////////////////
class GdsRecMgr
{
public:

  /// @brief コンストラクタ
  GdsRecMgr();

  /// @brief デストラクタ
  ~GdsRecMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief レコードの生成
  /// @param[in] scanner 字句解析器
  GdsRecord*
  new_record(const GdsScanner& scanner);

  /// @brief レコードの破壊
  /// @param[in] rec 破壊するレコード
  void
  free_record(GdsRecord* rec);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // レコード用のアロケータ
  FragAlloc mAlloc;

};

END_NAMESPACE_YM_GDS

#endif

#ifndef YM_NETWORKS_BDNBLIFWRITER_H
#define YM_NETWORKS_BDNBLIFWRITER_H

/// @file ym_networks/BdnBlifWriter.h
/// @brief BdnBlifWriter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BdnBlifWriter.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include <ym_networks/bdn.h>


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class BdnBlifWriter BdnBlifWriter.h <ym_networks/BdnBlifWriter.h>
/// @ingroup BnetGroup
/// @brief BdnMgr の内容を blif 形式で出力するクラス
/// @sa Bdnwork
//////////////////////////////////////////////////////////////////////
class BdnBlifWriter
{
public:

  /// @brief コンストラクタ
  BdnBlifWriter();

  /// @brief デストラクタ
  ~BdnBlifWriter();


public:

  /// @brief 出力する．
  void
  operator()(ostream& s,
	     const BdnMgr& network);

};

END_NAMESPACE_YM_NETWORKS

#endif // YM_NETWORKS_BDNBLIFWRITER_H

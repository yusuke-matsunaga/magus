#ifndef NETWORKS_CMNBLIFWRITER_H
#define NETWORKS_CMNBLIFWRITER_H

/// @file YmNetworks/CmnBlifWriter.h
/// @brief CmnBlifWriter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/cmn.h"


BEGIN_NAMESPACE_YM_NETWORKS_CMN

class BlifWriterImpl;

//////////////////////////////////////////////////////////////////////
/// @class CmnBlifWriter CmnBlifWriter.h "YmNetworks/CmnBlifWriter.h"
/// @brief CmnMgr の内容を net-blif 形式で出力するクラス
//////////////////////////////////////////////////////////////////////
class CmnBlifWriter
{
public:

  /// @brief コンストラクタ
  CmnBlifWriter();

  /// @brief デストラクタ
  ~CmnBlifWriter();


public:

  /// @brief relates CmnMgr
  /// @brief 内容を net-blif 形式で出力するクラス
  void
  operator()(ostream& s,
	     const CmnMgr& mgr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の処理を行うクラス
  BlifWriterImpl* mImpl;

};

END_NAMESPACE_YM_NETWORKS_CMN

#endif // NETWORKS_CMNBLIFWRITER_H

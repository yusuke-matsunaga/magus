#ifndef YM_BDN_BDNVERILOGWRITER_H
#define YM_BDN_BDNVERILOGWRITER_H

/// @file ym_bdn/BdnVerilogWriter.h
/// @brief BdnVerilogWriter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BdnVerilogWriter.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bdn/bdn_nsdef.h"


BEGIN_NAMESPACE_YM_BDN

//////////////////////////////////////////////////////////////////////
/// @class BdnVerilogWriter BdnVerilogWriter.h <ym_bdn/BdnVerilogWriter.h>
/// @ingroup BnetGroup
/// @brief BdnMgr の内容を blif 形式で出力するクラス
/// @sa Bdnwork
//////////////////////////////////////////////////////////////////////
class BdnVerilogWriter
{
public:

  /// @brief コンストラクタ
  BdnVerilogWriter();

  /// @brief デストラクタ
  ~BdnVerilogWriter();


public:

  /// @brief 出力する．
  void
  operator()(ostream& s,
	     const BdnMgr& network);

};

END_NAMESPACE_YM_BDN

#endif // YM_BDN_BDNVERILOGWRITER_H

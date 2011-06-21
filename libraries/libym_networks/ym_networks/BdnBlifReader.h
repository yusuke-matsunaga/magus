#ifndef YM_NETWORKS_BDNBLIFREADER_H
#define YM_NETWORKS_BDNBLIFREADER_H

/// @file ym_networks/BdnBlifReader.h
/// @brief BdnBlifReader のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BlifBdnConv.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class BdnBlifReader BdnBlifReader.h "ym_networks/BdnBlifReader.h"
/// @ingroup BdnGroup
/// @brief blif 形式のファイルを読み込んで BDN に設定するクラス
/// @sa BdnMgr nsYm::nsBlif::BlifParser
//////////////////////////////////////////////////////////////////////
class BdnBlifReader
{
public:

  /// @brief コンストラクタ
  BdnBlifReader();

  /// @brief デストラクタ
  ~BdnBlifReader();


public:

  /// @brief blif 形式のファイルを読み込む
  /// @param[in] filename ファイル名
  /// @param[in] network 読み込んだ内容を設定するネットワーク
  /// @retval true 正常に読み込めた
  /// @retval false 読み込み中にエラーが起こった．
  bool
  read(const string& filename,
       BdnMgr& network);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_YM_NETWORKS

#endif // YM_NETWORKS_BDNBLIFREADER_H

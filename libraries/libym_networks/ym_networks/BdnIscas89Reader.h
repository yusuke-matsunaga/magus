#ifndef YM_NETWORKS_BDNISCAS89READER_H
#define YM_NETWORKS_BDNISCAS89READER_H

/// @file ym_networks/BdnIscas89Reader.h
/// @brief BdnIscas89Reader のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS_BDN

//////////////////////////////////////////////////////////////////////
/// @class BdnIscas89Reader BdnIscas89Reader.h "ym_networks/BdnIscas89Reader.h"
/// @brief iscas89 形式のファイルを読み込んで BDN に設定するクラス
/// @sa BdnMgr
//////////////////////////////////////////////////////////////////////
class BdnIscas89Reader
{
public:

  /// @brief コンストラクタ
  BdnIscas89Reader();

  /// @brief デストラクタ
  ~BdnIscas89Reader();


public:

  /// @brief iscas89 形式のファイルを読み込む
  /// @param[in] filename ファイル名
  /// @param[in] network 読み込んだ内容を設定するネットワーク
  /// @retval true 正常に読み込めた．
  /// @retval false 読み込み中にエラーが起こった．
  bool
  read(const string& filename,
       BdnMgr& network);

};

END_NAMESPACE_YM_NETWORKS_BDN

#endif // YM_NETWORKS_BDNISCAS89READER_H

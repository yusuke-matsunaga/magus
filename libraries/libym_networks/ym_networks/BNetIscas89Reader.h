#ifndef YM_NETWORKS_BNETISCAS89READER_H
#define YM_NETWORKS_BNETISCAS89READER_H

/// @file ym_networks/BNetIscas89Reader.h
/// @brief BNetIscas89Reader のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bnet.h"


BEGIN_NAMESPACE_YM_NETWORKS

namespace nsIscas89 {
  class Iscas89Parser;
  class BNetIscas89Handler;
}

END_NAMESPACE_YM_NETWORKS

BEGIN_NAMESPACE_YM_NETWORKS_BNET

//////////////////////////////////////////////////////////////////////
/// @class BNetIscas89Reader BNetIscas89Reader.h "ym_networks/BNetIscas89Reader.h"
/// @ingroup BnetGroup
/// @brief iscas89 形式のファイルを読み込んで BNetwork に設定するクラス
/// @sa BNetwork
//////////////////////////////////////////////////////////////////////
class BNetIscas89Reader
{
public:

  /// @brief コンストラクタ
  BNetIscas89Reader();

  /// @brief デストラクタ
  ~BNetIscas89Reader();


public:

  /// @brief ISCAS89 形式のファイルを読み込む
  /// @param[in] filename ファイル名
  /// @param[in] network 読み込んだ内容を設定するネットワーク
  /// @retval true 正常に読み込めた
  /// @retval false 読み込み中にエラーが起こった．
  bool
  read(const string& filename,
       BNetwork& network);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // iscas89 パーサー
  nsIscas89::Iscas89Parser* mParser;

  // ハンドラ
  nsIscas89::BNetIscas89Handler* mHandler;

};

END_NAMESPACE_YM_NETWORKS_BNET

#endif // YM_NETWORKS_BNETISCAS89READER_H

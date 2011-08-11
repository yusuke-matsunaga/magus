#ifndef BLIFNETWORKREADER_H
#define BLIFNETWORKREADER_H

/// @file BlifNetwork.h
/// @brief BlifNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "blif_nsdef.h"
#include "BlifParser.h"


BEGIN_NAMESPACE_YM_BLIF

class BlifNetworkHandler;

//////////////////////////////////////////////////////////////////////
/// @class BlifNetworkReader BlifNetwork.h "BlifNetwork.h"
/// @ingroup BlifGroup
/// @brief blif 形式のファイルを読み込んで BlifNetwork に設定するクラス
/// @sa BlifNetwork
//////////////////////////////////////////////////////////////////////
class BlifNetworkReader
{
public:

  /// @brief コンストラクタ
  BlifNetworkReader();

  /// @brief デストラクタ
  ~BlifNetworkReader();


public:

  /// @brief BLIF 形式のファイルを読み込む
  /// @param[in] filename ファイル名
  /// @param[out] network 読み込んだ内容を設定するネットワーク
  /// @retval true 正常に読み込めた
  /// @retval false 読み込み中にエラーが起こった．
  bool
  read(const string& filename,
       BlifNetwork& network);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // blif パーサー
  BlifParser mParser;

  // ハンドラ
  BlifNetworkHandler* mHandler;

};

END_NAMESPACE_YM_BLIF

#endif // BLIFNETWORKREADER_H

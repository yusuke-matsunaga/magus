
/// @file BdnIscas89Reader.sip
/// @brief BdnIscas89Reader の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class BdnIscas89Reader
{
%TypeHeaderCode
#include "networks/BdnIscas89Reader.h"
using nsYm::BdnIscas89Reader;
%End

public:

  /// @brief コンストラクタ
  BdnIscas89Reader();

  /// @brief iscas89 形式のファイルを読み込む
  /// @param[in] filename ファイル名
  /// @param[in] network 読み込んだ内容を設定するネットワーク
  /// @retval true 正常に読み込めた．
  /// @retval false 読み込み中にエラーが起こった．
  bool
  operator()(const char* filename,
	     BdnMgr& network);

};

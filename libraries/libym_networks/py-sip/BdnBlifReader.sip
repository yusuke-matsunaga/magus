
/// @file BdnBlifReader.sip
/// @brief BdnBlifReader の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class BdnBlifReader
{
%TypeHeaderCode
#include "networks/BdnBlifReader.h"
using nsYm::BdnBlifReader;
%End

public:

  /// @brief コンストラクタ
  BdnBlifReader();

  /// @brief blif 形式のファイルを読み込む
  /// @param[in] filename ファイル名
  /// @param[in] network 読み込んだ内容を設定するネットワーク
  /// @param[in] cell_library セルライブラリ
  /// @retval true 正常に読み込めた
  /// @retval false 読み込み中にエラーが起こった．
  bool
  __call__(const char* filename,
	   BdnMgr& network,
	   const CellLibrary* cell_library = NULL);
  %TypeHeaderCode
  sipRes = sipCpp->operator()(a0, a1, a2);
  %End

};

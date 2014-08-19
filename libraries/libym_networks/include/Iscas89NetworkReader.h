#ifndef ISCAS89NETWORKREADER_H
#define ISCAS89NETWORKREADER_H

/// @file Iscas89NetworkReader.h
/// @brief Iscas89NetworkReader のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "iscas89_nsdef.h"
#include "Iscas89Parser.h"


BEGIN_NAMESPACE_YM_ISCAS89

class Iscas89Network;
class Iscas89NetworkHandler;

//////////////////////////////////////////////////////////////////////
/// @class Iscas89NetworkReader Iscas89NetworkReader.h
/// @brief iscas89 のファイルを読み込んで Iscas89Network に設定するクラス
/// @sa Iscas89Network
//////////////////////////////////////////////////////////////////////
class Iscas89NetworkReader
{
public:

  /// @brief コンストラクタ
  Iscas89NetworkReader();

  /// @brief デストラクタ
  ~Iscas89NetworkReader();


public:

  /// @brief ISCAS89 形式のファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @param[out] network 読み込んだ内容を設定するネットワーク
  /// @retval true 正常に読み込めた．
  /// @retval false 読み込み中にエラーが起こった．
  bool
  read(const string& filename,
       Iscas89Network& network);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // iscas89 パーサー
  Iscas89Parser mParser;

  // ハンドラ
  Iscas89NetworkHandler* mHandler;

};

END_NAMESPACE_YM_ISCAS89

#endif // ISCAS89NETWORKREADER_H

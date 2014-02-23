
/// @file GenBase.sip
/// @brief GenBase の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.

class GenBase {

%TypeHeaderCode
#include "utils/GenBase.h"
using nsYm::GenBase;
%End

public:

  /// @brief コンストラクタ
  /// @param[in] n 全要素数
  /// @param[in] k 選び出す要素数
  GenBase(unsigned int n,
	  unsigned int k);

  /// @brief コピーコンストラクタ
  GenBase(const GenBase& src);

  /// @brief 全要素数を得る．
  /// @return 全要素数
  unsigned int
  n() const;

  /// @brief 選択する要素数を得る．
  /// @return 選択する要素数
  unsigned int
  k() const;

  /// @brief 最初の要素を指すように初期化する．
  void
  init();

  /// @brief 要素の取得
  /// @param[in] pos 取り出す要素の位置 (最初の位置は 0)
  /// @return pos 番目の要素
  unsigned int
  operator()(unsigned int pos) const;

  /// @brief 末尾のチェック
  /// @return 末尾の時に true を返す．
  bool
  is_end() const;

};
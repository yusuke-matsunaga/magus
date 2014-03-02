
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

  /// @brief 要素の取得(リスト型)
  SIP_PYLIST
  get() const;
  %MethodCode
  ymuint num = sipCpp->k();
  sipRes = PyList_New(num);
  for (ymuint i = 0; i < num; ++ i) {
    PyObject* obj1 = PyLong_FromUnsignedLong(sipCpp->operator()(i));
    PyList_SetItem(sipRes, i, obj1);
  }
  %End

  /// @brief 末尾のチェック
  /// @return 末尾の時に true を返す．
  bool
  is_end() const;

};

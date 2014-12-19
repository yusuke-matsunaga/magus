#ifndef YMSLLABEL_H
#define YMSLLABEL_H

/// @file YmslLabel.h
/// @brief YmslLabel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslLabel YmslLabel.h "YmslLabel.h"
/// @brief ラベルを表すクラス
//////////////////////////////////////////////////////////////////////
class YmslLabel
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  YmslLabel(ShString name);

  /// @brief デストラクタ
  ~YmslLabel();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  ShString
  name() const;

  /// @brief 配置済みか調べる．
  bool
  is_placed() const;

  /// @brief 配置する
  /// @param[in] addr アドレス
  void
  place(Ymsl_INT addr);

  /// @brief 参照する．
  /// @param[in] p_ref アドレスを書き込む位置
  void
  refer(Ymsl_INT* p_ref);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // 配置済みフラグ
  bool mPlaced;

  // 配置位置
  Ymsl_INT mAddr;

  // 参照リスト
  vector<Ymsl_INT*> mRefList;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLLABEL_H

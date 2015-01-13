#ifndef YMSLENUM_H
#define YMSLENUM_H

/// @file YmslEnum.h
/// @brief YmslEnum のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslEnumConst YmslEnum.h "YmslEnum.h"
/// @brief enum 定数を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslEnumConst
{
  friend class YmslEnum;

public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] val 値
  YmslEnumConst(ShString name,
		int val);

  /// @brief デストラクタ
  ~YmslEnumConst();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  ShString
  name() const;

  /// @brief 値を返す．
  int
  val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // 値
  int mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class YmslEnum YmslEnum.h "YmslEnum.h"
/// @brief enum を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslEnum
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] elem_list 要素のリスト
  YmslEnum(ShString name,
	   const vector<ShString>& elem_list);

  /// @brief デストラクタ
  ~YmslEnum();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  ShString
  name() const;

  /// @brief 要素数を返す．
  ymuint
  elem_num() const;

  /// @brief 要素名を返す．
  /// @param[in] pos 位置 ( 0 <= pos < elem_num() )
  ShString
  elem(ymuint pos) const;

  /// @brief 名前からインデックスを返す．
  /// @param[in] elem_name 要素名
  ///
  /// 見つからなかった時は -1 を返す．
  int
  elem_index(ShString elem_name) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // 要素数
  ymuint mElemNum;

  // 要素名の配列
  ShString* mElemName;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLENUM_H

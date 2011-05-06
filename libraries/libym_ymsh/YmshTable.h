#ifndef LIBYM_YMSH_YMSHTABLE_H
#define LIBYM_YMSH_YMSHTABLE_H

/// @file libym_ymsh/YmshTable.h
/// @brief YmshTable のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_ymsh/ymsh_nsdef.h"


BEGIN_NAMESPACE_YM_YMSH

//////////////////////////////////////////////////////////////////////
/// @class YmshTable YmshTable.h "YmshTable.h"
/// @brief 名前をキーにする連想配列
//////////////////////////////////////////////////////////////////////
template<typename T>
class YmshTable
{
public:

  /// @brief コンストラクタ
  YmshTable();

  /// @brief デストラクタ
  ~YmshTable();


public:

  /// @brief 要素を追加する．
  /// @param[in] obj 追加する要素
  /// @retval true 追加が成功した．
  /// @retval false 追加が失敗した．
  /// @note 同名の要素がすでに登録されていたらエラーとなる．
  bool
  reg_obj(T* obj);

  /// @brief 要素をテーブルから取り除く
  /// @param[in] obj 取り除く要素
  /// @note obj が登録されていなくてもエラーにはならない．
  void
  unreg_obj(T* obj);

  /// @brief 要素を検索する．
  /// @param[in] name 名前
  /// @return name という名の要素を返す．
  /// @note 見つからなければ NULL を返す．
  T*
  find(const char* name);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  //
};

END_NAMESPACE_YM_YMSH

#endif // LIBYM_YMSH_YMSHTABLE_H

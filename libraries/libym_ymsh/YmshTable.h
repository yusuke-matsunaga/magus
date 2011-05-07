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
/// @class YmshTableBase YmshTable.h "YmshTable.h"
/// @brief YmshTable の基底クラス
//////////////////////////////////////////////////////////////////////
class YmshTableBase
{
public:

  /// @brief コンストラクタ
  YmshTableBase();

  /// @brief デストラクタ
  ~YmshTableBase();


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
  find(const char* name) const;

  /// @brief 全要素の名前のリストを返す．
  /// @param[out] name_list 名前のリストを格納する変数
  void
  get_name_list(vector<const char*>& name_list) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Cell の領域確保用のオブジェクト
  UnitAlloc mAlloc;

  // ハッシュ表のサイズ
  ymuint32 mSize;

  // ハッシュ表
  Cell** mTable;

  // ハッシュ表を拡大するしきい値
  ymuint32 mLimit;

  // 要素数
  ymuint32 mNum;

};


//////////////////////////////////////////////////////////////////////
/// @class YmshTable YmshTable.h "YmshTable.h"
/// @brief 名前をキーにする連想配列
//////////////////////////////////////////////////////////////////////
template<typename T>
class YmshTable :
  public YmshTableBase
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
  find(const char* name) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  //
};

END_NAMESPACE_YM_YMSH

#endif // LIBYM_YMSH_YMSHTABLE_H

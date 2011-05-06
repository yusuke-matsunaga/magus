#ifndef LIBYM_YMSH_YMSHOBJTABLE_H
#define LIBYM_YMSH_YMSHOBJTABLE_H

/// @file libym_ymsh/YmshObjTable.h
/// @brief YmshObjTable のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_ymsh/ymsh_nsdef.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_YMSH

//////////////////////////////////////////////////////////////////////
/// @class YmshObjTable YmshObjTable.h "YmshObjTable.h"
/// @brief 名前付きオブジェクト用のハッシュテーブル
//////////////////////////////////////////////////////////////////////
class YmshObjTable
{
public:

  /// @brief コンストラクタ
  YmshObjTable();

  /// @brief デストラクタ
  ~YmshObjTable();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部に公開している関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を空にする．
  void
  clear();

  /// @brief 登録されている要素数を返す．
  ymuint
  num() const;

  /// @brief 登録されている名前のリストを返す．
  /// @param[out] name_list 答を格納するリスト
  /// @return 要素数を返す．
  ymuint
  get_name_list(vector<const char*>& name_list) const;

  /// @brief 要素の追加を行う．
  /// @param[in] name 名前
  /// @return 要素番号を返す．
  /// @note すでに name という要素が登録されていた場合にはその番号を返す．
  ymuint
  reg_obj(const char* name);

  /// @brief 要素をテーブルから取り除く
  /// @param[in] name 取り除く名前
  /// @note name という要素が登録されていなければなにもしない．
  void
  unreg_obj(const char* name);

  /// @brief name という名の要素を探す．
  /// @param[in] name 名前
  /// @return 要素番号を返す．
  /// @note 見つからなかった場合には kBadId が返される．
  ymuint
  find_obj(const char* name) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部に公開している定数
  //////////////////////////////////////////////////////////////////////

  /// @brief 無効な要素番号
  static
  const ymuint32 kBadId = 0xFFFFFFFF;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Cell を確保する．
  Cell*
  new_cell();

  /// @brief Cell を解放する．
  void
  free_cell(Cell* cell);

  /// @brief テーブルの領域を確保する．
  /// @param[in] req_size 要求するサイズ
  void
  alloc_table(ymuint req_size);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Cell
  {
    // 名前
    const char* mName;

    // 要素番号
    ymuint32 mId;

    // つぎのセルを指すポインタ
    Cell* mLink;

  };


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

END_NAMESPACE_YM_YMSH

#endif // LIBYM_YMSH_YMSHOBJTABLE_H

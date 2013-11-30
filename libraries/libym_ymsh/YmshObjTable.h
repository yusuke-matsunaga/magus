#ifndef LIBYM_YMSH_YMSHOBJTABLE_H
#define LIBYM_YMSH_YMSHOBJTABLE_H

/// @file libym_ymsh/YmshObjTable.h
/// @brief YmshObjTable のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_ymsh/ymsh_nsdef.h"
#include "ym_utils/UnitAlloc.h"
#include "ym_utils/StrBuff.h"


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
  void
  get_name_list(vector<const char*>& name_list) const;

  /// @brief name という名の要素が登録されているか調べる．
  /// @param[in] name 名前
  bool
  check_obj(const char* name) const;

  /// @brief name という名の値を返す．
  /// @param[in] name 名前
  /// @note 登録してなければ新しく作る．
  vector<string>&
  get_obj(const char* name);

  /// @brief 要素をテーブルから取り除く
  /// @param[in] name 取り除く名前
  /// @note name という要素が登録されていなければなにもしない．
  void
  unreg_obj(const char* name);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Cell
  {

    // コンストラクタ
    Cell(const char* name) :
      mName(name),
      mLink(NULL)
    {
    }

    // デストラクタ
    ~Cell()
    {
    }

    // 名前を返す．
    const char*
    name() const
    {
      return mName.c_str();
    }

    // 名前
    StrBuff mName;

    // 値
    vector<string> mValue;

    // つぎのセルを指すポインタ
    Cell* mLink;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Cell を確保する．
  /// @param[in] name 名前
  Cell*
  new_cell(const char* name);

  /// @brief Cell を解放する．
  void
  free_cell(Cell* cell);

  /// @brief テーブルの領域を確保する．
  /// @param[in] req_size 要求するサイズ
  void
  alloc_table(ymuint req_size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Cell の領域確保用のオブジェクト
  UnitAlloc mAlloc;

  // ハッシュ表のサイズ
  ymuint32 mTableSize;

  // ハッシュ表
  Cell** mTable;

  // ハッシュ表を拡大するしきい値
  ymuint32 mNextLimit;

  // 要素数
  ymuint32 mNum;

};

END_NAMESPACE_YM_YMSH

#endif // LIBYM_YMSH_YMSHOBJTABLE_H

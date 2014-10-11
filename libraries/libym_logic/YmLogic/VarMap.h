#ifndef YMLOGIC_VARMAP_H
#define YMLOGIC_VARMAP_H

/// @file YmLogic/VarMap.h
/// @brief VarMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/VarId.h"
#include "YmUtils/HashBase.h"


BEGIN_NAMESPACE_YM


//////////////////////////////////////////////////////////////////////
/// @class VarMap VarMap.h "YmUtils/VarMap.h"
/// @brief 変数番号をキーにして任意の型を保持する連想配列クラス
//////////////////////////////////////////////////////////////////////
template<typename T>
class VarMap :
  public HashBase<VarId>
{
private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いるデータ構造
  //////////////////////////////////////////////////////////////////////
  struct Cell :
    public HashBase<VarId>::_Cell
  {
    virtual
    ~Cell() { }

    // データ
    T mData;
  };


public:

  /// @brief コンストラクタ
  VarMap() { }

  /// @brief デストラクタ
  ~VarMap() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数番号で検索して要素を得る．
  /// @param[in] varid 変数番号
  /// @param[out] data 結果を格納する変数
  /// @retval true 要素が存在した．
  /// @retval false 要素が存在しなかった．
  bool
  find(VarId varid,
       T& data) const
  {
    _Cell* cell = find_cell(varid);
    if ( cell != NULL ) {
      Cell* mycell = reinterpret_cast<Cell*>(cell);
      data = mycell->mData;
      return true;
    }
    else {
      return false;
    }
  }

  /// @brief 要素を登録する．
  /// @param[in] varid 変数番号
  /// @param[in] data 登録する要素
  void
  reg(VarId varid,
      const T& data)
  {
    Cell* cell = new Cell;
    cell->mKey = key;
    cell->mData = data;
    reg_cell(cell);
  }

  /// @brief 要素のリストを得る．
  void
  data_list(vector<T>& data_list) const
  {
    vector<_Cell*> tmp_list;
    cell_list(tmp_list);
    data_list.clear();
    data_list.reserve(tmp_list.size());
    for (vector<_Cell*>::iterator p = tmp_list.begin();
	 p != tmp_list.end(); ++ p) {
      Cell* cell = reinterpret_cast<Cell*>(*p);
      data_list.push_back(cell->mData);
    }
  }

};

END_NAMESPACE_YM

#endif // YMLOGIC_VARMAP_H

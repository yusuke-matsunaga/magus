#ifndef YMUTILS_DICTIONARY_H
#define YMUTILS_DICTIONARY_H

/// @file YmUtils/Dictionary.h
/// @brief Dictionary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class DictBase DictBase.h "YmUtils/DictBase.h"
/// @brief Dictionary の基底クラス
//////////////////////////////////////////////////////////////////////
class DictBase
{
protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct _Cell
  {

    virtual
    ~_Cell() { }

    /// @brief 名前
    string mName;

    /// @brief 次のセルへのリンク
    _Cell* mLink;
  };


protected:
  //////////////////////////////////////////////////////////////////////
  // このクラスが単独で用いられることはない．
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  DictBase();

  /// @brief デストラクタ
  ~DictBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief 要素数を返す．
  ymuint
  num() const;

  /// @brief 登録されている名前のリストを返す．
  /// @param[out] name_list 名前のリストを格納するリスト
  void
  name_list(vector<string>& name_list) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の一致する要素を返す．
  /// @param[in] name 名前
  /// @return name と一致する要素を返す．
  ///
  /// 見つからなければ NULL を返す．
  _Cell*
  find_cell(const string& name) const;

  /// @brief 要素を登録する．
  /// @param[in] cell 登録する要素
  ///
  /// 同じ名前の要素がすでに登録されていたら動作は不定
  void
  reg_cell(_Cell* cell);

  /// @brief 登録されているセルのリストを得る．
  /// @param[out] cell_list 結果を格納するリスト
  void
  cell_list(vector<_Cell*>& cell_list) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ表を確保する．
  /// @param[in] req_size 表のサイズ
  void
  alloc_table(ymuint req_size);

  /// @brief 要素を登録する．
  /// @param[in] cell 登録する要素
  ///
  /// 同じ名前の要素がすでに登録されていたら動作は不定
  /// reg_cell() との違いはハッシュ表のサイズチェックをしない．
  void
  _reg_cell(_Cell* cell);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表のサイズ
  ymuint32 mHashSize;

  // ハッシュ表
  _Cell** mHashTable;

  // 拡大する目安
  ymuint32 mNextLimit;

  // 要素数
  ymuint32 mNum;

};


//////////////////////////////////////////////////////////////////////
/// @class Ditionary Dictionary.h "YmUtils/Dictionary.h"
/// @brief 名前をキーにして任意の型を保持する連想配列クラス
//////////////////////////////////////////////////////////////////////
template<typename T>
class Dictionary :
  public DictBase
{
 private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Cell :
    public DictBase::_Cell
  {
    virtual
    ~Cell() { }

    // データ
    T mData;
  };


 public:

  /// @brief コンストラクタ
  Dictionary() { }

  /// @brief デストラクタ
  ~Dictionary() { }


 public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前で検索して要素を得る．
  /// @param[in] name 名前
  /// @param[out] data 結果を格納する変数
  /// @retval true 要素が存在した．
  /// @retval false 要素が存在しなかった．
  bool
  find(const string& name,
       T& data) const
  {
    _Cell* cell = find_cell(name);
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
  /// @param[in] name 名前
  /// @param[in] data 登録する要素
  void
  reg(const string& name,
      const T& data)
  {
    Cell* cell = new Cell;
    cell->mName = name;
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

#endif // YMUTILS_DICTIONARY_H

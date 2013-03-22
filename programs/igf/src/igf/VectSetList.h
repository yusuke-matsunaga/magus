#ifndef VECTSETLIST_H
#define VECTSETLIST_H

/// @file VectSetList.h
/// @brief VectSetList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class VectSetList VectSetList.h "VectSetList.h"
/// @brief RegVect の集合のリストを表すクラス
//////////////////////////////////////////////////////////////////////
class VectSetList
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_num 予想される最大要素数
  /// @param[in] max_setnum 予想される最大分割数
  VectSetList(ymuint max_num,
	      ymuint max_setnum);

  /// @brief デストラクタ
  ~VectSetList();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 空の集合を追加する．
  void
  add_set();

  /// @brief 現在の集合に要素を追加する．
  void
  add_vect(RegVect* vect);

  /// @brief 全要素数を得る．
  ymuint
  all_elem_num() const;

  /// @brief 集合の数を得る．
  ymuint
  set_num() const;

  /// @brief 指定された集合の要素数を得る．
  /// @param[in] set_id 集合のID番号 ( 0 <= set_id < set_num() )
  ymuint
  set_size(ymuint set_id) const;

  /// @brief 指定された集合の要素を得る．
  /// @param[in] set_id 集合のID番号 ( 0 <= set_id < set_num() )
  /// @param[in] vec_id ベクタのID番号 ( 0 <= vect_id < set_size(set_id) )
  RegVect*
  set_elem(ymuint set_id,
	   ymuint vect_id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // すべてのベクタを格納する配列
  vector<RegVect*> mBody;

  // 個々の集合の開始位置を記録する配列
  vector<ymuint32> mStartPos;
};

END_NAMESPACE_YM_IGF

#endif // VECTSETLIST_H

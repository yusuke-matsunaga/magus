#ifndef ASSIGNLIST_H
#define ASSIGNLIST_H

/// @file AssignList.h
/// @brief AssignList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class Assign AssignList.h "AssignList.h"
/// @brief ノードに対する値の割当を表すクラス
//////////////////////////////////////////////////////////////////////
class Assign
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 内容は不定
  Assign();

  /// @brief 値を指定したコンストラクタ
  /// @param[in] node_id ノードID
  /// @param[in] val 値
  Assign(ymuint node_id,
	 bool val);


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード番号を返す．
  ymuint
  node_id() const;

  /// @brief 値を返す．
  bool
  val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パックした値
  ymuint32 mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class AssignList AssignList.h "AssignList.h"
/// @brief ノードに対する値の割当を記録するクラス
//////////////////////////////////////////////////////////////////////
class AssignList
{
public:

  /// @brief コンストラクタ
  AssignList();

  /// @brief デストラクタ
  ~AssignList();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を追加する．
  /// @param[in] node_id ノードID
  /// @param[in] val 値
  void
  add(ymuint node_id,
      bool val);

  /// @brief 要素数を返す．
  ymuint
  size() const;

  /// @brief 要素を返す．
  /// @param[in] pos 位置 ( 0 <= pos < size() )
  Assign
  elem(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値割当のリスト
  vector<Assign> mAsList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
//
// 内容は不定
inline
Assign::Assign() :
  mVal(0)
{
}

// @brief 値を指定したコンストラクタ
// @param[in] node_id ノードID
// @param[in] val 値
inline
Assign::Assign(ymuint node_id,
	       bool val) :
  mVal((node_id << 1) | val)
{
}

// @brief ノード番号を返す．
inline
ymuint
Assign::node_id() const
{
  return mVal >> 1;
}

// @brief 値を返す．
inline
bool
Assign::val() const
{
  return static_cast<bool>(mVal & 1U);
}

// @brief コンストラクタ
inline
AssignList::AssignList()
{
}

// @brief デストラクタ
inline
AssignList::~AssignList()
{
}

// @brief 値を追加する．
// @param[in] node_id ノードID
// @param[in] val 値
inline
void
AssignList::add(ymuint node_id,
		bool val)
{
  mAsList.push_back(Assign(node_id, val));
}

// @brief 要素数を返す．
inline
ymuint
AssignList::size() const
{
  return mAsList.size();
}

// @brief 要素を返す．
// @param[in] pos 位置 ( 0 <= pos < size() )
inline
Assign
AssignList::elem(ymuint pos) const
{
  ASSERT_COND( pos < size() );
  return mAsList[pos];
}

END_NAMESPACE_YM_SATPG

#endif // ASSIGNLIST_H

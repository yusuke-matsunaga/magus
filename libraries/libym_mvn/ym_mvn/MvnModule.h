#ifndef YM_MVN_MVNMODULE_H
#define YM_MVN_MVNMODULE_H

/// @file ym_mvn/MvnModule.h
/// @brief MvnModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"
#include "ym_utils/ItvlMgr.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnModule MvnModule.h "ym_mvn/MvnModule.h"
/// @brief 多値ネットワークを表すクラス
//////////////////////////////////////////////////////////////////////
class MvnModule
{
  friend class MvnMgr;

public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内部情報を得るためのメンバ関数
  /// @{

  /// @brief ID番号を得る．
  ymuint
  id() const;

  /// @brief 名前を得る．
  string
  name() const;

  /// @brief 親のノードを得る．
  /// @note トップレベルモジュールの場合には NULL を返す．
  MvnNode*
  parent() const;

  /// @brief ポート数を得る．
  ymuint
  port_num() const;

  /// @brief ポートを得る．
  /// @param[in] pos 位置 ( 0 <= pos < port_num() )
  const MvnPort*
  port(ymuint pos) const;

  /// @brief 入力ノード数を得る．
  ymuint
  input_num() const;

  /// @brief 入力ノードを得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() )
  MvnNode*
  input(ymuint pos) const;

  /// @brief 出力ノード数を得る．
  ymuint
  output_num() const;

  /// @brief 出力ノードを得る．
  /// @param[in] pos 位置 ( 0 <= pos < output_num() )
  MvnNode*
  output(ymuint pos) const;

  /// @brief 入出力ノード数を得る．
  ymuint
  inout_num() const;

  /// @brief 入出力ノードを得る．
  /// @param[in] pos 位置 ( 0 <= pos < inout_num() )
  MvnNode*
  inout(ymuint pos) const;

  /// @brief 内部ノードの数を得る．
  ymuint
  node_num() const;

  /// @brief 内部ノードのリストを得る．
  const list<MvnNode*>&
  node_list() const;

  /// @brief 内部ノードのリストの先頭の反復子を得る．
  list<MvnNode*>::const_iterator
  nodes_begin() const;

  /// @brief 内部ノードのリストの末尾の反復子を得る．
  list<MvnNode*>::const_iterator
  nodes_end() const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] np ポート数
  /// @param[in] ni 入力ノード数
  /// @param[in] no 出力ノード数
  /// @param[in] nio 入出力ノード数
  MvnModule(const char* name,
	    ymuint np,
	    ymuint ni,
	    ymuint no,
	    ymuint nio);

  /// @brief デストラクタ
  ~MvnModule();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint mId;

  // 名前
  string mName;

  // 親のノード
  MvnNode* mParent;

  // ポートの配列
  vector<MvnPort*> mPortArray;

  // 入力ノードの配列
  vector<MvnNode*> mInputArray;

  // 出力ノードの配列
  vector<MvnNode*> mOutputArray;

  // 入出力ノードの配列
  vector<MvnNode*> mInoutArray;

  // 内部ノードのリスト
  list<MvnNode*> mNodeList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID番号を得る．
inline
ymuint
MvnModule::id() const
{
  return mId;
}

// @brief 名前を得る．
inline
string
MvnModule::name() const
{
  return mName;
}

// @brief 親のノードを得る．
// @note トップレベルモジュールの場合には NULL を返す．
inline
MvnNode*
MvnModule::parent() const
{
  return mParent;
}

// @brief ポート数を得る．
inline
ymuint
MvnModule::port_num() const
{
  return mPortArray.size();
}

// @brief ポートを得る．
// @param[in] pos 位置 ( 0 <= pos < port_num() )
inline
const MvnPort*
MvnModule::port(ymuint pos) const
{
  return mPortArray[pos];
}

// @brief 入力ノード数を得る．
inline
ymuint
MvnModule::input_num() const
{
  return mInputArray.size();
}

// @brief 入力ノードを得る．
// @param[in] pos 位置 ( 0 <= pos < input_num() )
inline
MvnNode*
MvnModule::input(ymuint pos) const
{
  return mInputArray[pos];
}

// @brief 出力ノード数を得る．
inline
ymuint
MvnModule::output_num() const
{
  return mOutputArray.size();
}

// @brief 出力ノードを得る．
// @param[in] pos 位置 ( 0 <= pos < output_num() )
inline
MvnNode*
MvnModule::output(ymuint pos) const
{
  return mOutputArray[pos];
}

// @brief 入出力ノード数を得る．
inline
ymuint
MvnModule::inout_num() const
{
  return mInoutArray.size();
}

// @brief 入出力ノードを得る．
// @param[in] pos 位置 ( 0 <= pos < inout_num() )
inline
MvnNode*
MvnModule::inout(ymuint pos) const
{
  return mInoutArray[pos];
}

// @brief 内部ノードの数を得る．
inline
ymuint
MvnModule::node_num() const
{
  return mNodeList.size();
}

// @brief 内部ノードのリストを得る．
inline
const list<MvnNode*>&
MvnModule::node_list() const
{
  return mNodeList;
}

// @brief 内部ノードのリストの先頭の反復子を得る．
inline
list<MvnNode*>::const_iterator
MvnModule::nodes_begin() const
{
  return mNodeList.begin();
}

// @brief 内部ノードのリストの末尾の反復子を得る．
inline
list<MvnNode*>::const_iterator
MvnModule::nodes_end() const
{
  return mNodeList.end();
}

END_NAMESPACE_YM_MVN

#endif // YM_MVN_MVNMODULE_H

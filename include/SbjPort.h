#ifndef SBJPORT_H
#define SBJPORT_H

/// @file SbjPort.h
/// @brief SbjPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "sbj_nsdef.h"


BEGIN_NAMESPACE_YM_SBJ

//////////////////////////////////////////////////////////////////////
/// @class SbjPort SbjGraph.h "SbjGraph.h"
/// @brief ポートを表すクラス
///
/// ポートは通常，名前を持ち，1つもしくは複数の入出力ノード
/// と対応づけられる．
//////////////////////////////////////////////////////////////////////
class SbjPort
{
  friend class SbjGraph;

private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] body 入出力ノードのベクタ
  SbjPort(const string& name,
	  const vector<SbjNode*>& body);

  /// @brief デストラクタ
  ~SbjPort();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を得る．
  string
  name() const;

  /// @brief ビット数を得る．
  int
  bit_width() const;

  /// @brief pos ビット目の内容を得る．
  /// @param[in] pos ビット位置 ( 0 <= pos < bit_width() )
  const SbjNode*
  bit(int pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  string mName;

  // 入出力ノードのベクタ
  vector<SbjNode*> mBody;

};


//////////////////////////////////////////////////////////////////////
// inline 関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] body 入出力ノードのベクタ
inline
SbjPort::SbjPort(const string& name,
		 const vector<SbjNode*>& body) :
  mName(name),
  mBody(body)
{
}

// @brief デストラクタ
inline
SbjPort::~SbjPort()
{
}

// @brief 名前を得る．
inline
string
SbjPort::name() const
{
  return mName;
}

// @brief ビット数を得る．
inline
int
SbjPort::bit_width() const
{
  return mBody.size();
}

// @brief pos ビット目の内容を得る．
// @param[in] pos ビット位置 ( 0 <= pos < bit_width() )
inline
const SbjNode*
SbjPort::bit(int pos) const
{
  ASSERT_COND( pos >= 0 && pos < bit_width() );

  return mBody[pos];
}

END_NAMESPACE_YM_SBJ

#endif // SBJGRAPH_H

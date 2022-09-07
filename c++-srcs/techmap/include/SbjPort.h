#ifndef SBJPORT_H
#define SBJPORT_H

/// @file SbjPort.h
/// @brief SbjPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "sbj_nsdef.h"


BEGIN_NAMESPACE_SBJ

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
  SbjPort(
    const string& name,          ///< [in] 名前
    const vector<SbjNode*>& body ///< [in] 入出力ノードのベクタ
  ) : mName(name),
      mBody(body)
  {
  }

  /// @brief デストラクタ
  ~SbjPort() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を得る．
  string
  name() const
  {
    return mName;
  }

  /// @brief ビット数を得る．
  SizeType
  bit_width() const
  {
    return mBody.size();
  }

  /// @brief pos ビット目の内容を得る．
  const SbjNode*
  bit(
    SizeType pos ///< [in] ビット位置 ( 0 <= pos < bit_width() )
  ) const
  {
    ASSERT_COND( pos >= 0 && pos < bit_width() );

    return mBody[pos];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  string mName;

  // 入出力ノードのベクタ
  vector<SbjNode*> mBody;

};

END_NAMESPACE_SBJ

#endif // SBJGRAPH_H

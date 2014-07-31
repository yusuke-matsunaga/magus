#ifndef CNFCLAUSE_H
#define CNFCLAUSE_H

/// @file CnfClause.h
/// @brief CnfClause のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ImpBase.h"
#include "YmLogic/Literal.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class CnfClause CnfClause.h "CnfClause.h"
/// @brief CNF の節を表すクラス
//////////////////////////////////////////////////////////////////////
class CnfClause
{
public:

  /// @brief コンストラクタ
  CnfClause();

  /// @brief デストラクタ
  ~CnfClause();


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief リテラル数を設定する．
  /// @param[in] n 設定するリテラル数
  /// @note 内容は未定義
  void
  set_literal_num(ymuint n);

  /// @brief リテラルを設定する．
  /// @param[in] pos 位置番号 ( 0 <= pos < literal_num() )
  /// @param[in] lit 設定するリテラル
  void
  set_literal(ymuint pos,
	      Literal lit);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief リテラル数を返す．
  ymuint
  literal_num() const;

  /// @brief リテラルを取り出す．
  /// @param[in] pos 位置番号 ( 0 <= pos < literal_num() )
  Literal
  literal(ymuint pos) const;

  /// @brief 指定したリテラルが含まれているか調べる．
  bool
  is_in(Literal lit) const;

  /// @brief 内容を出力する．
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // リテラルのリスト
  vector<Literal> mLitList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
CnfClause::CnfClause()
{
}

// @brief デストラクタ
inline
CnfClause::~CnfClause()
{
}

// @brief リテラル数を設定する．
// @param[in] n 設定するリテラル数
// @note 内容は未定義
inline
void
CnfClause::set_literal_num(ymuint n)
{
  mLitList.resize(n);
}

// @brief リテラルを設定する．
// @param[in] pos 位置番号 ( 0 <= pos < literal_num() )
// @param[in] lit 設定するリテラル
inline
void
CnfClause::set_literal(ymuint pos,
		       Literal lit)
{
  assert_cond( pos < literal_num(), __FILE__, __LINE__);
  mLitList[pos] = lit;
}

// @brief リテラル数を返す．
inline
ymuint
CnfClause::literal_num() const
{
  return mLitList.size();
}

// @brief リテラルを取り出す．
// @param[in] pos 位置番号 ( 0 <= pos < literal_num() )
inline
Literal
CnfClause::literal(ymuint pos) const
{
  assert_cond( pos < literal_num(), __FILE__, __LINE__);
  return mLitList[pos];
}

// @brief 指定したリテラルが含まれているか調べる．
inline
bool
CnfClause::is_in(Literal lit) const
{
  for (vector<Literal>::const_iterator p = mLitList.begin();
       p != mLitList.end(); ++ p) {
    if ( *p == lit ) {
      return true;
    }
  }
  return false;
}

// @brief 内容を出力する．
inline
void
CnfClause::print(ostream& s) const
{
  const char* plus = "";
  for (vector<Literal>::const_iterator p = mLitList.begin();
       p != mLitList.end(); ++ p) {
    cout << plus << *p;
    plus = " + ";
  }
  cout << endl;
}

END_NAMESPACE_YM_NETWORKS

#endif // CNFCLAUSE_H

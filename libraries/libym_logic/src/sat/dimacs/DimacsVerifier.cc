
/// @file DimacsVerifier.cc
/// @brief DimacsVerifier の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/DimacsVerifier.h"


BEGIN_NAMESPACE_YM_SAT

// @brief コンストラクタ
DimacsVerifier::DimacsVerifier()
{
}

// @brief デストラクタ
DimacsVerifier::~DimacsVerifier()
{
}

// @brief 初期化
bool
DimacsVerifier::init()
{
  return true;
}

// @brief p 行の読込み
// @param[in] loc ファイル上の位置情報
// @param[in] nv 変数の数
// @param[in] nc 節の数
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
DimacsVerifier::read_p(const FileRegion& loc,
		       ymuint nv,
		       ymuint nc)
{
  return true;
}

// @brief clause 行の読込み
// @param[in] loc ファイル上の位置情報
// @param[in] lits リテラルの配列．最後の0は含まない
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
DimacsVerifier::read_clause(const FileRegion& loc,
			    const vector<int>& lits)
{
  clause_list.push_back(lits);

  return true;
}

// @brief 終了処理
// @param[in] loc 位置情報
bool
DimacsVerifier::end()
{
  return true;
}

// @brief エラー終了時の処理
void
DimacsVerifier::error_exit()
{
}

// @brief 検証する．
bool
DimacsVerifier::verify(const vector<Bool3>& model) const
{
  for (vector<vector<int> >::const_iterator p = clause_list.begin();
       p != clause_list.end(); ++ p) {
    const vector<int>& lits = *p;
    bool sat = false;
    for (vector<int>::const_iterator q = lits.begin();
	 q != lits.end(); ++ q) {
      int l = *q;
      if ( l > 0 ) {
	if ( model[l - 1] == kB3True ) {
	  sat = true;
	  break;
	}
      }
      else {
	if ( model[-l - 1] == kB3False ) {
	  sat = true;
	  break;
	}
      }
    }
    if ( !sat ) {
      const char* plus = "";
      for (vector<int>::const_iterator q = lits.begin();
	   q != lits.end(); ++ q) {
	int l = *q;
	cout << plus << l;
	plus = " + ";
      }
      cout << " is unsatisfied" << endl;
      return false;
    }
  }
  return true;
}

END_NAMESPACE_YM_SAT


/// @file NodeDumper.cc
/// @brief NodeDumper とその継承クラスの実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "NodeDumper.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN

//////////////////////////////////////////////////////////////////////
// クラス UnaryOpDumper
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] opr_str 演算子を表す文字列
UnaryOpDumper::UnaryOpDumper(const char* opr_str) :
  mOprStr(opr_str)
{
}

// @brief デストラクタ
UnaryOpDumper::~UnaryOpDumper()
{
}

// @brief ノードの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
UnaryOpDumper::operator()(ostream& s,
			  const MvnNode* node)
{
  ymuint ni = node->input_num();
  assert_cond( ni == 1, __FILE__, __LINE__);
  ymuint no = node->output_num();
  assert_cond( no == 1, __FILE__, __LINE__);

  const MvnInputPin* ipin = node->input(0);
  const MvnOutputPin* src_pin = ipin->src_pin();
  assert_cond( src_pin != NULL, __FILE__, __LINE__);

  s << mOprStr << net_name(src_pin);
}


//////////////////////////////////////////////////////////////////////
// クラス BinaryOpDumper
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] opr_str 演算子を表す文字列
// @param[in] need_paren 括弧をつける時に true にするフラグ
BinaryOpDumper::BinaryOpDumper(const char* opr_str,
			       bool need_paren) :
  mOprStr(opr_str),
  mNeedParen(need_paren)
{
}

// @brief デストラクタ
BinaryOpDumper::~BinaryOpDumper()
{
}

// @brief ノードの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
BinaryOpDumper::operator()(ostream& s,
			   const MvnNode* node)
{
  ymuint ni = node->input_num();
  assert_cond( ni == 2, __FILE__, __LINE__);
  ymuint no = node->output_num();
  assert_cond( no == 1, __FILE__, __LINE__);

  const MvnInputPin* ipin0 = node->input(0);
  const MvnOutputPin* src_pin0 = ipin0->src_pin();
  assert_cond( src_pin0 != NULL, __FILE__, __LINE__);

  const MvnInputPin* ipin1 = node->input(1);
  const MvnOutputPin* src_pin1 = ipin1->src_pin();
  assert_cond( src_pin1 != NULL, __FILE__, __LINE__);

  const char* lp = "";
  const char* rp = "";
  if ( mNeedParen ) {
    lp = "(";
    rp = ")";
  }

  s << lp << net_name(src_pin0)
    << " " << mOprStr << " "
    << net_name(src_pin1) << rp;
}


//////////////////////////////////////////////////////////////////////
// クラス TernaryOpDumper
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] opr_str1, opr_str2 演算子を表す文字列
TernaryOpDumper::TernaryOpDumper(const char* opr_str1,
				 cosnt char* opr_str2) :
  mOprStr1(opr_str1),
  mOprStr2(opr_str2)
{
}

// @brief デストラクタ
TernaryOpDumper::~TernaryOpDumper()
{
}

// @brief ノードの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
TernaryOpDumper::operator()(ostream& s,
			    const MvnNode* node)
{
  ymuint ni = node->input_num();
  assert_cond( ni == 3, __FILE__, __LINE__);
  ymuint no = node->output_num();
  assert_cond( no == 1, __FILE__, __LINE__);

  const MvnInputPin* ipin0 = node->input(0);
  const MvnOutputPin* src_pin0 = ipin0->src_pin();
  assert_cond( src_pin0 != NULL, __FILE__, __LINE__);

  const MvnInputPin* ipin1 = node->input(1);
  const MvnOutputPin* src_pin1 = ipin1->src_pin();
  assert_cond( src_pin1 != NULL, __FILE__, __LINE__);

  const MvnInputPin* ipin2 = node->input(2);
  const MvnOutputPin* src_pin2 = ipin2->src_pin();
  assert_cond( src_pin2 != NULL, __FILE__, __LINE__);

  s << net_name(src_pin0) << " " << mOprStr1 << " "
    << net_name(src_pin1) << " " << mOprStr2 << " "
    << net_name(src_pin2);
}


//////////////////////////////////////////////////////////////////////
// クラス NaryOpDumper
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] opr_str 演算子を表す文字列
NaryOpDumper::NaryOpDumper(const char* opr_str) :
  mOprStr(opr_str)
{
}

// @brief デストラクタ
NaryOpDumper::~NaryOpDumper()
{
}

// @brief ノードの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
NaryOpDumper::operator()(ostream& s,
			 const MvnNode* node)
{
  ymuint ni = node->input_num();
  assert_cond( ni >= 2, __FILE__, __LINE__);
  ymuint no = node->output_num();
  assert_cond( no == 1, __FILE__, __LINE__);

  const MvnInputPin* ipin0 = node->input(0);
  const MvnOutputPin* src_pin0 = ipin0->src_pin();
  assert_cond( src_pin0 != NULL, __FILE__, __LINE__);

  s << net_name(src_pin0);
  for (ymuint i = 1; i < ni; ++ i) {
    const MvnInputPin* ipin1 = node->input(i);
    const MvnOutputPin* src_pin1 = ipin1->src_pin();
    assert_cond( src_pin1 != NULL, __FILE__, __LINE__);
    s << " " << mOprStr << " " << net_name(src_pin1);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス ConcatDumper
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ConcatDumper::ConcatDumper()
{
}

// @brief デストラクタ
ConcatDumper::~ConcatDumper()
{
}

// @brief ノードの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
ConcatDumper::operator()(ostream& s,
			 const MvnNode* node)
{
  ymuint no = node->output_num();
  assert_cond( no == 1, __FILE__, __LINE__);

  s << "{";
  const char* comma = "";
  ymuint ni = node->input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const MvnInputPin* ipin = node->input(i);
    const MvnOutputPin* opin = ipin->src_pin();
    assert_cond( opin != NULL, __FILE__, __LINE__);

    s << comma << net_name(opin);
    comma = ", ";
  }
  s << "};" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス CaseEqDumper
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CaseEqDumper::CaseEqDumper()
{
}

// @brief デストラクタ
CaseEqDumper::~CaseEqDumper()
{
}

// @brief ノードの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
CaseEqDumper::operator()(ostream& s,
			 const MvnNode* node)
{
}


//////////////////////////////////////////////////////////////////////
// クラス ConstBitSelectDumper
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ConstBitSelectDumper::ConstBitSelectDumper()
{
}

// @brief デストラクタ
ConstBitSelectDumper::~ConstBitSelectDumper()
{
}

// @brief ノードの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
ConstBitSelectDumper::operator()(ostream& s,
				 const MvnNode* node)
{
}


//////////////////////////////////////////////////////////////////////
// クラス ConstPartSelectDumper
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ConstPartSelectDumper::ConstPartSelectDumper()
{
}

// @brief デストラクタ
ConstPartSelectDumper::~ConstPartSelectDumper()
{
}

// @brief ノードの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
ConstPartSelectDumper::operator()(ostream& s,
				  const MvnNode* node)
{
}


//////////////////////////////////////////////////////////////////////
// クラス BitSelectDumper
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BitSelectDumper::BitSelectDumper()
{
}

// @brief デストラクタ
BitSelectDumper::~BitSelectDumper()
{
}

// @brief ノードの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
BitSelectDumper::operator()(ostream& s,
			    const MvnNode* node)
{
}


//////////////////////////////////////////////////////////////////////
// クラス PartSelectDumper
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PartSelectDumper::PartSelectDumper()
{
}

// @brief デストラクタ
PartSelectDumper::~PartSelectDumper()
{
}

// @brief ノードの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
PartSelectDumper::operator()(ostream& s,
			     const MvnNode* node)
{
}


END_NAMESPACE_YM_NETWORKS_MVN

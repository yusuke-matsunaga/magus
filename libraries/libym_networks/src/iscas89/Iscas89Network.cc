
/// @file Iscas89Network.cc
/// @brief Iscas89Network の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Iscas89Network.h"


BEGIN_NAMESPACE_YM_ISCAS89

//////////////////////////////////////////////////////////////////////
// Iscas89Node
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID 番号
Iscas89Node::Iscas89Node(ymuint32 id) :
  mId(id),
  mType(kUndef),
  mNi(0),
  mFanins(NULL)
{
}

// @brief デストラクタ
Iscas89Node::~Iscas89Node()
{
}


//////////////////////////////////////////////////////////////////////
// Iscas89Network
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Iscas89Network::Iscas89Network() :
  mAlloc(4096)
{
}

// @brief デストラクタ
Iscas89Network::~Iscas89Network()
{
  // メモリは mAlloc のデストラクタで適切に解放される．
}

// @brief 内容をクリアする．
void
Iscas89Network::clear()
{
  mNodeArray.clear();
  mPIArray.clear();
  mPOArray.clear();
  mFFArray.clear();
  mGateArray.clear();
}

// @brief ノードを取り出す．
// @param[in] id ID番号
// @note なければ作成する．
Iscas89Node*
Iscas89Network::get_node(ymuint32 id)
{
  ymuint32 n = mNodeArray.size();
  while ( n <= id ) {
    mNodeArray.push_back(NULL);
    ++ n;
  }

  Iscas89Node* node = mNodeArray[id];
  if ( node == NULL ) {
    void* p = mAlloc.get_memory(sizeof(Iscas89Node));
    node = new (p) Iscas89Node(id);
    mNodeArray[id] = node;
  }

  return node;
}

// @brief ノード名を設定する．
// @param[in] node 対象のノード
// @param[in] name 名前
void
Iscas89Network::set_node_name(Iscas89Node* node,
			      const char* name)
{
  ymuint32 l = strlen(name);
  void* p = mAlloc.get_memory(l + 1);
  char* buff = new (p) char[l + 1];
  node->mName = buff;
  while ( (*buff ++ = *name ++) ) ;
}

// @brief ノードを外部入力に設定する．
// @param[in] node 対象のノード
// @note 既に他の型に設定されていたら false を返す．
bool
Iscas89Network::set_input_type(Iscas89Node* node)
{
  if ( node->mType != Iscas89Node::kUndef ) {
    return false;
  }
  node->mType = Iscas89Node::kInput;
  mPIArray.push_back(node);
  return true;
}

// @brief ノードをゲートに設定する．
// @param[in] node 対象のノード
// @param[in] ni ファンイン数
// @param[in] type 型
// @note すでに他の型に設定されていたら false を返す．
bool
Iscas89Network::set_gate_type(Iscas89Node* node,
			      ymuint32 ni,
			      tIscas89GateType gate_type)
{
  if ( node->mType != Iscas89Node::kUndef ) {
    return false;
  }
  node->mType = Iscas89Node::kGate;
  node->mGateType = gate_type;

  node->mNi = ni;
  void* p = mAlloc.get_memory(sizeof(Iscas89Node*) * ni);
  node->mFanins = new (p) Iscas89Node*[ni];

  if ( gate_type == kGt89DFF ) {
    mFFArray.push_back(node);
  }
  else {
    mGateArray.push_back(node);
  }

  return true;
}

// @brief 内容を iscas89 形式で出力する．
// @param[in] s 出力先のストリーム
void
Iscas89Network::write_iscas89(ostream& s) const
{
  for (ymuint i = 0; i < npi(); ++ i) {
    const Iscas89Node* node = pi(i);
    s << "INPUT(" << node->name() << ")" << endl;
  }
  s << endl;

  for (ymuint i = 0; i < npo(); ++ i) {
    const Iscas89Node* node = po(i);
    s << "OUTPUT(" << node->name() << ")" << endl;
  }
  s << endl;

  for (ymuint i = 0; i < nff(); ++ i) {
    const Iscas89Node* node = ff(i);
    const Iscas89Node* inode = node->fanin(0);
    s << node->name() << " = DFF(" << inode->name() << ")" << endl;
  }
  s << endl;

  for (ymuint i = 0; i < ngate(); ++ i) {
    const Iscas89Node* node = gate(i);
    s << node->name() << " = ";
    switch ( node->gate_type() ) {
    case kGt89BUFF: s << "BUF"; break;
    case kGt89NOT:  s << "NOT"; break;
    case kGt89AND:  s << "AND"; break;
    case kGt89NAND: s << "NAND"; break;
    case kGt89OR:   s << "OR"; break;
    case kGt89NOR:  s << "NOR"; break;
    case kGt89XOR:  s << "XOR"; break;
    case kGt89XNOR: s << "XNOR"; break;
    default: assert_not_reached(__FILE__, __LINE__); break;
    }
    s << "(";
    const char* comma = "";
    for (ymuint j = 0; j < node->fanin_num(); ++ j) {
      const Iscas89Node* inode = node->fanin(j);
      s << comma << inode->name();
      comma = ", ";
    }
    s << ")" << endl;
  }
}

END_NAMESPACE_YM_ISCAS89


/// @file libym_blif/BlifNetworkReader.cc
/// @brief BlifNetworkReader の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BlifNetworkReader.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include <ym_blif/BlifNetworkReader.h>
#include <ym_blif/BlifNetwork.h>
#include <ym_blif/BlifHandler.h>


BEGIN_NAMESPACE_YM_BLIF

//////////////////////////////////////////////////////////////////////
/// @class BlifNetworkHandler BlifNetworkReader.cc
/// @brief BlifNetwork 用の BlifHandler
//////////////////////////////////////////////////////////////////////
class BlifNetworkHandler :
  public BlifHandler
{
public:

  /// @brief コンストラクタ
  BlifNetworkHandler();

  /// @brief デストラクタ
  virtual
  ~BlifNetworkHandler();


public:

  /// @brief 読み込む対象のネットワークを設定する．
  void
  set_network(BlifNetwork* network);

  /// @brief 初期化
  virtual
  bool
  init();

  /// @brief .model 文中の文字列の処理
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  model(const FileRegion& loc1,
	const FileRegion& loc2,
	const char* name);

  /// @brief .inputs 文中の文字列の処理
  /// @param[in] name_id 文字列の情報
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  inputs_elem(ymuint32 name_id);

  /// @brief .outputs 文中の文字列の処理
  /// @param[in] name_id 文字列の情報
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  outputs_elem(ymuint32 name_id);

  /// @brief .names 文の処理
  /// @param[in] name_id_array 各識別子のID番号の配列
  /// @param[in] nc キューブ数
  /// @param[in] cover_pat 入力カバーを表す文字列
  /// @param[in] opat 出力の極性
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  /// @note cover_pat は ni 個ごとに1行のパタンを表す．
  /// 各要素のとりうる値は '0', '1', '-' を表す．
  /// @note opat は '0' か '1' のどちらか
  virtual
  bool
  names(const vector<ymuint32>& name_id_array,
	ymuint32 nc,
	const char* cover_pat,
	char opat);

  /// @brief .gate 文の開始
  /// @param[in] loc1 .gate の位置情報
  /// @param[in] loc2 セル名の位置情報
  /// @param[in] name セル名
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  gate_begin(const FileRegion& loc1,
	     const FileRegion& loc2,
	     const char* name);

  /// @brief .gate 文中のピン割り当ての処理
  /// @param[in] loc1 ピン名の位置情報
  /// @param[in] f_name ピン名
  /// @param[in] loc2 ノード名の位置情報
  /// @param[in] a_name ノード名
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  gate_assign(const FileRegion& loc1,
	      const char* f_name,
	      const FileRegion& loc2,
	      const char* a_name);

  /// @brief .gate 文の終了
  virtual
  bool
  gate_end();

  /// @brief .latch 文中の本体の処理
  /// @param[in] name1_id 最初の文字列
  /// @param[in] name2_id 次の文字列
  /// @param[in] loc4 リセット値の位置情報
  /// @param[in] rval リセット時の値 ('0'/'1') 未定義なら ' '
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  latch(ymuint32 name1_id,
	ymuint32 name2_id,
	const FileRegion& loc4,
	char rval);

  /// @brief .end 文の処理
  /// @param[in] loc 位置情報
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  end(const FileRegion& loc);

  /// @brief 通常終了時の処理
  virtual
  void
  normal_exit();

  /// @brief エラー終了時の処理
  virtual
  void
  error_exit();

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のネットワーク
  BlifNetwork* mNetwork;
  
};


// @brief コンストラクタ
BlifNetworkHandler::BlifNetworkHandler() :
  mNetwork(NULL)
{
}

// @brief デストラクタ
BlifNetworkHandler::~BlifNetworkHandler()
{
}

// @brief 読み込む対象のネットワークを設定する．
void
BlifNetworkHandler::set_network(BlifNetwork* network)
{
  mNetwork = network;
}

// @brief 初期化
bool
BlifNetworkHandler::init()
{
  mNetwork->clear();
  return true;
}
  
// @brief .model 文の読み込み
// @param[in] loc 位置情報
// @param[in] name model名
bool
BlifNetworkHandler::model(const FileRegion& loc1,
			  const FileRegion& loc2,
			  const char* name)
{
  mNetwork->mName = name;
  return true;
}
  
// @brief .input 文の読み込み
// @param[in] loc 位置情報
// @param[in] name 入力ノード名
bool
BlifNetworkHandler::inputs_elem(ymuint32 name_id)
{
  BlifNode* node = mNetwork->get_node(name_id);
  if ( !mNetwork->set_input_type(node) ) {
    return false;
  }
  return true;
}
  
// @brief .output 文の読み込み
// @param[in] loc 位置情報
// @param[in] name 出力ノード名
bool
BlifNetworkHandler::outputs_elem(ymuint32 name_id)
{
  BlifNode* node = mNetwork->get_node(name_id);
  mNetwork->mPOArray.push_back(node);
  return true;
}

// @brief .names 文の処理
bool
BlifNetworkHandler::names(const vector<ymuint32>& name_id_array,
			  ymuint32 nc,
			  const char* cover_pat,
			  char opat)
{
  ymuint32 n = name_id_array.size();
  ymuint32 ni = n - 1;
  ymuint32 name_id = name_id_array[ni];
  BlifNode* node = mNetwork->get_node(name_id);
  if ( !mNetwork->set_logic_type(node, ni, nc, cover_pat, opat) ) {
    return false;
  }
  for (size_t i = 0; i < ni; ++ i) {
    ymuint32 id = name_id_array[i];
    BlifNode* inode = mNetwork->get_node(id);
    node->mFanins[i] = inode;
  }
  
  return true;
}

// @brief .gate 文の開始
// @param[in] loc1 .gate の位置情報
// @param[in] loc2 セル名の位置情報
// @param[in] name セル名
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
BlifNetworkHandler::gate_begin(const FileRegion& loc1,
			       const FileRegion& loc2,
			       const char* name)
{
  return true;
}

// @brief .gate 文中のピン割り当ての処理
// @param[in] loc1 ピン名の位置情報
// @param[in] f_name ピン名
// @param[in] loc2 ノード名の位置情報
// @param[in] a_name ノード名
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
BlifNetworkHandler::gate_assign(const FileRegion& loc1,
				const char* f_name,
				const FileRegion& loc2,
				const char* a_name)
{
  return true;
}

// @brief .gate 文の終了
bool
BlifNetworkHandler::gate_end()
{
  return true;
}
  
// @brief .latch 文の読み込み
bool
BlifNetworkHandler::latch(ymuint32 name1_id,
			  ymuint32 name2_id,
			  const FileRegion& loc4,
			  char rval)
{
  BlifNode* node2 = mNetwork->get_node(name2_id);
  if ( !mNetwork->set_latch_type(node2, rval) ) {
    return false;
  }
  BlifNode* node1 = mNetwork->get_node(name1_id);
  node2->mFanins[0] = node1;
  
  return true;
}
  
// @brief 終了処理
// @param[in] loc 位置情報
bool
BlifNetworkHandler::end(const FileRegion& loc)
{
  // 各ノードの名前の領域を確保する．
  ymuint32 n = mNetwork->max_node_id();
  for (ymuint32 id = 0; id < n; ++ id) {
    BlifNode* node = mNetwork->mNodeArray[id];
    if ( node == NULL ) continue;
    mNetwork->set_node_name(node, id2str(id));
  }
  return true;
}

// @brief 通常終了時の処理
void
BlifNetworkHandler::normal_exit()
{
}

// @brief エラー終了時の処理
void
BlifNetworkHandler::error_exit()
{
  mNetwork->clear();
}


//////////////////////////////////////////////////////////////////////
// BlifNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID 番号
BlifNode::BlifNode(ymuint32 id) :
  mId(id),
  mType(kUndef),
  mNi(0),
  mFanins(NULL),
  mNc(0),
  mCover(NULL)
{
}

// @brief デストラクタ
BlifNode::~BlifNode()
{
}


//////////////////////////////////////////////////////////////////////
// BlifNetwork
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BlifNetwork::BlifNetwork() :
  mAlloc(4096)
{
}

// @brief デストラクタ
BlifNetwork::~BlifNetwork()
{
}

// @brief 内容をクリアする．
void
BlifNetwork::clear()
{
  mName = string();
  mNodeArray.clear();
  mPIArray.clear();
  mPOArray.clear();
  mFFArray.clear();
  mLogicArray.clear();
}
  
// @brief ノードを取り出す．
// @param[in] id ID 番号
// @note なければ作成する．
BlifNode*
BlifNetwork::get_node(ymuint32 id)
{
  ymuint32 n = mNodeArray.size();
  while ( n <= id ) {
    mNodeArray.push_back(NULL);
    ++ n;
  }
  
  BlifNode* node = mNodeArray[id];
  if ( node == NULL ) {
    void* p = mAlloc.get_memory(sizeof(BlifNode));
    node = new (p) BlifNode(id);
    mNodeArray[id] = node;
  }
  return node;
}
  
// @brief ノード名を設定する．
// @param[in] node 対象のノード
// @param[in] name 名前
void
BlifNetwork::set_node_name(BlifNode* node,
			   const char* name)
{
  ymuint32 l = strlen(name);
  void* p = mAlloc.get_memory(l + 1);
  char* buff = new (p) char[l + 1];
  node->mName = buff;
  while ( (*buff ++ = *name ++) ) ;
}

// @brief ノードを外部入力に設定する．
// @note 既に他の型に設定されていたら false を返す．
bool
BlifNetwork::set_input_type(BlifNode* node)
{
  if ( node->mType != BlifNode::kUndef ) {
    return false;
  }
  node->mType = BlifNode::kInput;
  mPIArray.push_back(node);
  return true;
}

// @brief ノードを論理ノードに設定する．
// @param[in] ni ファンイン数
// @param[in] nc キューブ数
// @param[in] cover_pat 全キューブのパタンをつなげたもの
// @param[in] opat 出力のパタン
// @note 既に他の型に設定されていたら false を返す．
bool
BlifNetwork::set_logic_type(BlifNode* node,
			    ymuint32 ni,
			    ymuint32 nc,
			    const char* cover_pat,
			    char opat)
{
  if ( node->mType != BlifNode::kUndef ) {
    return false;
  }
  node->mType = BlifNode::kLogic;
  alloc_fanin(node, ni);
  node->mNc = nc;
  
  if ( ni > 0 ) {
    ymuint32 size = nc * ni;
    void* p = mAlloc.get_memory(size);
    char* buff = new (p) char[size];
    node->mCover = buff;
    while ( size -- > 0 ) {
      *buff ++ = *cover_pat ++;
    }
  }
  else {
    node->mCover = NULL;
  }
  node->mOpat = opat;
  
  mLogicArray.push_back(node);
  return true;
}

// @brief ノードをラッチノードに設定する．
// @param[in] rval リセット値 ( '0', '1', ' ' のいづれか )
// @note 既に他の型に設定されていたら false を返す．
bool
BlifNetwork::set_latch_type(BlifNode* node,
			    char rval)
{
  if ( node->mType != BlifNode::kUndef ) {
    return false;
  }
  node->mType = BlifNode::kLatch;
  alloc_fanin(node, 1);

  node->mOpat = rval;
  
  mFFArray.push_back(node);
  return true;
}

// @brief BlifNode のファンイン領域を確保する．
// @param[in] node 対象のノード
// @param[in] ni ファンイン数
void
BlifNetwork::alloc_fanin(BlifNode* node,
			 ymuint32 ni)
{
  node->mNi = ni;
  void* q = mAlloc.get_memory(sizeof(BlifNode*) * ni);
  node->mFanins = new (q) BlifNode*[ni];
}

// @brief 内容を blif 形式で出力する．
// @param[in] s 出力先のストリーム
void
BlifNetwork::write_blif(ostream& s) const
{
  s << ".model " << name() << endl;
  for (vector<BlifNode*>::const_iterator p = mPIArray.begin();
       p != mPIArray.end(); ++ p) {
    const BlifNode* node = *p;
    s << ".inputs " << node->name() << endl;
  }
  for (vector<BlifNode*>::const_iterator p = mPOArray.begin();
       p != mPOArray.end(); ++ p) {
    const BlifNode* node = *p;
    s << ".outputs " << node->name() << endl;
  }
  for (vector<BlifNode*>::const_iterator p = mFFArray.begin();
       p != mFFArray.end(); ++ p) {
    const BlifNode* node = *p;
    const BlifNode* inode = node->fanin(0);
    s << ".latch " << inode->name() << " " << node->name();
    if ( node->opat() != ' ' ) {
      s << node->opat();
    }
    s << endl;
  }
  for (vector<BlifNode*>::const_iterator p = mLogicArray.begin();
       p != mLogicArray.end(); ++ p) {
    const BlifNode* node = *p;
    s << ".names";
    ymuint32 ni = node->ni();
    for (ymuint32 i = 0; i < ni; ++ i) {
      const BlifNode* inode = node->fanin(i);
      s << " " << inode->name();
    }
    s << " " << node->name() << endl;
    ymuint32 nc = node->nc();
    if ( ni > 0 ) {
      for (ymuint32 c = 0; c < nc; ++ c) {
	for (ymuint32 i = 0; i < ni; ++ i) {
	  s << node->cube_pat(c, i);
	}
	s << " " << node->opat() << endl;
      }
    }
    else if ( nc == 1 ) {
      s << node->opat();
    }
  }
  s << ".end" << endl;
}


//////////////////////////////////////////////////////////////////////
// BlifNetworkReader
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BlifNetworkReader::BlifNetworkReader() :
  mHandler(new BlifNetworkHandler)
{
  mParser.add_handler(mHandler);
}

// @brief デストラクタ
BlifNetworkReader::~BlifNetworkReader()
{
  // mHandler は BlifParser が責任を持って破壊してくれる．
}

// @brief BLIF 形式のファイルを読み込む
// @param[in] filename ファイル名
// @param[out] network 読み込んだ内容を設定するネットワーク
// @param[in] msg_handler メッセージハンドラ
// @retval true 正常に読み込めた
// @retval false 読み込み中にエラーが起こった．
bool
BlifNetworkReader::read(const string& filename,
			BlifNetwork& network)
{
  mHandler->set_network(&network);
  
  bool stat = mParser.read(filename);
  if ( !stat ) {
    return false;
  }
  
  return true;
}

// @brief メッセージハンドラを付加する．
void
BlifNetworkReader::add_msg_handler(MsgHandler* msg_handler)
{
  mParser.add_msg_handler(msg_handler);
}

END_NAMESPACE_YM_BLIF

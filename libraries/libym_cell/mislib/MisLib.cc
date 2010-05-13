
/// @file libym_cell/MisLib.cc
/// @brief MisLib の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MisLib.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

#if HAVE_CONFIG
#include <ymconfig.h>
#endif

#include <ym_cell/MisLib.h>
#include <ym_cell/Msg.h>
#include "MisLibPt.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス MisLibGate
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] ni 入力数
MisLibGate::MisLibGate(const string& name,
		       double area,
		       const ShString& oname,
		       ymuint32 ni) :
  mName(name),
  mArea(area),
  mPins(ni),
  mOName(oname)
{
}

// @brief デストラクタ
MisLibGate::~MisLibGate()
{
}

// @brief 内容を表示する．
void
MisLibGate::dump(ostream& s) const
{
  s << "GATE " << name() << " "
    << fixed << setprecision(2) << area() << " "
    << oname() << " = " << expr() << ";" << endl;
  for (ymuint32 i = 0; i < ni(); i ++) {
    const MisLibPin* pin = ipin(i);
    const char* phase;
    switch ( pin->mPhase ) {
    case 0:  phase = "UNKNOWN"; break;
    case 1:  phase = "NONINV"; break;
    case -1: phase = "INV"; break;
    default: phase = "XXX"; break;
    }
    s << "  PIN " << pin->mName << " " << phase << " "
      << pin->mLoad << " " << pin->mMaxLoad << " "
      << pin->mRise_delay.mBase << " " << pin->mRise_delay.mFanout << " "
      << pin->mFall_delay.mBase << " " << pin->mFall_delay.mFanout << endl;
  }
  s << endl;
}


//////////////////////////////////////////////////////////////////////
// @class MisLibMsgHandler 
// @brief MisLib 用のメッセージハンドラ
//////////////////////////////////////////////////////////////////////
class MisLibMsgHandler :
  public MsgHandler
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] mask マスク
  MisLibMsgHandler(list<string>& err_list,
		   ymuint32 mask = kMsgMaskAll);

  /// @brief デストラクタ
  virtual
  ~MisLibMsgHandler();

  /// @brief メッセージを処理する．
  /// @param[in] msg メッセージ
  virtual
  void
  operator()(const Msg* msg);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力先
  list<string>& mErrList;
  
};


// @brief コンストラクタ
// @param[in] mask マスク
MisLibMsgHandler::MisLibMsgHandler(list<string>& err_list,
				   ymuint32 mask) :
  MsgHandler(mask),
  mErrList(err_list)
{
}

// @brief デストラクタ
MisLibMsgHandler::~MisLibMsgHandler()
{
}

// @brief メッセージを処理する．
// @param[in] msg メッセージ
void
MisLibMsgHandler::operator()(const Msg* msg)
{
  ostringstream os;
  os << "[";
  switch ( msg->type() ) {
  case Msg::kError:   os << "ERROR-"; break;
  case Msg::kWarning: os << "WARN-"; break;
  case Msg::kInfo:    os << "INFO-"; break;
  case Msg::kFail:    os << "FAIL-"; break;
  case Msg::kDebug:   os << "DEBUG-"; break;
  }
  os << msg->label() << "]: ";

  switch ( msg->type() ) {
  case Msg::kError:
  case Msg::kWarning:
    msg->dump_file_region(os);
    os << ": " << msg->body() << endl;
    break;
  case Msg::kInfo:
    msg->dump_file_region(os);
    os << ": " << msg->body() << endl;
    break;
  case Msg::kFail:
    os << msg->body() << endl;
    break;
  case Msg::kDebug:
    msg->dump_file_region(os);
    os << ": " << msg->body() << endl;
    break;
  }
  mErrList.push_back(os.str());
}


//////////////////////////////////////////////////////////////////////
// クラス MisLib
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MisLib::MisLib()
{
}

// @brief デストラクタ
MisLib::~MisLib()
{
  clear();
}

// @brief 内容をクリアする．
void
MisLib::clear()
{
  mDict.clear();
  
  ymuint32 n = mGates.size();
  for (ymuint32 i = 0; i < n; ++ i) {
    delete mGates[i];
  }
  mGates.clear();
}

// @brief ファイルを読み込む
bool
MisLib::read(const string& filename,
	     list<string>& err_list)
{
  MisLibPtMgr ptmgr(this);
  MisLibMsgHandler mhandler(err_list);
  MsgMgr::the_mgr().reg_handler(&mhandler);
  
  bool stat = ptmgr.read(filename);
  if ( !stat ) {
    return false;
  }
  
  return true;
}
  
// @brief 新たなセルを加える．
MisLibGate*
MisLib::new_gate(const string& name,
		 double area,
		 const ShString& oname,
		 ymuint32 ni)
{
  MisLibGate* gate = new MisLibGate(name, area, oname, ni);
  mGates.push_back(gate);
  mDict.insert(make_pair(gate->name(), gate));
  return gate;
}

END_NAMESPACE_YM_CELL

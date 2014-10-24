# coding=UTF-8

from grammer import *

# LR(1)構文解析表を作るモジュール
#
# - LR(1) 項は (rule_id, pos, token) という3つ組で表す．
class LR1_table :
    """LR(1)構文解析表を作るモジュール
       - LR(1) 項は (rule_id, pos, token) という3つ組で表す．"""

    # コンストラクタ
    def __init__(self) :
        self.clear()

    # 内容をクリアする．
    def clear(self) :

        # 状態リスト
        # 状態とは項の集合
        self._StateList = []

        # ACTION マップ
        self._ActionMap = []

    # @brief LR(1)構文解析表を作る．
    def make_table(self, grammer) :
        self.clear()
        self.items(grammer)
        for state in self._StateList :
            state_id = self.state2id(state)
            self._ActionMap.append({})
            for (rule_id, pos, token) in state :
                (left, right) = grammer.id2rule(rule_id)
                if len(right) > pos :
                    # 次のトークンを読む．
                    token_id = right[pos]
                    tmp_state = LR1_next_state(grammer, state, token_id)
                    if grammer._TerminalList[token_id] :
                        next_id = self.state2id(tmp_state)
                        assert next_id != -1
                        if not self.set_action(state_id, token_id, 'shift', next_id) :
                            print "Error: not an LR(1)"
                    else :
                        if len(tmp_state) > 0 :
                            next_id = self.state2id(tmp_state)
                            if not self.set_action(state_id, token_id, 'goto', next_id) :
                                print "Error: not an LR(1)"
                else :
                    # 末尾の場合
                    if left == grammer._StartNode :
                        if not self.set_action(state_id, 0, 'accept', 0) :
                            print "Error: not an LR(1)"
                    else :
                        if not self.set_action(state_id, token, 'reduce', rule_id) :
                            print "Error: not an LR(1)"

    # @brief ACTION 表の項目をセットする．
    # @param[in] state_id 状態番号
    # @param[in] token_id 記号番号
    # @param[in] action 動作
    # @param[in] action_id 動作に関連した番号
    #
    # 以前の設定内容と矛盾が生じた場合には False を返す．
    def set_action(self, state_id, token_id, action, action_id) :
        if self._ActionMap[state_id].has_key(token_id) :
            if self._ActionMap[state_id][token_id] != (action, action_id) :
                return False
            else :
                return True
        else :
            self._ActionMap[state_id][token_id] = (action, action_id)
            return True

    # @brief LR(1)正準集を作る．
    def items(self, grammer) :
        start_state = LR1_closure(grammer, [(grammer._StartRule, 0, 0)])

        self._StateList = []
        self._StateList.append(start_state)
        new_states = []
        new_states.append(start_state)
        while len(new_states) > 0 :
            cur_states = new_states
            new_states = []
            for state in cur_states :
                for token_id in range(0, len(grammer._TokenList)) :
                    new_state = LR1_next_state(grammer, state, token_id)
                    if len(new_state) > 0 and not new_state in self._StateList :
                        self._StateList.append(new_state)
                        new_states.append(new_state)

    # @brief LR(1)状態リストを表示する．
    def print_states(self, grammer) :
        for state in self._StateList :
            state_id = self.state2id(state)
            print 'State#%d:' % state_id
            print ''
            LR1_print_terms(grammer, state)
            print ""
            for key in self._ActionMap[state_id].keys() :
                (action, action_id) = self._ActionMap[state_id][key]
                token = grammer.id2token(key)
                print '  %s: %s %d' % (token, action, action_id)
            print ''

    # @brief 状態番号を返す．
    # @param[in] state 状態(項の集合)
    def state2id(self, state) :
        state_id = 0
        for state1 in self._StateList :
            if state1 == state :
                return state_id
            state_id += 1
        else :
            return -1

if __name__ == '__main__' :
    # テストプログラム
    g = Grammer()

    S = g.add_token('S')
    C = g.add_token('C')
    c = g.add_token('c')
    d = g.add_token('d')

    g.add_rule(S, (C, C))
    g.add_rule(C, (c, C))
    g.add_rule(C, (d, ))

    g.set_start(S)

    g.print_rules()

    g.print_tokens()

    lr1_tab = LR1_table()

    lr1_tab.make_table(g)

    lr1_tab.print_states(g)

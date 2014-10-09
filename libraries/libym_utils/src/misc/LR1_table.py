# coding=UTF-8

import grammer

# LR(1)構文解析表を作るモジュール
#
# - LR(1) 項は (rule_id, pos, token) という3つ組で表す．

class LR1_table :
    # コンストラクタ
    def __init__(self) :
        self.clear()

    # 内容をクリアする．
    def clear(self) :

        # 状態リスト
        # 状態とは項の集合
        self.m_StateList = []

        # ACTION マップ
        self.m_ActionMap = []

    # @brief LR(1)構文解析表を作る．
    def make_table(self, grammer) :
        self.clear()
        self.items(grammer)
        for state in self.m_StateList :
            state_id = self.state2id(state)
            self.m_ActionMap.append({})
            for token_id in range(0, len(grammer.m_TokenList)) :
                if grammer.m_TerminalList[token_id] :
                    for (rule_id, pos, token) in state :
                        (left, right) = grammer.id2rule(rule_id)
                        if len(right) > pos and right[pos] == token_id :
                            tmp_state = next_state(grammer, state, token_id)
                            next_id = self.state2id(tmp_state)
                            assert next_id != -1
                            if not self.set_action(state_id, token_id, 'shift', next_id) :
                                print "Error: not an LR(0)"
                else :
                    tmp_state = next_state(grammer, state, token_id)
                    if len(tmp_state) > 0 :
                        next_id = self.state2id(tmp_state)
                        if not self.set_action(state_id, token_id, 'goto', next_id) :
                            print "Error: not an LR(0)"
            for (rule_id, pos, token) in state :
                (left, right) = grammer.id2rule(rule_id)
                if len(right) == pos :
                    if left == grammer.m_StartNode :
                        if not self.set_action(state_id, 0, 'accept', 0) :
                            print "Error: not an LR(0)"
                    else :
                        if not self.set_action(state_id, token, 'reduce', rule_id) :
                            print "Error: not an LR(0)"

    # @brief ACTION 表の項目をセットする．
    # @param[in] state_id 状態番号
    # @param[in] token_id 記号番号
    # @param[in] action 動作
    # @param[in] action_id 動作に関連した番号
    #
    # 以前の設定内容と矛盾が生じた場合には False を返す．
    def set_action(self, state_id, token_id, action, action_id) :
        if self.m_ActionMap[state_id].has_key(token_id) :
            if self.m_ActionMap[state_id][token_id] != (action, action_id) :
                return False
            else :
                return True
        else :
            self.m_ActionMap[state_id][token_id] = (action, action_id)
            return True

    # @brief LR(1)正準集を作る．
    def items(self, grammer) :
        start_state = closure(grammer, [(grammer.m_StartRule, 0, 0)])

        self.m_StateList = []
        self.m_StateList.append(start_state)
        new_states = []
        new_states.append(start_state)
        while len(new_states) > 0 :
            cur_states = new_states
            new_states = []
            for state in cur_states :
                for token_id in range(0, len(grammer.m_TokenList)) :
                    new_state = next_state(grammer, state, token_id)
                    if len(new_state) > 0 and not new_state in self.m_StateList :
                        self.m_StateList.append(new_state)
                        new_states.append(new_state)

    # @brief LR(1)状態リストを表示する．
    def print_states(self, grammer) :
        for state in self.m_StateList :
            state_id = self.state2id(state)
            print 'State#%d:' % state_id
            print ''
            print_terms(grammer, state)
            print ""
            for key in self.m_ActionMap[state_id].keys() :
                (action, action_id) = self.m_ActionMap[state_id][key]
                token = grammer.id2token(key)
                print '  %s: %s %d' % (token, action, action_id)
            print ''

    # @brief 状態番号を返す．
    # @param[in] state 状態(項の集合)
    def state2id(self, state) :
        state_id = 0
        for state1 in self.m_StateList :
            if state1 == state :
                return state_id
            state_id += 1
        else :
            return -1

# @brief LR(1)項集合の遷移先を求める．
# @param[in] terms 入力の項集合
def next_state(grammer, cur_state, token) :
    tmp_state = []
    for (rule_id, pos, token1) in cur_state :
        (left, right) = grammer.id2rule(rule_id)
        if len(right) > pos and right[pos] == token :
            tmp_state.append( (rule_id, pos + 1, token1) )
    return closure(grammer, tmp_state)

# @brief LR(1)項の閉包演算を行う．
# @param[in] terms 入力の項集合(リスト)
# @return terms に対する閉包(項のリスト)を返す．
def closure(grammer, terms) :
    ans_terms = list(terms)
    new_terms = list(terms)
    while len(new_terms) > 0 :
        cur_terms = new_terms
        new_terms = []
        for (rule_id, pos, token) in cur_terms :
            (left, right) = grammer.id2rule(rule_id)
            if len(right) > pos :
                head = right[pos]
                for rule1_id in range(0, len(grammer.m_RuleList)) :
                    (left1, right1) = grammer.id2rule(rule1_id)
                    if left1 == head :
                        rest = list(right[pos + 1:])
                        rest.append(token)
                        for token1 in grammer.first(rest) :
                            term1 = (rule1_id, 0, token1)
                            if not term1 in ans_terms :
                                ans_terms.append(term1)
                                new_terms.append(term1)
    ans_terms.sort()
    return ans_terms

# @brief LR(1)項集合を表示する．
def print_terms(grammer, terms) :
    for (rule_id, pos, token) in terms :
        (left, right) = grammer.id2rule(rule_id)
        line = "  Rule (%d): " % rule_id
        line += grammer.m_LeftFormat % grammer.id2token(left)
        cur = 0
        for token_id in right :
            line += ' '
            if cur == pos :
                line += '. '
            line += grammer.id2token(token_id)
            cur += 1
        if cur == pos :
            line += " ."
        line += ", %s" % grammer.id2token(token)
        print line

if __name__ == '__main__' :
    # テストプログラム
    g = grammer.Grammer()

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

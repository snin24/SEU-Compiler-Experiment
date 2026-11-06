keywords = {
    'if':     'IF',
    'then':   'THEN',
    'else':   'ELSE',
    'end':    'END',
    'repeat': 'REPEAT',
    'until':  'UNTIL',
    'read':   'READ',
    'write':  'WRITE',
}

def recognize_keys_and_ids(src,pos):
    if not 0 <= pos < len(src):
        return None

    start = pos
    state = 0
    while pos < len(src):
        now = src[pos]
        is_letter = 'a' <= now <= 'z' or 'A' <= now <= 'Z'

        if state == 0:      #状态0检测到a~z,A~Z,'_'的时候进入状态1，否则推出
            if not (is_letter or now == '_'):
                return None
            state = 1
        elif not (is_letter or '0' <= now <= '9' or now == '_'):
            # 遇到非法字符，标识符结束
            break

        pos += 1    #状态1的时候，持续向后读

    lexeme = src[start:pos]
    token_type = keywords.get(lexeme, 'ID')     #如果不是在keywords里面，就是ID标识符
    return token_type, lexeme, pos

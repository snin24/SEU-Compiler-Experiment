'''
识别所有整数
'''

def recognize_number(src,pos):
    if not 0 <= pos < len(src):
        return None

    start = pos
    state = 0
    while pos < len(src):
        is_num = '0' <= src[pos] <= '9'

        if state == 0:
            if not is_num:
                return None
            state = 1
        else:
            if not is_num:
                break
        pos += 1

    return 'NUM', src[start:pos], pos

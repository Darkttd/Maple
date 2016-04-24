# -*- coding: utf-8 -*-
# author: halakshin@gmail.com

with open(u'characters.txt') as f:
    characters = list(f.read().decode(u'utf-8'))  # 문자 모음 파일을 불러옵니다.

import fontforge
target_font = fontforge.open(u'NanumGothic_bak.ttf')  # 글꼴 파일을 불러옵니다.
target_selection = target_font.selection

characters.append(u' ')

for character in characters:
    code = 'u%05X' % ord(character)  # 'A' -> 65 -> 'u00041'과 같은 형태로 변환합니다.
    target_selection.select(('more',), code)  # 문자를 추가 선택합니다.

target_selection.invert()  # 선택을 반전합니다.
target_font.clear()  # 선택한 글자들을 모두 제거합니다.

target_font.generate(u'NanumGothic.ttf')  # 최적화된 글꼴 파일을 저장합니다.
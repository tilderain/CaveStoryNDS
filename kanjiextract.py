# -*- coding: utf-8 -*-
import re

''' This is a library of functions and variables that are helpful to have handy 
	when manipulating Japanese text in python.
	This is optimized for Python 3.x, and takes advantage of the fact that all strings are unicode.

	Copyright (c) 2014-2015, Mads Sørensen Ølsgaard
	All rights reserved.
	Released under BSD3 License, see http://opensource.org/licenses/BSD-3-Clause or license.txt '''




## UNICODE BLOCKS ##

# Regular expression unicode blocks collected from 
# http://www.localizingjapan.com/blog/2012/01/20/regular-expressions-for-japanese-text/

hiragana_full = r'[ぁ-ゟ]'
katakana_full = r'[゠-ヿ]'
kanji = r'[㐀-䶵一-鿋豈-頻]'
radicals = r'[⺀-⿕]'
katakana_half_width = r'[｟-ﾟ]'
alphanum_full = r'[！-～]'
symbols_punct = r'[、-〿]'
misc_symbols = r'[ㇰ-ㇿ㈠-㉃㊀-㋾㌀-㍿]'
ascii_char = r'[ -~]'

## FUNCTIONS ##

def extract_unicode_block(unicode_block, string):
	''' extracts and returns all texts from a unicode block from string argument.
		Note that you must use the unicode blocks defined above, or patterns of similar form '''
	return re.findall( unicode_block, string)

def remove_unicode_block(unicode_block, string):
	''' removes all chaacters from a unicode block and returns all remaining texts from string argument.
		Note that you must use the unicode blocks defined above, or patterns of similar form '''
	return re.sub( unicode_block, '', string)

## EXAMPLES ## 

text = "々、「」。ｒ２－７３（＋４．ｚ６Ｍ～？Ｘ１９ＤＰ！）　、、、、、ジホネクビョエタワヌパユミゲサイオトロォガボゾナダヨゼィノケカェマニムペウァューポン・キザゴヘギバリシプチブグレヤヒハヴモゥピメラドルソヶセヅコスズデャフツアヲテベッ、、、、、、、、、、、、、よてかるえすあちうまでゆぐぉぢりぬおらさへどくろやんみずぽわことづねっごぅぼいぇふぺのぜほぱればゃめぃながぎきせぶつをげにひしだぞぁざたけょじべはむもそび供ⅢⅠ：･♪■↑Ⅳ↓…←Ⅱ→、、、、、秩説祈臓一回土免只耐特戴叩別攻完借島戦到基会健康沈入素身丈撃床集悔増品浅魚友準序殴切屋血魔仕試我発紙仇失宿痴報以体拾昔這着妖始店跳的廃制待家言陽空事含良保技済建様与貯勢背所果当実鋭安炉毎局炭光題白子落全団電下永軍親黙敷終目悪頼庫役逃味客傍違爆藤元記思徊棒刑行乗飽真惜逆伝使勝汎稼国浸大絡仲徘平牢破廊兄銀凝塊固灰随強捕朽詳持賭調複進優能返盾葉喋懐彷燃導唯弾騒闇選痛隊三因狙貴後塗書起扇意策小妬食娘鍵繰死怪険歳呑限走日込楽恩修縮機襲暗昇存釣士緒腕作多天雑労解崩前夫戻流具洞鬼焼階証阿来奴座関開似払釜培域結閉降疲転丁俺受接製踏段笑道効闊考念砂裂酷次百応泉無腹印甘暴出弱性冠父病支検暖太許手確糧悲砲怖農憑新械横旦震八壁散未奥区久自定療取割花鈴同鼻曲炎呆覚傷第離給他勘万慎幸時料脈扉徨録協剣爺伊操長愚駄索過宜復便射係住頭谷宝今冶器危獄不化損産押触困晴願諦再恐析憎爽義窟員成為骸付運相沢然途置面物通猛比方世練若講姿知犬罠超話民語吹室用主達十風極武黄威口力才凶系活純憶彫質延正補最盗赤眺述育蓮期少術山巣命想火美板渡識加感形頂者護噴博打原臆研私聖池何石勇蓋惑獣声密罰人常判夜厳度心可闘備芽苦察残管君星魅二骨晶遅装妻礼殺退溜除鉄刻鍛要救断止飲立月男変本喰迷処夢難宮休半臭表状喪送卵計症旅悟園愛飛囲分謎地放双種狂馴必及僕反遠斜級噂嫌配馬漂格合得婆助檻登蘇絆査贈暮歯外奇早封惨揮沼台有舞範順防援張名冷咲指動北虫挑聞服了利滅辺究連腰消見任突足択遣験在上隠害望問倒木迎側栄掛探液初水投負雄年追嫉詰英潜騙古和令金呼竜決墓居浮静端黒杯怒先絶魂牙栽肉工慕奪息中法屈寂箱精壊呂称対従姉妙幾業敗答末売村文理荒明昆顔去泥閃近抜快禁頑源寝医減邪停塞坂祖教信音色潰派向兵欲脱番像続広即移眠交差引呪秘裕数読換部帰高銃匹孵功場内拠野点茶代母貫染女診予争幽気熱画倉程重徹態裏注婦飢看歩速敵肥薬王案造帽枯材現誰忘守生示神単異弟巨穴急余字遊墟乱情劇彼忙好路適働責間監視個駐跡憩玉卓胎唄鼓圧迫灼虎将、、、、、、、、、、"

text2 = "年庫義小呂妖育宮快陽防床送鼻正１鉄北切係ゥづ八英よ旅客ヶ危処斜び腹苦痛増遣巨序泉沢ン精窟注染研ぽハ倒鋭暗熱急武話牢ポ銃存炭配勘肉君毎起ち↓コ蘇砂は徘従能牙借段換彷婆オ掛馴りひ獄途ェ竜交呼徊噴剣流ヌキ案俺踏←徹接製釜音旦達好ダぐ倉ん弟数孵：服み作家以国勇長療ガ～横査ァ弱塗酷感似が必咲ル襲順今蓮黄臓こ主破険サ洞劇民ゼ不盾巣美安っ働合才、迫閃後呆鬼択字液ゲ寂聖特舞リ塊昔脈禁猛病了免転敗程惑護ヲ試浸沼…す栄マ救ャモ匹品末薬覚報的戦ュ坂ぺ現表徨争屈講器壊理ペ潜実や揮用械婦日．団売残唄証望策白７識ヒ雑ピ悪画撃何怖ネ焼ト定ｚ夜減行謎真イ自爺崩ら。忘聞進罠ズ態高土常Ｍ随ぁ予工居噂隠守路示物素礼帰廃王遅過せ放葉姉計惨教勝頭木忙晴持産欲彼奇宝手ほ利止続払端デ祖詰空夢中状脱けつ暖セ恐三卵朽超食ぇ修タ私遊親芽完レ刻檻杯逆多上ｒユ元威落部近幾反昆言震散飲将対の優ゃゴ圧重技損解怪迷足比乗域・読失な鍛狂２邪荒燃む源連限ぉ愚向↑呪厳退し惜性ラ力冶ニ谷期て指寝ジ押制色魅プ店及農投所そ晶ス索詳双赤絡異園ゾ果茶炎集便隊語６ド発ぬ絆ソ月跳廊形ホ補無装？支書準命変拠度除開基離敵称万内百」肥仲伝培勢和協練探と運い遠有ぃ刑囲終花方若追ど広彫ょ余裂カ最ー令張得妻析代闇さ当穴明で妬傍漂一娘症浅眠法ヴ渡へ石歳ギ側諦ロ泥！ぼねあ枯黙闊るメ到骨純ぱ野攻停栽戴Ｘ歩３兄ぎ降天腰９山速役世チ触ィミ喋炉二然汎乱軍心島だず全ア付眺関ヨ飽姿同貫狙ケ屋込「虫う拾稼早ブ墓受第打待復疲住捕固通死説呑夫凝帽糧動壁光可を番村考れ奴抜少前ッノ要ナⅠ導意助テ逃雄Ｂ丈憑任見信ヘ顔ざフ博大凶登確＋ク着）賭悟パ傷悔休活兵給回じⅡ体ォビ電ボえ父懐質笑鍵想極闘格水嫌扇建置弾適ぶ害保紙騒因問来獣Ｇ暴灰駄飛憶複検嫉気先判星犬未もグ走滅割面墟宜与浮困功ベ（階成歯玉吹久違応康答丁バに台殴た名冷ぅ新返贈引ョ願くヅ伊頼初負士砲ツ化消慕即貯出生Ⅳ箱者造め々愛在射封魂貴古口子身閉腕文ぢ調宿阿十妙取強魔げ選罰這密太池我祈情お料胎蓋鼓始奥扉健声記暮棒相ば別Ｄ４道只述裕済思魚辺憎盗ま爆裏援座ウ種ぞ像時恩派息診挑女ザ供灼　母材半良■血業単友印慎知機悲神絶区験か移究爽操管立ゆＰ念範他様昇術臆会医分事効ご人銀責鈴沈背外飢使誰藤決秘－含原許僕突為幽入断局冠べ黒甘幸秩次察頂加潰ぜ喰差縮仇火戻点級労塞板味員題釣場シき室目金地殺具馬静虎延間下本怒風痴唯骸喪再溜頑系迎去耐わ看ヤエ備永Ⅲふ平ワ敷繰緒仕ム♪叩結ろ臭→録奪男騙"

text = extract_unicode_block(alphanum_full, text)
text2 = extract_unicode_block(alphanum_full, text2)

with open("totFix.txt", "w") as f:
	for x in text2:
		if x not in text:
			print(x)
			f.write(str(x))




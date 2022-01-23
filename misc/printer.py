#this script determines the width of each character in the font
#so that the non-monospace text display will have a distance of 1 pixel to the next character

texts = \
[
" !\"#$%&`()*+,-./0123456789:;<=>?",
"@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_",
"'abcdefghijklmnopqrstuvwxyz{|}~~",
"々、「」。ｒ２－７３（＋４．ｚ６Ｍ～？Ｘ１９ＤＰ！）　、、、、、",
"ジホネクビョエタワヌパユミゲサイオトロォガボゾナダヨゼィノケカェ",
"マニムペウァューポン・キザゴヘギバリシプチブグレヤヒハヴモゥピメ",
"ラドルソヶセヅコスズデャフツアヲテベッＢＧ、、、、、、、、、、、",
"よてかるえすあちうまでゆぐぉぢりぬおらさへどくろやんみずぽわこと",
"づねっごぅぼいぇふぺのぜほぱればゃめぃながぎきせぶつをげにひしだ",
"ぞぁざたけょじべはむもそび供ⅢⅠ：･♪■↑Ⅳ↓…←Ⅱ→、、、、、",
"秩説祈臓一回土免只耐特戴叩別攻完借島戦到基会健康沈入素身丈撃床集",
"悔増品浅魚友準序殴切屋血魔仕試我発紙仇失宿痴報以体拾昔這着妖始店",
"跳的廃制待家言陽空事含良保技済建様与貯勢背所果当実鋭安炉毎局炭光",
"題白子落全団電下永軍親黙敷終目悪頼庫役逃味客傍違爆藤元記思徊棒刑",
"行乗飽真惜逆伝使勝汎稼国浸大絡仲徘平牢破廊兄銀凝塊固灰随強捕朽詳",
"持賭調複進優能返盾葉喋懐彷燃導唯弾騒闇選痛隊三因狙貴後塗書起扇意",
"策小妬食娘鍵繰死怪険歳呑限走日込楽恩修縮機襲暗昇存釣士緒腕作多天",
"雑労解崩前夫戻流具洞鬼焼階証阿来奴座関開似払釜培域結閉降疲転丁俺",
"受接製踏段笑道効闊考念砂裂酷次百応泉無腹印甘暴出弱性冠父病支検暖",
"太許手確糧悲砲怖農憑新械横旦震八壁散未奥区久自定療取割花鈴同鼻曲",
"炎呆覚傷第離給他勘万慎幸時料脈扉徨録協剣爺伊操長愚駄索過宜復便射",
"係住頭谷宝今冶器危獄不化損産押触困晴願諦再恐析憎爽義窟員成為骸付",
"運相沢然途置面物通猛比方世練若講姿知犬罠超話民語吹室用主達十風極",
"武黄威口力才凶系活純憶彫質延正補最盗赤眺述育蓮期少術山巣命想火美",
"板渡識加感形頂者護噴博打原臆研私聖池何石勇蓋惑獣声密罰人常判夜厳",
"度心可闘備芽苦察残管君星魅二骨晶遅装妻礼殺退溜除鉄刻鍛要救断止飲",
"立月男変本喰迷処夢難宮休半臭表状喪送卵計症旅悟園愛飛囲分謎地放双",
"種狂馴必及僕反遠斜級噂嫌配馬漂格合得婆助檻登蘇絆査贈暮歯外奇早封",
"惨揮沼台有舞範順防援張名冷咲指動北虫挑聞服了利滅辺究連腰消見任突",
"足択遣験在上隠害望問倒木迎側栄掛探液初水投負雄年追嫉詰英潜騙古和",
"令金呼竜決墓居浮静端黒杯怒先絶魂牙栽肉工慕奪息中法屈寂箱精壊呂称",
"対従姉妙幾業敗答末売村文理荒明昆顔去泥閃近抜快禁頑源寝医減邪停塞",
"坂祖教信音色潰派向兵欲脱番像続広即移眠交差引呪秘裕数読換部帰高銃",
"匹孵功場内拠野点茶代母貫染女診予争幽気熱画倉程重徹態裏注婦飢看歩",
"速敵肥薬王案造帽枯材現誰忘守生示神単異弟巨穴急余字遊墟乱情劇彼忙",
"好路適働責間監視個駐跡憩玉卓胎唄鼓圧迫灼虎将、、、、、、、、、、",
]

from PIL import Image
img = Image.open("smalfont.png")
pixels = list(img.getdata())
w,h = img.size
print(pixels)
charas = []
for y in range(36):
	line = []
	for x in range(32):
		bit = []
		for sy in range(13):
			row = []
			for sx in range(12):
				a = x*12
				b = y * 13 * 384
				c = (sy*384)
				row.append(pixels[a+b+c+sx])
			bit.append(row)

		line.append(bit)

	charas.append(line)

'''
for chara in charas:
	for rows in chara:
		for row in rows:
			for a in row:
				print(5 if a == 5 else ".", end="")
			print()
		print()
	print()
'''
i = 0
j = 0
for chara in charas:
	for rows in chara:
		right = 0
		for row in rows:
			for i in range(len(row)):
				#each row[i] is the color index
				#so if it's 1 then that is the color white (if the palette is right)
				if row[i] == 1 and i > right:
					right = i
				
				#print(4 if row[i] == 4 else ".", end="")
				
			#print()
			
		#minimum width is 3
		add = 3 if j > 2 else 2
		print(str(right+add) + ",", end="")
		#print()
		#print()
	i+=1
	if i == 12:
		print("")
		i=0
		j += 1



'''
		right = 0
		for sy in range(13):
			for sx in range(12):
				a = x*12
				b = y * 13 * 384
				c = (sy*384)
				if pixels[a+b+c+sx] == 5 and sx+1 > right:
					right = sx+1

		print(str(right+1) + ",", end="")
	print()
'''

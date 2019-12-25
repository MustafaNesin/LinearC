
# LinearC
Lineer cebir uygulamalarıyla matrisler üzerinde işlem yapabilen ve lineer denklem sistemi çözebilen Konsol uygulaması.

# Komutlar
### 1. Genel Komutlar
 - return;	"Ana menüye geri döndürür."
 - clear;	"Ekranı temizler."
 - list;	"Matris adlarını listeler."
 - deg;		"Derece açı birimine geçer."
 - degree;	"Derece açı birimine geçer."
 - rad;		"Radyan açı birimine geçer."
 - radian;	"Radyan açı birimine geçer."
***
### 2. Matris İşlemleri
#### 2.1. Genel Matris İşlemleri
- t(M);	Transpoz
- inv(M);	Ters
- det(M);	Determinant
- adj(M);	Ek Matris
- r(M);	Rank
- rank(M);	Rank
- get(M, R, C);		Eleman Okuma
- set(M, S, R, C);	Eleman Yazma

#### 2.2. Özel Matrisler
- id(R);	Birim Matris
- diag(S, R); Tüm Köşegen Elemanları Eşit Matris
- low(S, R); Tüm Alt Üçgen Elemanları Eşit Matris
- up(S, R); Tüm Üst Üçgen Elemanları Eşit Matris
- all(S, R, C);	Tüm Elemanları Eşit Matris

#### 2.3. Satır - Sütun İşlemleri
- rowop(M); Sıradaki Satır İşlemi
- rowswt(M, R1, R2);		R1 <-> R2
- rowmul(M, R1, S);		R1 -> S*R1
- rowadd(M, R1, R2, S);	R1 -> R1 + S*R2
- colop(M); Sıradaki Sütun İşlemi
- colswt(M, C1, C2);		C1 <-> C2
- colmul(M, C1, S);		C1 -> S*C1
- coladd(M, C1, C2, S);	C1 -> C1 + S*C2
***
### 3. Matematiksel İşlemler

#### 3.1. Genel Matematik İşlemleri
- pow(S, P); Üs
- pow(M, P); Üs
- abs; Mutlak Değer
- pi;		PI
- e;		E
- sqrt(S);	Karekök  
- ln(S);	Doğal Logaritma
- log(S);	Onluk Logaritma
- log(S, B); Logaritma

#### 3.2. Trigonometrik İşlemler
##### 3.2.1. Trigonometrik İfadeler
- sin(S);	Sinüs
- cos(S);	Kosinüs
- tan(S);	Tanjant
- cot(S);	Kotanjant
- sec(S);	Sekant
- csc(S);	Kosekant

##### 3.2.2. Ters Trigonometrik İfadeler

- asin(S);	Ters Sinüs
- arcsin(S); Ters Sinüs
- acos(S); 	Ters Kosinüs
- arccos(S); Ters Kosinüs
- atan(S); Ters Tanjant
- arctan(S); Ters Tanjant
- acot(S); Ters Kotanjant
- arccot(S); Ters Kotanjant
- asec(S); Ters Sekant
- arcsec(S); Ters Sekant
- acsc(S);	Ters Kosekant
- arccsc(S); Ters Kosekant

##### 3.2.3. Hiperbolik İfadeler
- sinh(S);	Sinüs Hiperbol
- cosh(S); Kosinüs Hiperbol
- tanh(S); Tanjant Hiperbol
- coth(S); Kotanjant Hiperbol
- sech(S); Sekant Hiperbol
- csch(S); Kosekant Hiperbol

##### 3.2.4. Ters Hiperbolik İfadeler
- asinh(S); Ters Sinüs Hiperbol
- arcsinh(S); Ters Sinüs Hiperbol
- acosh(S); Ters Kosinüs Hiperbol
- arccosh(S); Ters Kosinüs Hiperbol
- atanh(S); Ters Tanjant Hiperbol
- arctanh(S); Ters Tanjant Hiperbol
- acoth(S); Ters Kotanjant Hiperbol
- arccoth(S); Ters Kotanjant Hiperbol
- asech(S); Ters Sekant Hiperbol
- arcsech(S); Ters Sekant Hiperbol
- acsch(S); Ters Kosekant Hiperbol
- arccsch(S); Ters Kosekant Hiperbol

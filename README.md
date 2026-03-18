# Github-projektek:

## Első projekt
A feladat egy olyan program és a hozzá tartozó GLSL shader fájlok elkészítése, amely egy kör megjelenítésére és animációjára képes. A kör a képernyő széleit érintve, megfelelő szögben visszapattan, miközben mozgása folyamatos marad (a lépéshossz falérintéskor arányosan módosul).

### Követelmények

#### 1. Ablak létrehozása
- A program hozzon létre egy **600×600 pixel méretű négyzetes ablakot**.
- A háttér színe legyen **sárga** (az árnyalat szabadon választható).

#### 2. Kör megjelenítése
- A képernyő közepén jelenjen meg egy:
  - **r = 50 pixel sugarú kör**
  - **zöld határvonallal**
  - **piros centrummal**
- A határ és a centrum között **lineáris színinterpoláció** legyen.
- Tipp: a kör középpontját érdemes **uniform változóként** átadni a fragment shader számára.
- A zöld és piros színek árnyalata szabadon választható.

#### 3. Kör mozgása
- A kör a képernyő közepéről induljon.
- Mozogjon **vízszintesen (x-tengely mentén)** oda-vissza.
- A képernyő szélét **pontosan érintve pattanjon vissza**.

#### 4. Vízszintes szakasz
- A képernyő közepén jelenjen meg egy:
  - **kék színű**
  - **vízszintes szakasz**
  - hossza: **az ablak oldalhosszának egyharmada**
  - vastagsága: **3 pixel**

#### 5. Felhasználói interakció
- A szakasz mozgatható legyen:
  - **Fel nyíl** → felfelé mozgatás
  - **Le nyíl** → lefelé mozgatás

#### 6. Metszésvizsgálat és színcsere
- Amikor a kör **nem metszi** a szakaszt:
  - a **határ (zöld)** és a **centrum (piros)** színei **cserélődjenek fel**
- Ez a változás az interpolált belső pontokra is hasson.

### Saját ötlet

#### Színváltoztatás
- Az **A** és **D** billentyűk segítségével legyen lehetőség a színek módosítására.

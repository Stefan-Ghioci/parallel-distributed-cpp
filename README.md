---
title: '[]{#_rq78n4twacqr .anchor}**[PPD: Laborator 1b - C++]{dir="ltr"}**'
---

**[Analiza cerințelor]{dir="ltr"}**
===================================

**[[Cerințe]{.underline}]{dir="ltr"}**
--------------------------------------

[Înmulțirea a două "numere mari" (= numere reprezentate pe tablouri de numere întregi fără semn, ordonate crescător după cea mai nesemnificativă cifră din număr), folosind algoritmi ce lucrează **secvențial** sau **paralel**.]{dir="ltr"}

**[[Constrângeri]{.underline}]{dir="ltr"}**
-------------------------------------------

-   [Datele de intrare (cele două numere de înmulțit) se vor citi dintr-un fișier generat anterior rulării programului, cu numere aleatoare având între x și y cifre (parametri variabili)]{dir="ltr"}

-   [Numărul de thread-uri este un parametru variabil, dat la rulare]{dir="ltr"}

-   [Programul va afișa la final timpul de execuție corespunzător fiecărei implementări]{dir="ltr"}

-   [Încărcarea echilibrată a fiecărui thread]{dir="ltr"}

-   [Folosirea funcțiilor implementate la laboratorul precedent (lab0)]{dir="ltr"}

[**Proiectare**]{dir="ltr"}
===========================

**[[Sumar]{.underline}]{dir="ltr"}**
------------------------------------

[În **main**.cpp începe execuția programului, ce execută o serie de funcții (statice, pentru eficiență, împărțite intuitiv în clase pe directoare, în funcție de responsabilitate și utilitate).]{dir="ltr"}

[]{dir="ltr"}![][1]

[Se generează două numere aleatoare în fișiere. Se citesc cele două numere și se apelează succesiv algoritmii secvențial și paralel. Se printează timpul execuției fiecăruia și se salvează rezultatele intr-un fișier Excel (doar după ce se testează egalitatea rezultatelor).]{dir="ltr"}

[]{dir="ltr"}

[Soluția acestui laborator este construită pe baza celei din laboratorul anterior (adunarea a 2 numere),]{dir="ltr"}

[deoarece majoritatea funcțiilor utile si algoritmii implementați se pot refolosi în mod eficient și rapid în implementarea algoritmilor de înmulțire.]{dir="ltr"}

[]{dir="ltr"}

[]{dir="ltr"}

**[[Diagrama de clase]{.underline}]{dir="ltr"}**
------------------------------------------------

[]{dir="ltr"}

> ![][2][]{dir="ltr"}

[**[Descrierea algoritmului]{.underline}**]{dir="ltr"}
------------------------------------------------------

[]{dir="ltr"}

1.  [[Varianta secvențială]{.underline}]{dir="ltr"}

    a.  [Se alege un deînmulțit și un înmulțitor dintre cele două numere (optim înmulțitorul va fi numărul mai scurt)]{dir="ltr"}

    b.  [Pentru fiecare cifră din înmulțitor, se calculează produsul dintre cifră și înmulțitor în mod iterativ (cifră cu cifră), rezultând o listă de **N** produse parțiale ( N = numărul de cifre al înmulțitorului)]{dir="ltr"}

    c.  [Se adună toate produsele parțiale, iterativ, adunând număr cu număr la un subtotal, folosind algoritmul de adunare secvențială de la lab. **1a**]{dir="ltr"}

2.  [[Varianta paralelă]{.underline}]{dir="ltr"}

    d.  [Identic variantei secvențiale, se aleg deînmulțitul și înmulțitorul]{dir="ltr"}

    e.  [Se alocă fiecărui din cele **P** thread-uri un interval de cifre din înmulțitor ( N/P + restul de cifre, distribuite în mod aproximativ egal)]{dir="ltr"}

    f.  [Fiecare thread calculează N/P produsuri parțiale între deînmulțit și cifrele înmulțitorului din intervalul primit și le adaugă intr-o listă comună]{dir="ltr"}

    g.  [Pentru **optimizare**, cele N produsuri parțiale sunt adunate în mod secvențial de **P** thread-uri, fiecare thread adunând N/P numere]{dir="ltr"}

    h.  [Cele N/P sume rezultate din adunarea produsurilor parțiale se vor aduna în mod secvențial de P thread-uri, fiecare adunând N/(P\^2) numere; astfel, se va repeta pasul **d** până când lista de numere de adunat are o dimensiune mai mică decât 2\*P (adică până când nu mai există un minim de 2 numere de adunat pentru fiecare thread)]{dir="ltr"}

    i.  [Rezultatul final se obține din adunarea secvențială a numerelor rămase (\<2\*P)]{dir="ltr"}

**[[Optimizare]{.underline}]{dir="ltr"}**
-----------------------------------------

-   [Folosirea unor tipuri de date optime (ca dimensiune, acces și siguranță):]{dir="ltr"}

    -   [vectori de tip **unsigned char** (echivalent **byte** in Java)]{dir="ltr"}

-   [Pasarea datelor folosind referinte **&**, folosind tipul **const**, oriunde e permis]{dir="ltr"}

-   [Folosirea containelor și funcțiilor specifice bibliotecii **STL**, pentru a folosi cod eficient și optimizat, dar și pentru a păstra algoritmii paraleli **thread-safe**, din punct de vedere al accesului concurent al datelor comune]{dir="ltr"}

-   [Obiecte create **local**, funcții care nu copiază date, pentru a evita crearea de memory leak-uri si a lăsa memoria să fie gestionată eficient în mod implicit]{dir="ltr"}

**[Testare]{dir="ltr"}**
========================

![Chart][]{dir="ltr"}

**[[Condiții de testare]{.underline}]{dir="ltr"}**
--------------------------------------------------

-   [Processor Intel Core i5-8250U CPU @ 1.60GHz, 1800 Mhz, 4 Cores, 8 Logical]{dir="ltr"}

-   [8 GB RAM]{dir="ltr"}

-   [Performanța testată pe 2\^15 (\~33 mii) cifre]{dir="ltr"}

-   [5 rulări pentru fiecare caz (1/2/4/8 thread-uri)]{dir="ltr"}

-   [unitatea pentru măsurarea timpului de execuție - **milisecunde**]{dir="ltr"}

  [1]: media/image1.png {width="2.617481408573928in" height="3.213542213473316in"}
  [2]: media/image2.png {width="8.74101268591426in" height="4.286458880139983in"}
  [Chart]: media/image3.png {width="6.270833333333333in" height="3.611111111111111in"}

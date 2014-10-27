EGC - Geometry Wars

Usurelu Catalin Constantin 
333CA

1. Cerinta
    Se cere implementarea unui joc gen Geometry Wars. Mai pe scurt, cel putin 3 tipuri de nave,
jucatorul sa aibe ca arme burghiu si eventual proiectile, simularea coliziunilor sa fie realista
si sa avem si scor, vieti, eventual nivele de dificultate.

2. Utilizare
    Nu este nevoie de comenzi la consola, doar rularea executabilului.
    
    Input Keyboard:
        - tasta a -> activare/dezactivare burghiu
        - tasta spatiu -> trage proiectil
        - sageti -> miscare pe directiile corespunzatoare
        - ESC -> iesire din joc
        
3. Implementare
    Sistem de operare: Windows 8
    IDE: Visual Studio Ultimate 2012 (cu compilatorul sau desigur). Trebuie activat RTTI pentru
         ca folosesc niste dynamic_cast-uri.
         
    Algoritmi coliziune:
        - Ma folosesc de produsul vectorial destul de mult. Am gasit intr-o carte (Programare in limbajul
          C/C++ pentru liceu, editura Polirom) formula produsului vectorial si utilizarea lui pentru a 
          determina intersectia de segmente. Mai pe scurt, prin produsul vectorial putem afla daca unul din 2
          segmente care pornesc din acelasi punct trebuie rotit la stanga sau la dreapta pentru a se suprapune cu
          celalalt (ma folosesc si la algoritmul de A.I. de asta).
          
          Ca sa verific daca 2 segmente se intersecteaza fac in felul urmator: un segment intersecteaza o dreapta
          daca capetele se afla de o parte si de cealalta a acesteia, deci facem produs vectorial pentru ambele
          capete si un punct de pe segmentul celalalt. Analog pentru cealalta dreapta. Verificam daca pentru un
          capat obtinem negativ si pentru celalalt pozitiv (adica trebuie rotit in sens invers trigonometric,
          respectiv trigonometric, deci unul se aflta de o parte si altul de cealalta).
          
          Daca exista coliziune, facem ciocnire - aplicam formulele de la fizica, considerand ca toate obiectele au
          masa 1 (iar peretele masa infinit, deci sta pe loc).
          
          Am reprezentat pentru fiecare obiect, viteza in forma vectoriala (sa fie realistic), directie (ca sa stie
          unde aplica acceleratia - modificand astfel vectorul viteza) si coeficienti de franare liniara respectiv
          unghiulara (si rotatia e realista).
          
          Algoritmul de A.I. foloseste tot produsul vectorial. Practic, avand centrul navei mele (calculat ca mijlocul
          cercului) P, centrul navei inamice (centroid) E si directia navei inamice D, putem face urmatorul lucru:
          Translatam nava inamica in origine si facem produs vectorial intre directia sa  si segmentul determinat de 
          punctul (0, 0) si centrul jucatorului. Daca rezultatul e negativ, trebuie sa ne miscam in sensul acelor de
          ceasornic ca sa suprapunem directia inamicului cu centru jucatorului si analog invers. Daca produsul e destul
          de aproape de 0, mergem inainte. Daca e aproape de 0, dar burghiul e ON atunci ne miscam la dreapta (arbitrar
          ales) ca sa ne ferim.
          
    Design OOP:
         Am folosit mai multe clase, in spiritul OOP, de aceea o sa las doar diagrama valabila in Visual Studio (ClassDiagram.cd) si o sa explic putin:
            - Am o clasa Movable cu caracteristici de miscare si componente de obiect (figuri 2D). Ea sta la baza a tot
            ceea ce se poate "misca" : nave, "perete", proiectile
            - Un engine de fizica - implementeaza Observer Pattern pentru a actualiza obiectele pe care le observa
            - Ship - baza tuturor navelor, are ca parinte pe Movable
            
 4. Testare - am testat toate cazurile posibile, merge bine :)
 
 5. Probleme:
    - A trebuit sa imi aduc aminte multe subtilitati de C++ si a trebuit sa rezolv multe erori incredibil de greu de depanat.
    - Cazurile de ciocniri intre diverse tipuri de obiecte
    - Actualizare scorurilor / mesajelor - odata afisat un text, nu puteam sa dau pur si simplu wait ca sa il mentina afisat, ca se
    bloca totul. Am adaugat niste if-uri in plus ca sa rezolv asta.
    
 6. Continutul arhivei:
    - Framework-ul cu multe modificari gen operatori +, * pentru Point2D (pentru compunere de viteze mai formala)
    - Altele (sper ca nu am sarit ceva):
        - Readme.txt
        - GeometryWars.cpp - main
        - Ship.cpp/h
        - Movable.cpp/h
        - PhysicsEngine.cpp/h
        - PlayerShip.cpp/h - jucatorul
        - PlayerShip2D.cpp/h - forma navei - poligon
        - Chain.cpp/h - inamic
        - Plane.cpp/h - inamic
        - Elice.cpp/h - inamic
        - Torpedo.cpp/h - proiectil
        - Wall.cpp/h - perete
        
 7. Functionalitati:
    - toate din enunt - cele standard
    - bonus:
        - accelerare, franare, viteza realista (vectorial, e mai mult decat ce cerea bonusul :D)
        - mai multe nivele
        - inamici inteligenti, te urmaresc si evita burghi-ul
        - proiectile
        - ciocniri realiste - ca la fizica (nu era la bonus, dar la fel, e mai mult decat atat)

        
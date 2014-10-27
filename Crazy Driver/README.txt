EGC - CRAZY DRIVER

Usurelu Catalin Constantin 
333CA

1. Cerinta
    Se cere implementarea unui joc cu masini. Exista trei benzi. Exista obstacole mobile si fixe,
fiecare pe banda lui.

2. Utilizare
    Nu este nevoie de comenzi la consola, doar rularea executabilului.
    
    Input Keyboard:
        - sageti -> miscare pe directiile corespunzatoare
        - ESC -> iesire din joc
        
3. Implementare
    Sistem de operare: Windows 8
    IDE: Visual Studio Ultimate 2012 (cu compilatorul sau desigur). Trebuie activat RTTI pentru
         ca folosesc niste dynamic_cast-uri.
         
    Atentie!!! - apare o eroare la inceput pentru ca am generat class diagram si am sters fisierul sdf, dati ok si eventual,
                 daca e nevoie, dati view class diagram pentru diagrama
    Algoritmi coliziune:
        - Folosesc algoritmul de intersectie a doua dreptunghiuri cu laturile paralele cu axele de coordonate.
        - Practic, calculez bounding box-ul i planul XOZ pentru fiecare obiect, apoi verific intersectia de 
          bounding box-uri obtinute.
          
    Design OOP:
         Am folosit mai multe clase, in spiritul OOP, de aceea o sa las doar diagrama valabila in Visual Studio (ClassDiagram.cd) si o sa explic putin:
            - Am o clasa Movable cu caracteristici de miscare si componente de obiect (figuri 3D). Ea sta la baza a tot
            ceea ce se poate "misca" : jucator, masini, obstacole, marcaje delimitatoare
            - Un engine de fizica - implementeaza Observer Pattern pentru a actualiza obiectele pe care le observa
            
    Transform3D:
        - A trebuit sa impart applyTransform in applyTransform (modifica doar vertices) si applyPerspectiveTransform (adauga rezultatul in transf_points
          ca sa afiseze). Motivul este ca am vrut ca odata aplicata o transformarea aceasta sa ramana aplicata. De asemenea vreau sa calculez coliziunile
          pe obiecte inainte de a aplica perspectiva (altfel modifica aiurea coordonatele).
            
 4. Testare - am testat toate cazurile posibile, merge bine :)
 
 5. Probleme:
    - A trebuit sa impart matricea MVP in MV si P, adica doua operatii separate. Fac asta deoarece am modificar applyTransform astfel incat,
      un obiect sa retina permanent noua pozitie (adica fara proiectie inca). Proiectia o aplic dupa ce am aplicat transformarile necesare.
      De asemenea applyTransform, modifica doar vertices, applyPerspectiveTransform modifica transf_vertices, cele care sunt defapt afisare.
    
 6. Continutul arhivei:
    - Framework-ul cu multe modificari gen operatori +, * pentru Point2D (pentru compunere de viteze mai formala)
    - Altele (sper ca nu am sarit ceva):
        - Readme.txt
        - main.cpp
        - Car.cpp
        - Car.h
        - Enemy.cpp
        - Enemy.h
        - Obstacle.cpp
        - Obstacle.h
        - RoadStripes.h
        - RoadStripes.cpp
        - PhysicsEngine.h
        - PhysicsEngine.cpp
        - PhysicsProperties.cpp
        - PhysicsProperties.h
        - Colors.h
        - UnitCube.cpp
        - UnitCube.h
        - Road.cpp
        - Road.h
        - Wheel.cpp
        - Wheel.h
        - PowerUp.cpp
        - PowerUp.h
        
        
 7. Functionalitati:
    - toate din enunt - cele standard
    - bonus:
        - obstacole bonus care adauga un scor mai mare (si isi schimba culoarea cand se misca)
        - modele geometrice mai complexe pentru jucator si celelalte masini - au roti
        - diferite animatii aplicate masinii - rotile se invart in functie de viteza actuala - ca in realitate.
          De asemenea, e mai greu decat pare sa rotesti masini si rotile sa se roteasca corect pe noua directie.
          Trebuie ca roata sa fie translatata in centru, rotita pe directia inainte si apoi rotita circular (axa OX)
        - sistem consum carburant
        - poligoane pline - strada, stripe-urile si power-upurile. Celelalte obiecte aratau urat umplute - isi pierdeau forma, adica
          aratau mai bine ca wireframe, se vedea cum sunt construite.

        
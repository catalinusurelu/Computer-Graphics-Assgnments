EGC - Atletism

Usurelu Catalin Constantin 
333CA

1. Cerinta
    Se cere implementarea unui joc cursa de atletism 3D. Exista 3 jucatori, de culori
diferite (aici rosu, galben si albastru). Exista 5 camera, 3 camera in spatele fiecarui
juactor, una deasupra stadionului si una TPS pe marginea stadionului care urmareste jucatorul
principal. Dupa ce un jucator castiga, se afiseaza un steag fluturand in culoarea lui.

2. Utilizare
    Nu este nevoie de comenzi la consola, doar rularea executabilului.
    
    Input Keyboard:
        - tasta 1 -> activare camera jucator 1
        - tasta 2 -> activare camera jucator 2
        - tasta 3 -> activare camera jucator 3
        - tasta 4 -> activare camera TPS
        - tasta 5 -> activare camera deasupra stadionului
        - tasta 6 -> resetarea pozitie initiala a camerei deasupra stadionului
        - sageti -> miscare pe directiile corespunzatoare
        - i -> activarea wireframe
        - w -> translatare camera FPS inaite
        - s -> translatare camera FPS inapoi
        - a -> translatare camera FPS stanga
        - d -> translatare camera FPS dreapta
        - r -> translatare camera FPS sus
        - f -> translatare camera FPS jos
        - q -> rotire camera FPS pe OY in sens trigonometric
        - e -> rotire camera FPS pe OY in sens invers trigonometric
        - z -> rotire camera FPS pe OZ in sens trigonometric
        - c -> rotire camera FPS pe OZ in sens invers trigonometric
        - t -> rotire camera FPS pe OX in sens trigonometric
        - g -> rotire camera FPS pe OX in sens invers trigonometric
        - ESC -> iesire din joc
        
3. Implementare
    Sistem de operare: Windows 8
    IDE: Visual Studio Ultimate 2012 (cu compilatorul sau desigur).
         
    Stadionul l-am facut de mana. In principiu partea de centru (2 semicercuri si un dreptunghi a fost usoara).
Pentru marcajele alb, culoare si tribune am folosit o functie care genereaza obiecte circularea cu forma specifica
unui culora/marcaj/tribuna. Mi-a fost usor ca o refoloseam pentru tot. A fost greu totusi sa o fac, pentru ca in 3D
a trebui sa fac triunghiuri pentru toate fetele de mana. Arata mai ciudat codul, pentru ca pun toti indicii intr-un
spatiu continuu de memorie si astfel am practic 4 linii circulare cu puncte, Trebuie sa fac fetze cu triunghiuri cu
punctele de pe aceste linii. Am folosit o functie increment care are grija ca atunci cand punctul incrementat depaseste
o valoare (ajunge la sfarsit) sa o ia de la capat. E echivalent cu modulo, doar ca avand 4 linii pe acelasi vector nu pot
sa fac modulo, ca ar lua-o de la 0 nu de la offsetul liniei.

    Jucatorii i-am importat. Ca sa simulez miscarea realista import 15 obiecte (keyframe) si la fiecare frame nou este
afisat altul, dand senzatia de miscare.

    Am creat un cer, si sursa de iluminare se afla sus (da senzatia de soare). Pozitia ochiului este pozitia camerei curente.
    
    Pentru a misca circular camera TPS, calculez normala la semicerc (practic vectorul centru semicerc -> jucator, normalizat) si mut camera
cu un offset (sa fie in afara stadionului) pe directia normalei, fata de centrul semicercului. Pentru miscarea modelului camerei
stiind vectorul centru semicerc si cel al jucatorului, calculez unghiul dintre ele (cu formula 
cos a = vector1 * vector2 / lungimevector1 * lungimevector2) si rotesc corespunzator. Cam la fel fac si pentru rotirea modelelor
jucatorilor necontrolati. Pentru a misca efectiv jucatorii necontrolati, calculez normala (vecotr pozitie semicerc -> pozitie jucator)
si o rotesc 90 de grade, aceasta e tangenta la cer, deci noua directie de deplasare.

    Pentru a detecta castigatorul si sa fiu sigur ca daca ma intorc imediat inapoi nu castig (adica sa trec linia). Activez un checkpoint
cand trec de jumatatea stadionului si inca unul cand efectiv trec linia.
       
    Pentru simularea efectului de unduire al steagului, am creat un vertex shader care misca un vertex folosind o formula de unda.
Formula este asemanatoarea cu cea de la fizica, depinde de timp, frecventa si faza (faza o folosesc pentru a simula efectul undei intr-un
punct, adica un punct mai din stanga va avea unda decalata fata de un punct din dreapta) si o intensitate. Intensitatea este 0 langa batz-ul
steagului si 1 la margine. Astfel panza se unduieste mai mult la margine si deloc langa batz (pentru ca e lipita de el, adica e realistic).
Am facut un radical pentru intensitate pentru ca se modifica prea incet efectul de unda.
    Cand mut pozitia pe OZ, a vertexului (asta e idea principala), mai modific cu acelasi efect (dar injumatatit) si pe OX si OY. Adica, daca
avem o carpa si tragem de ea, se modific si pozitile din planull XOY (se strange).

            
 4. Testare - am testat toate cazurile posibile, merge bine :)
 
 5. Probleme:
    - Multa munca :)
    - Crearea stadionului a fost mai grea, a trebuit sa fac triunghiurile de mana, si pentru
      un obiect circular (gen tribunele) este destul de greu
    - Miscarea camerei TPS dupa jucator
    - Miscarea jucatorilor pe semicerc
    - Putine probleme pana mi-am dat seama cum sa scriu formula undei
    
 6. Continutul arhivei:
    - Framework-ul cu iluminare combinat cu cel cu camera
    - Altele (sper ca nu am sarit ceva):
        - Readme.txt
        - main.cpp
        - Runner.hpp -> atlet
        - Stadium.hpp
        - TPSCam.hpp
        - VertexFormat.h -> l-am mutat in fisierul asta
        - Sky
        - shadere/shader_phong_fragment.glsl
        - shadere/shader_phong_vertex.glsl
        - shadere/shader_wave_phong_fragment.glsl
        - shadere/shader_wave_phong_vertex.glsl
        
        
 7. Functionalitati:
    - toate din enunt - cele standard
    - bonus:
        - accelerare, decelerare
        - scena realista
        - alte elemente ce aduc realism jocului

        
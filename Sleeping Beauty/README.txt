EGC - Atletism

Usurelu Catalin Constantin 
333CA

1. Cerinta
    Se cere implementarea unei animatii prin interpolare si a unui sistem de particule.
Animatia reprezinta un model care reactioneaza la aparitia unuor note muzicale, si trece
prin 3 stari (somn, surpriza, suparare, somn, surpriza ...) la nesfarsit.
Mai pe scurt, apar notele muzicale, personajul se trezeste, iar cand isi da seama ca totul
e in regula notele dispar si se culca la loc. Acest lucru se repeta la nesfarsit

2. Utilizare
    Nu este nevoie de comenzi la consola, doar rularea executabilului.
    
    Input Keyboard:
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
        - i -> activarea wireframe
        - F1 -> Fullscreen 
        - F2 -> window sized screen
        - ESC -> iesire din joc
        
3. Implementare
    Sistem de operare: Windows 8
    IDE: Visual Studio Ultimate 2012 (cu compilatorul sau desigur).
         
    Sistemul de animatie l-am implementat in felul urmator:
        - am un automat finit mai special, in sensul ca se afla intre 2 stari. Adica
        nu intoarce ce e intr-o stare sau elementul consumat de o tranzitie, ci o "pozitie" de pe
        tranzitie ([0, 1] -> factor de umplere). De asemenea returneaza un vao care corespunde
        unei perechi de stari. Astfel, in shadere, pot calcula din cele 2 stari transmise, si factorul
        curent de umplere, o noua stare intermediara.
        
        - Fiecare tranzitie are 6 secunde (poate fi setata). Doar in primele 2 tranzitii apar note muzicale.
        Starea de sfarsit (somn) dureaza si ea 6 secunde.
        
        - Totul se repeta la nesfarsit (cum este descris in enunt).
        
        - la fiecare desenare de frame, automatul ne da blend_factorul s vao-ul corespunzator starii curente (intermediare).
        - trecerea intre 2 stari [A, B] [B, C] e tratata simplu de automat, acesta avand o variabila de timp generala,
        si stie daca timpul este in primul interval sau in al doilea (si eviden, blend factorul este calculat corect,
        acesta tinand cont doar de timp-ul din interval curent - defapt timpul de la inceputul intervalului).
        
        - iluminarea Phong foloseste drept componente speculare si de difuzie chiar textura (culoare pixelului pentru
          care calculeaza culoarea.
        
        
        Sistemul de particule:
        
        - acesta este calculat simplu dupa cum este descris in pdf. Idea este ca avem un dreptunghi centrat in (0,0,0).
        - stiind centrul lui, ecuatia de miscare a proiectiluli (pentru a calcula noul centru) si timpul curent, putem
          calcula noul centru. Acest nou centru (pozitia noua) il punem in view space (adica totul este"indreaptat spre noi"),
          iar noi vertexi ii calculam ca in pdf. Adica, stiind ce coltz reprezinta vertexul curent (stanga sus, dreapta jos, etc),
          putem sa translatam din centru pe ox si oy, sa corespunda aceleiasi pozitii, doar ca e indreptat spre noi (e billboard) - la fel
          cum se explica si in pdf.
        - pe langa ecuatia proiectilului (ecuatia pe oy, tinand cont de gravitatie) i-am mai dat si o miscare la viteza constanta in spate si dreapta
          personaj
        - particulele sunt generate la puncte random intr-un paraleliped (ca sa arate ca in exemplul dat de laborant) si indreptate 
          pe directia OX si putin OZ (si desigur tinand cont de gravitatie, cade jos incetul cu incetul).
        - pentru a textura particula pe mesha (quad) ii dam quadului coordonate UV pentru a putea aplica textura, ii dam o culoare si folosind
          alpha discard decidem daca afisam pixelul curent sau nu.
        
        Sincronizarea animatiei si sistemului de particule e simpla: notele muzicale apar la inceputul primei stari si dispar la sfarsitul
        celei de-a 2a tranzitii. Cand personajul adoarme din nou, doarme 6 secunde si totul se reia de la capat.
            
 4. Testare - am testat toate cazurile posibile, merge bine :)
 
 5. Probleme:
    - Nici o problema, poate doar creare unui automat finit
    
    
 6. Continutul arhivei:
    - Framework-ul cu iluminare combinat cu cel cu camera si cu cel de texturare
    - Altele (sper ca nu am sarit ceva):
        - Readme.txt
        - main.cpp
        - VertexFormat.h
        - Finite_state_automata.h - automatul finit
        - Particle.h - mesh-ul particulei e implentat ca un singleton (sa nu folosim aiurea resursele)
        - shader_particle_fragment.glsl
        - shader_particle_vertex.glsl
        - shader_phong_fragment.glsl
        - shader_phong_vertex.glsl
        
        
 7. Functionalitati:
    - toate din enunt - cele standard

        
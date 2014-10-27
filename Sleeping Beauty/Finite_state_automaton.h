#ifndef FSM_H
#define FSM_H

#include <ctime>
#include <chrono>
#include <vector>
#include <utility>

#include "VertexFormat.h"

// Automat finit liniar :)
// Poate trece doar intr-o singura stare urmatoare
// Practic e format doar din tranzitii
// Nu se poate trece direct (intr-un pas) dintr-o stare in alta
// Mereu automatul e pe o tranzitie (adica de obicei, in functie de timp
// e la trecere dintre o stare in alta. Ajunge intr-o stare (adica, capatul unei tranzitii) cand se 
// afla la inceputul unei tranzitii (adica [stare1, stare2))

class Finite_state_automaton
{
public:
    Finite_state_automaton()
    {
        blend_factor = 0;
        transition = 0;
        stopped = true;
    }

    void add_state(unsigned int state)
    {
        // nu ne trebuie pentru tipul asta de automat
    }

    void add_transition(unsigned int vao)
    {
        this->vao.push_back(vao);
    }

    void add_transition_times(int start_time, int finish_time)
    {
        time.push_back(std::pair<int, int>(start_time, finish_time));
    }

    unsigned int get_current_transition ()
    {
        return vao[transition];
    }

    float get_blend_factor()
    {
        return blend_factor;
    }
    
    void start()
    {
        if(stopped)
        {
            start_time = std::chrono::high_resolution_clock::now();
            stopped = false;
        }
    }

    void stop()
    {
        stopped = true;
    }

    void finish()
    {
        blend_factor = 0;
        transition = 0;
        stopped = true;
    }

    void update()
    {
        if(!stopped)
        {
            auto ellapsed_time = std::chrono::high_resolution_clock::now() - start_time;
            float time_milli = std::chrono::duration_cast<std::chrono::milliseconds>(ellapsed_time).count();
            float time_sec = std::chrono::duration_cast<std::chrono::seconds>(ellapsed_time).count();

            // fiecare tranzitie are un timp, deci numarul de timpi = numar de tranzitii
            // => i = numarul tranzitiei curente (tinand cont de timp)
            for(int i = 0; i < time.size(); i++)
            {
                if(time_sec >= time[i].first && time_sec < time[i].second)
                {
                    transition = i;

                    // aplic regula de 3 simpla pentru a afla
                    // factorul de "imbinare" pentru interpolarea lineara

                    // timp_total  ... 1
                    // timp_curent ... x
                    // x -> valoare blend_factor, (mapare [0, timp_total] la [0, 1])

                    blend_factor = (time_milli - time[i].first * 1000) / (float)((time[i].second - time[i].first) * 1000);
                    break;
                }
            }
        }
    }

    std::chrono::high_resolution_clock::time_point get_start_time()
    {
        return start_time;
    }
    
private:
    std::chrono::high_resolution_clock::time_point start_time;
    
    bool stopped;
    float blend_factor;

    int transition;

    std::vector<unsigned int> vao;
    std::vector<std::pair<int, int> > time;
};

#endif
#include <iostream>
#include <fstream>
#include <algorithm>
// #include <functional>
#include <numeric>

#include <boost/format.hpp>

using namespace std;


typedef double real;
typedef std::vector<real> Data;

const real x0    = -1.0;
const real x1    = +1.0;
const real span  = x1-x0;

const real g_cfl  = 0.2;
const real g_velocity = 0.5;

const real time_stop  = 0.5;

real f_eval_gauss(real x) {
    x = fmod(fabs(x - x0), span) + x0;
    return exp(-10*x*x);
}

// https://en.wikipedia.org/wiki/Courant%E2%80%93Friedrichs%E2%80%93Lewy_condition

int main()
{

    std::vector<size_t> levels {6, 7, 8, 10, 12, 13, 14, 15, 16};

    // output file
    std::ofstream file("/tmp/output.txt");
    file << "#level N error_1 error_2 error_3" << endl;

    for(const size_t &level: levels) {
        const size_t N = (1 << level) + 1;
        const real dx    = span/(N);
        // https://en.wikipedia.org/wiki/Courant%E2%80%93Friedrichs%E2%80%93Lewy_condition
        const real dt = g_cfl*dx/g_velocity;
        // http://www.exp.univie.ac.at/cp1/cp1-6/node72.html (closed form)
        static const real alpha = g_velocity*dt/dx;

        Data data;
        data.reserve(N);
        Data data2;
        data2.reserve(N);
        // initialization
        for(size_t i = 0; i < N; ++i) {
            real x = i*dx+x0;
            data[i] = data2[i] = f_eval_gauss(x);
        }
        const size_t clock_stop = ceil(time_stop/dt);
        cerr << "level: " << level << " steps: " << clock_stop << endl;

        // simulation
        for(size_t clock = 0; clock < clock_stop; ++clock) {
            data[0] = data2[0] - alpha/2*(data2[1]-data2[N-2]-alpha*(data2[1]-2*data2[0]+data2[N-2]));
            for(size_t j = 1; j < N-1; ++j){
                data[j] = data2[j] - alpha/2*(data2[j+1]-data2[j-1]-alpha*(data2[j+1]-2*data2[j]+data2[j-1]));
            }
            data[N-1] = data2[N-1] - alpha/2*(data2[0]-data2[N-2]-alpha*(data2[0]-2*data2[N-1]+data2[N-2]));

            data2 = data;
        }

        // error estimation
        real offset = clock_stop*dt*g_velocity;

        real error_1 = 0;
        real error_2 = 0;
        real error_3 = 0;
        for(size_t i = 0; i < N; ++i) {
            // acurate solution
            const real value_th = f_eval_gauss(i*dx-offset+x0);
            const real diff = fabs(value_th-data[i]);
            error_1 += diff;
            error_2 += diff*diff;
            if(diff > error_3) {
                error_3 = diff;
            }
        }
        error_1 = error_1/N;
        error_2 = sqrt(error_2)/N;

        file << (boost::format("%02d %06d %e %e %e") % level % N % error_1 % error_2 % error_3) << endl;
    }
    file.close();
    return 0;
}


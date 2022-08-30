#include <bits/stdc++.h>
#define MSS 1.0            // MSS size in KB
#define RWS 1024.0          // RWS size in KB
#define THRESH_MULT 0.5     // Threshold multiplier for every timeout
using namespace std;
void congest(string outfile,int T,double ki, double km, double kn, double kf, double ps  ) {
    double cw, threshold;       // Congestion window size and congestion threshold
    cw = ki * MSS;              // Initial command window size
    threshold = DBL_MAX;        // Set to infinity
    ofstream output;
    output.open(outfile, ios::out |ios::trunc);
    output << cw << "\n";       // Print initial CW size
    int num= 0;
     int slow=1;
    // Seed random number generator
    srand((unsigned)time(NULL));
    while(num < T) {
        // Send segments in the command window
        int size = ceil(cw / MSS);
        for(int i = 0; i < size ; i++) {
            // Generate random probability
            double prob = (double)rand()/ RAND_MAX;
            double p =1-ps;
            if(prob < p) {
                // Timeout; congestion detection phase
                threshold = THRESH_MULT * cw;   // Update threshold
                cw = max(1.0, kf * cw);         // Update CW size
               // slow_start = true;              // Restart from slow start phase
                    slow=1;
                output << cw << "\n";
                num++;
                break;
            }
            // Packet acknowledged successfully; update window size accordingly
            if(slow) {
                // In slow start phase
                cw = min(cw + km * MSS, RWS);

                // Check if CW size has reached threshold
                if(threshold<=cw) {
                    slow =0;
                }
            }
            else {
                // In congestion avoidance phase
                cw = min(cw + (kn * MSS * MSS / cw), RWS);
            }
            output << cw << "\n";
            num++;
            if(num >= T)  break;
        }
    }   
}
int main(int argc, char* argv[]) {
    double ki = 1.0, km = 1.0, kn = 1.0, kf, ps;
    int T;
    string out;
    // Parse command line arguments
    //char names[10] ={'i','m','n','f','s','T'};
    for(int i = 1; i < argc; i++) {     
        if(!strcmp(argv[i], "-i")) {
                i++;
            if(i < argc)  ki = stod(argv[i]);     
       }
         if(!strcmp(argv[i], "-m")) {
            i++;
            if(i < argc)   km = stod(argv[i]); 
        }
         if(!strcmp(argv[i], "-n")) {
            i++;
            if(i < argc)   kn = stod(argv[i]);       
        }
         if(!strcmp(argv[i], "-f")) {
            i++;
            if(i < argc)   kf = stod(argv[i]);     
        }
         if(!strcmp(argv[i], "-s")) {
            i++;
            if(i < argc)    ps = stod(argv[i]);       
        }
         if(!strcmp(argv[i], "-T")) {
            i++;
            if(i < argc) T = stoi(argv[i]);      
        }  
        if(!strcmp(argv[i], "-o")) {
            i++;
            if(i < argc)  out = argv[i];       
        }     
    }  
    // Invoke simulator
    congest(out,T,ki, km, kn, kf, ps);
    return 0;
}

   

